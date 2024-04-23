#include <Arduino.h>

// Declarations BLE
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLEAdvertisedDevice.h>


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

*/
// --------------------------------------------------------------------- //

/*
Beacon non fonctionnels
                              "dc:23:4e:9c:f0:9c",
                              "dc:23:4e:9c:f3:c0"
*/
// Adresse mac beacons
const char* allMacAdress[] = {"84:fc:e6:00:83:ae", // beacon de test      "ec:da:3b:38:b4:6e"     CLEMENT
                              "84:fc:e6:00:83:ae",
                              "84:fc:e6:00:83:ae"
                              };
int countMacAdress = sizeof(allMacAdress) / sizeof(allMacAdress[0]);

// RSSI beacons
int allBeacon[] = {-110, -110, -110};


// UUID service et characteristique
#define SERVICE_UUID                "4fafc201-1fb5-459e-8fcc-c5c9c33191a1"
#define CHARACTERISTIC_BEACONT_UUID "beb5483e-36e1-4688-b7f5-ea07361b26b1"
#define CHARACTERISTIC_BEACON1_UUID "beb5483e-36e1-4688-b7f5-ea07361b26c1"
#define CHARACTERISTIC_BEACON2_UUID "beb5483e-36e1-4688-b7f5-ea07361b26d1"

BLEServer *pServer;
BLEScan* pBLEScan;
BLECharacteristic *pCharacteristicbeaconT;
BLECharacteristic *pCharacteristicbeacon1;
BLECharacteristic *pCharacteristicbeacon2;


// Temps de scan BLE, en secondes
const int scanTime = 1;

// Déclaration des variables globales
unsigned long previousMillis = 0;       // Stocke le temps de la dernière exécution
const long interval = 1000;              // Intervalle entre chaque exécution de la fonction en millisecondes
unsigned long currentMillis;            // Stocke le temps actuel


// Declaration fonction
void doBLEScans();


// definition leds
const int ledOn = 2;
const int led = 3;


// Variable temps d'allumage de la LED
unsigned long ledOnTime = 0;

// État de la LED
bool ledState = false;

// Callback sur lecture
class MyBLECallbacks : public BLECharacteristicCallbacks {
public:
    void onRead(BLECharacteristic *pCharacteristic) {
        // Allumer la LED
        digitalWrite(led, HIGH);
        ledOnTime = millis(); // Enregistrer le temps d'allumage de la LED
        ledState = true;      // Indiquer que la LED est allumée
    }
};

// Temps max d'allumage de la LED
const int ledHighMax = 1000;



// --------------------------------------------------------------------- //


void setup() {
  // Initialise les leds comme sortie
  pinMode(ledOn, OUTPUT);
  pinMode(led, OUTPUT);

  Serial.begin(115200);

  // Initialisation service Server
  BLEDevice::init("Capteur_1");
  pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Creations characteristiques beacons
  pCharacteristicbeaconT = pService->createCharacteristic(CHARACTERISTIC_BEACONT_UUID,
                                                          BLECharacteristic::PROPERTY_READ
                                                          );
  pCharacteristicbeacon1 = pService->createCharacteristic(CHARACTERISTIC_BEACON1_UUID,
                                                          BLECharacteristic::PROPERTY_READ
                                                          );
  pCharacteristicbeacon2 = pService->createCharacteristic(CHARACTERISTIC_BEACON2_UUID,
                                                          BLECharacteristic::PROPERTY_READ
                                                          );

  pCharacteristicbeaconT->setCallbacks(new MyBLECallbacks());  
  pCharacteristicbeacon1->setCallbacks(new MyBLECallbacks());
  pCharacteristicbeacon2->setCallbacks(new MyBLECallbacks());

// Lancement
  pService->start();
  pServer->getAdvertising()->start();
}


// Boucle principale
void loop() {
  // Allumer la LED
  digitalWrite(ledOn, HIGH);

  // Vérifier si une seconde s'est écoulée depuis l'allumage de la LED
  if (ledState && millis() - ledOnTime >= ledHighMax) {
      digitalWrite(led, LOW); // Éteindre la LED
      ledState = false; // Indiquer que la LED est éteinte
  }

  // Obtient le temps actuel
  currentMillis = millis();

  // Vérifie si l'intervalle est écoulé
  if (currentMillis - previousMillis >= interval) {
    // Enregistre le temps de cette exécution
    previousMillis = currentMillis;
    doBLEScans();
  }
}



// --------------------------------------------------------------------- //


void doBLEScans() {
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
      Serial.printf("RSSI %d : %d\n", i, allBeacon[i]);// 0 pour beacon test
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