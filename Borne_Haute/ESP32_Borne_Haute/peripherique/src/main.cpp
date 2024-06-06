#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include "BLEManager.h"
#include "MyServerCallbacks.h"

//const char *SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
//const char *CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";


BLEManager bleManager;
//const int ledRouge = 20; // GPIO20 for the LED
//const int ledVerte = 8; // GPIO08 for the LED

void setup()
{
  //pinMode(ledVerte, OUTPUT);
  //pinMode(ledRouge, OUTPUT);
  bleManager.init();
}

void loop()
{
  bleManager.isConnected();

}

