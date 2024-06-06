#include "BLEManager.h"
#include "MyServerCallbacks.h"


void MyServerCallbacks::onConnect(BLEServer *pServer) {
  bleManagerPtr->setDeviceConnected(true);
  Serial.println("callback connected");
}

void MyServerCallbacks::onDisconnect(BLEServer *pServer) {
  Error = false;
  bleManagerPtr->setDeviceConnected(false);
  Serial.println("callback disconnected");
}

void MyCharacteristicCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
  std::string value = pCharacteristic->getValue();
  Serial.println(value.c_str());

  if (value == "Err") {
    Error = true;
  }
  else {
    Error = false;
  }
}