#ifndef BLEMANAGER_H
#define BLEMANAGER_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// Predéclaration de la classe MyServerCallbacks
class MyServerCallbacks;

class BLEManager
{
private:
  BLEServer *pServer;
  BLECharacteristic *pCharacteristic;
  bool deviceConnected;
  bool oldDeviceConnected;
  bool wasDisconnected = false;

public:
  BLEManager() : deviceConnected(false), oldDeviceConnected(false) {}

  void init();
  void setDeviceConnected(bool connected);
  void isConnected();
};

#endif