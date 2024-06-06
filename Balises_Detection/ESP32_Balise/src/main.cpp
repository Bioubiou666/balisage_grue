#include "main.h"


// --------------------------------------------------------------------- //
/*

Pour ajouter un tag il faut ajouter :

Dans les varibles :
- L'adresse MAC dans
const char* allMacAdress[]

- La valeur -110 dans
int allBeacon[]

- L'UUID de la characteristique apres les autre (on la genere en suivant la logique alphabetique de l'avant dernier charactere)
#define CHARACTERISTIC_BEACONX_UUID "beb5483e-36e1-4688-b7f5-ea07361b26x1"

- La declaration de la characteristique apres les autres
BLECharacteristic *pCharacteristicbeaconX;

Dans le setup() :
- La création du service de la characteristique
  pCharacteristicbeaconX = pService->createCharacteristic(CHARACTERISTIC_BEACONX_UUID,
                                                          BLECharacteristic::PROPERTY_READ
                                                          );

- Le setup du Callbacks
  pCharacteristicbeaconX->setCallbacks(new MyBLECallbacks());

Dans le doBLEScans() :
- La mise a jour des valeurs RSSI apres les autres
    pCharacteristicbeaconX->setValue(String(allBeacon[X]).c_str());
    pCharacteristicbeaconX->notify();


Si on ne veut que modifier un beacon alors on ne modifie que l'adresse MAC dans
const char* allMacAdress[]



AMELIORATION :
    - Systeme de mesure batterie et envoie sur une caracteristique

*/
// --------------------------------------------------------------------- //


Capteur::Capteur() {}

void Capteur::setup() {
    // Initialise les leds comme sortie
    pinMode(ledOn, OUTPUT);
    pinMode(ledRead, OUTPUT);

    digitalWrite(ledOn, HIGH);    // Allumer la LED

    Serial.begin(115200);

    // Initialisation service Server
    BLEDevice::init(DEVICE_NAME);
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks(*this));
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Creations caractéristiques beacons
    pCharacteristicbeaconT = pService->createCharacteristic(CHARACTERISTIC_BEACONT_UUID,
                                                            BLECharacteristic::PROPERTY_READ
                                                            );
    pCharacteristicbeacon1 = pService->createCharacteristic(CHARACTERISTIC_BEACON1_UUID,
                                                            BLECharacteristic::PROPERTY_READ
                                                            );
    pCharacteristicbeacon2 = pService->createCharacteristic(CHARACTERISTIC_BEACON2_UUID,
                                                            BLECharacteristic::PROPERTY_READ
                                                            );

    pCharacteristicbeaconT->setCallbacks(new MyBLECallbacks(*this));  
    pCharacteristicbeacon1->setCallbacks(new MyBLECallbacks(*this));
    pCharacteristicbeacon2->setCallbacks(new MyBLECallbacks(*this));

    // Lancement
    pService->start();
    startAdvertising();
}

void Capteur::loop() {
    // Vérifier si une seconde s'est écoulée depuis l'allumage de la LED
    handleLED();

    // Obtient le temps actuel
    currentMillis = millis();

    // Vérifie si l'intervalle est écoulé
    if (currentMillis - previousMillis >= interval) {
        // Enregistre le temps de cette exécution
        previousMillis = currentMillis;
        doBLEScans();
    }
}

void Capteur::doBLEScans() {
    // Retour info start port COM
    Serial.println("Scanning...");

    // Initialisation service Scan
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan();

    pBLEScan->setActiveScan(true);  // active scan rapide
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);        // pour connection smartphone
    BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
    pBLEScan->setActiveScan(false); // desactive scan rapide

    // Comparaison adresse mac
    int count = foundDevices.getCount();
    if (count > 0) {
        for (int i = 0; i < countMacAdress; i++) {
            allBeacon[i] = -110;
        }

        for (int i = 0; i < count; i++) {
            BLEAdvertisedDevice d = foundDevices.getDevice(i);
            const char* address = d.getAddress().toString().c_str();

            for (int j = 0; j < countMacAdress; j++) {
                if (strcmp(address, allMacAdress[j]) == 0) {
                    allBeacon[j] = d.getRSSI();
                }
            }
        }

        // Retour infos RSSI port COM
        for (int i = 0; i < countMacAdress; i++) {
            Serial.printf("RSSI %d : %d\n", i, allBeacon[i]); // 0 pour beacon test
        }
        Serial.printf("\n");

        // Mise à jour des caractéristiques BLE
        pCharacteristicbeaconT->setValue(String(allBeacon[0]).c_str());
        pCharacteristicbeaconT->notify();
        pCharacteristicbeacon1->setValue(String(allBeacon[1]).c_str());
        pCharacteristicbeacon1->notify();
        pCharacteristicbeacon2->setValue(String(allBeacon[2]).c_str());
        pCharacteristicbeacon2->notify();
    }
    else {
        Serial.println("No beacons found");
    }

    // Nettoyage valeurs RSSI
    pBLEScan->clearResults();
}

void Capteur::handleLED() {
    if (ledState && millis() - ledOnTime >= ledHighMax) {
        digitalWrite(ledRead, LOW); // Éteindre la LED
        ledState = false; // Indiquer que la LED est éteinte
    }
}

void Capteur::startAdvertising() {
    pServer->getAdvertising()->start();
    Serial.println("Advertising started");
}

MyBLECallbacks::MyBLECallbacks(Capteur& capteur) : capteur(capteur) {}

void MyBLECallbacks::onRead(BLECharacteristic *pCharacteristic) {
    digitalWrite(capteur.ledRead, HIGH); // Allumer la LED
    capteur.ledOnTime = millis(); // Enregistrer le temps d'allumage de la LED
    capteur.ledState = true; // Indiquer que la LED est allumée
}

MyServerCallbacks::MyServerCallbacks(Capteur& capteur) : capteur(capteur) {}

void MyServerCallbacks::onConnect(BLEServer* pServer) {
    Serial.println("Device connected");
}

void MyServerCallbacks::onDisconnect(BLEServer* pServer) {
    Serial.println("Device disconnected, restarting advertising");
    capteur.startAdvertising();
}

// Création de l'objet Capteur globalement
Capteur capteur;

void setup() {
    capteur.setup();
}

void loop() {
    capteur.loop();
}