#ifndef MYSERVERCALLBACKS_H
#define MYSERVERCALLBACKS_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// Predéclaration de la classe BLEManager
class BLEManager;

class MyServerCallbacks : public BLEServerCallbacks
{
private:
  BLEManager* bleManagerPtr;

public:
  MyServerCallbacks(BLEManager *manager) : bleManagerPtr(manager) {}

  void onConnect(BLEServer *pServer);

  void onDisconnect(BLEServer *pServer);
};

#endif
