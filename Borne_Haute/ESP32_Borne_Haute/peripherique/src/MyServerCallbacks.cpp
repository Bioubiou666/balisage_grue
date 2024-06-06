#include "BLEManager.h"
#include "MyServerCallbacks.h"



void MyServerCallbacks::onConnect(BLEServer *pServer)
  {
    bleManagerPtr->setDeviceConnected(true);
    Serial.println("callback connected");
  }

  void MyServerCallbacks::onDisconnect(BLEServer *pServer)
  {
    bleManagerPtr->setDeviceConnected(false);
    Serial.println("callback disconnected");
  }