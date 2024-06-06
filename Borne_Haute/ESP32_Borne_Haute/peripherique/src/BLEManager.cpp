#include <Arduino.h>
#include "BLEManager.h"
#include "MyServerCallbacks.h"

const char *SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
const char *CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";
const int ledRouge = 20; // GPIO20 for the LED
const int ledVerte = 8; // GPIO8 for the LED
const int ledJaune = 9;

void BLEManager::init()
  {


    pinMode(ledVerte, OUTPUT);
    pinMode(ledRouge, OUTPUT);
    pinMode(ledJaune, OUTPUT);
    pinMode(A0, INPUT);       // ADC

    Serial.begin(115200);
    Serial.println("Initialisation BLE...");

    BLEDevice::init("ESP32 Central");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks(this));
    

    BLEService *pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ);
    pService->start();

    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->start();
  }

  void BLEManager::setDeviceConnected(bool connected)
  {
    deviceConnected = connected;
     if (connected) {
        wasDisconnected = false;
    }
  }

  void BLEManager::isConnected()
  {
    if (deviceConnected) {
        Serial.println("connected");
        digitalWrite(ledRouge, LOW);
        digitalWrite(ledVerte, HIGH);
        
    }

     else if (!deviceConnected && oldDeviceConnected) {
        if (!wasDisconnected) {
            Serial.println("disconnected");
            pServer->startAdvertising();
            Serial.println("Advertising restarted");
            digitalWrite(ledRouge, HIGH);
            digitalWrite(ledVerte, LOW);
            wasDisconnected = true;
        }
    }

     else if (!deviceConnected && wasDisconnected) {
        // Remain in disconnected state
        Serial.println("disconnected");
        digitalWrite(ledRouge, HIGH);
        digitalWrite(ledVerte, LOW);
    }

     else {
        Serial.println("not connected");
        digitalWrite(ledRouge, LOW);
    }
    /*
    uint32_t Vbatt = 0;
    for(int i = 0; i < 16; i++) {
        Vbatt = Vbatt + analogReadMilliVolts(A0); // ADC with correction   
    }
    float Vbattf = 2 * Vbatt / 16 / 1000.0;     // attenuation ratio 1/2, mV --> V
    Serial.println(Vbattf, 3);
    delay(1000);

    if (Vbattf < 2.7) {
        Serial.println("Battery voltage is below 2.7V! Turning on warning LED.");
        digitalWrite(ledJaune, HIGH); // Assuming ledWarning is the pin for the warning LED
    } else {
        digitalWrite(ledJaune, LOW);
    }

    delay(1000);
*/

        oldDeviceConnected = deviceConnected;
        delay(500);
    }