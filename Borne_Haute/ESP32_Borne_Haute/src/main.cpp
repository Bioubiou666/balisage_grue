#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include "BLEManager.h"
#include "MyServerCallbacks.h"

BLEManager bleManager;



void setup()
{
  bleManager.init();
}


void loop()
{
  bleManager.Main();
}

