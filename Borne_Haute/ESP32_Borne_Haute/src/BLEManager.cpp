#include <Arduino.h>
#include "BLEManager.h"
#include "MyServerCallbacks.h"

using namespace std;

const char *SERVICE_UUID = "1802";
const char *CHARACTERISTIC_UUID1 = "2A3F";
const char *CHARACTERISTIC_UUID2 = "8A19";
const int PinAlarm = 20;    // GPIO20 for the alarm
const int ledVerte = 8;     // GPIO8 for the LED connected
const int ledJaune = 9;     // GPIO9 for allimentation
bool Error = false;

uint32_t Vbatt;
float Vbattf;

#include <BLEServer.h>


void BLEManager::init() {

    pinMode(ledVerte, OUTPUT);
    pinMode(PinAlarm, OUTPUT);
    pinMode(ledJaune, OUTPUT);
    digitalWrite(ledJaune, HIGH);
    pinMode(A0, INPUT);       // GPIOA0 for level battery

    Serial.begin(115200);
    Serial.println("Initialisation BLE...");

    BLEDevice::init("ESP32 Central");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks(this));
    

    BLEService *pService = pServer->createService(SERVICE_UUID);
    pCharacteristic1 = pService->createCharacteristic(
                                            CHARACTERISTIC_UUID1,
                                            BLECharacteristic::PROPERTY_READ |
                                            BLECharacteristic::PROPERTY_WRITE
                                        );
    pCharacteristic2 = pService->createCharacteristic(
                                            CHARACTERISTIC_UUID2,
                                            BLECharacteristic::PROPERTY_READ |
                                            BLECharacteristic::PROPERTY_NOTIFY
                                        );


    pCharacteristic1->setValue("Hello");
    pCharacteristic2->setValue("OK");
    pCharacteristic1->setCallbacks(new MyCharacteristicCallbacks());

    pService->start();
    // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
}

void BLEManager::setDeviceConnected(bool connected) {
    deviceConnected = connected;

    if (connected) {
        wasDisconnected = false;
    }
}

bool BLEManager::testBatt() {
    Vbatt = 0;

    for(int i = 0; i < 16; i++) {
        Vbatt = Vbatt + analogReadMilliVolts(A0); // ADC with correction   
    }
    Vbattf = 2 * Vbatt / 16 / 1000.0;     // attenuation ratio 1/2, mV --> V

    Serial.println(Vbattf, 3);

    if (Vbattf > 3.4) {
        Serial.println("Up to 2.7V!");
        return 1;
    }
    else {
        Serial.println("Down to 2.7V!");
        return 0;
    }
}

void BLEManager::alarmState(bool state) {
    if (state) {
        digitalWrite(PinAlarm, HIGH);
        digitalWrite(ledVerte, LOW);
    }
    else {
        digitalWrite(PinAlarm, LOW);
        digitalWrite(ledVerte, HIGH);
    }
}

int BLEManager::isConnected() {
    bool result;
    if (deviceConnected) {      // connecté
        Serial.println("connected");
        result = 1;
    }

    else if (!deviceConnected && oldDeviceConnected) {      // deconnection -> restart server ble
        if (!wasDisconnected) {
            Serial.println("disconnected");
            pServer->startAdvertising();
            Serial.println("Advertising restarted");
            wasDisconnected = true;
            result = 0;
        }
    }

    else if (!deviceConnected && wasDisconnected) {     // déconnected
        Serial.println("disconnected");
        result = 0;
    }

    else {      // initialisation -> never connected
        Serial.println("not connected");
        result = -1;
    }
    
    oldDeviceConnected = deviceConnected;
    return result;
}

void BLEManager::Main() {
    if (testBatt()) {

        pCharacteristic2->setValue("OK");
        conn = isConnected();
        if (conn == 1) {

            if (!Error) {

                alarmState(false);
            }

            else {
                alarmState(true);
            }
        }
        else if (conn == 0) {
            alarmState(true);
        }
    }

    else {
        pCharacteristic2->setValue("NOK");
        alarmState(true);
    }

    delay(500);
}