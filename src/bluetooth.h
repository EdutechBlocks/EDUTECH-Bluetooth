#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "Arduino.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEService.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <BLEAdvertisedDevice.h>

class BLUETOOTH{
    public:
        void config(const char *nameServer,const char *service_UUID, const char *characteristic_UUID_TX, const char *characteristic_UUID_RX);
        void start();
        void send(String value);
        String receive();
        void stop();
        void startBLE();
        bool findNameBLE(String name);
        bool findMacBLE(String mac);
    private:
        BLEDevice *bleDevice;
        BLEServer *server;
        BLEService *service;
        BLECharacteristic *characteristicTX, *characteristicRX;
        int scanTime = 5; //In seconds
        BLEScan* pBLEScan;
        BLEScanResults foundDevices;
        void scanBLE();
        void clearBLE();
};
#endif
