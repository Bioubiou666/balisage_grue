#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

// Declarations BLE
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLEAdvertisedDevice.h>


// UUID service et caractéristiques
#define DEVICE_NAME                 "Capteur_1"
#define SERVICE_UUID                "4fafc201-1fb5-459e-8fcc-c5c9c33191a1"
#define CHARACTERISTIC_BEACONT_UUID "beb5483e-36e1-4688-b7f5-ea07361b26b1"
#define CHARACTERISTIC_BEACON1_UUID "beb5483e-36e1-4688-b7f5-ea07361b26c1"
#define CHARACTERISTIC_BEACON2_UUID "beb5483e-36e1-4688-b7f5-ea07361b26d1"

class Capteur {
public:
    Capteur();  // Constructeur
    void setup();
    void loop();

private:
    BLEServer *pServer;
    BLEScan *pBLEScan;
    BLECharacteristic *pCharacteristicbeaconT;
    BLECharacteristic *pCharacteristicbeacon1;
    BLECharacteristic *pCharacteristicbeacon2;

    const char* allMacAdress[3] = {"84:fc:e6:00:83:ae", "34:85:18:26:eb:32", "fb:40:6b:4b:60:1a"};
    int countMacAdress = sizeof(allMacAdress) / sizeof(allMacAdress[0]);
    int allBeacon[3] = {-110, -110, -110};

    unsigned long previousMillis = 0;
    unsigned long currentMillis;
    const long interval = 1000;
    const int scanTime = 1;

    int ledOn = 2;
    int ledRead = 3;
    unsigned long ledOnTime = 0;
    bool ledState = false;
    const int ledHighMax = 1000;

    void doBLEScans();
    void handleLED();
    void startAdvertising();

    friend class MyBLECallbacks;
    friend class MyServerCallbacks;
};

class MyBLECallbacks : public BLECharacteristicCallbacks {
public:
    MyBLECallbacks(Capteur& capteur);
    void onRead(BLECharacteristic *pCharacteristic);

private:
    Capteur& capteur;
};

class MyServerCallbacks : public BLEServerCallbacks {
public:
    MyServerCallbacks(Capteur& capteur);
    void onConnect(BLEServer* pServer);
    void onDisconnect(BLEServer* pServer);

private:
    Capteur& capteur;
};

#endif // MAIN_H
