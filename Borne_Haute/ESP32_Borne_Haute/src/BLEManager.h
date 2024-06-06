#ifndef BLEMANAGER_H
#define BLEMANAGER_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

extern bool Error;

// Predéclaration de la classe MyServerCallbacks
class MyServerCallbacks;
class MyCharacteristicCallbacks;

class BLEManager {
private:
  BLEServer *pServer;
  BLECharacteristic *pCharacteristic1;
  BLECharacteristic *pCharacteristic2;
  bool deviceConnected;
  bool oldDeviceConnected;
  bool wasDisconnected = false;
  int conn;

public:
  BLEManager() : deviceConnected(false), oldDeviceConnected(false) {}

  void init();
  void setDeviceConnected(bool connected);
  bool testBatt();
  void alarmState(bool state);
  int isConnected();
  void Main();
};

#endif