#include "bluetooth.h"

void BLUETOOTH::config(const char *nameServer,const char *service_UUID, const char *characteristic_UUID_TX, const char *characteristic_UUID_RX) {
    bleDevice->init(nameServer); // nome do dispositivo bluetooth
    server = BLEDevice::createServer(); //cria um BLE server
    //server->setCallbacks(new BLUETOOTH()); //seta o callback do server
    //server->setCallbacks(new MyServerCallbacks()); //seta o callback do server
    // Create the BLE Service
    service = server->createService(service_UUID);

    // Create a BLE Characteristic para envio de dados
    characteristicTX = service->createCharacteristic(
                      characteristic_UUID_TX,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
    characteristicTX->addDescriptor(new BLE2902());
    // Create a BLE Characteristic para recebimento de dados
    //BLECharacteristic *characteristic = service->createCharacteristic(
    characteristicRX = service->createCharacteristic(
                                         characteristic_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
}

void BLUETOOTH::send(String value){
    //seta o valor que a caracteristica notificará (enviar)
    characteristicTX->setValue(value.c_str());
    // Envia o valor para o smartphone
    characteristicTX->notify();
}

String BLUETOOTH::receive(){
     //std::string rxValue = characteristicRX->getValue();
     String rxValue = characteristicRX->getValue().c_str();
     if (rxValue.length() > 0) {
        return rxValue;
     } else return "";
}

void BLUETOOTH::start() {
  // Start the service
  service->start();
  // Start advertising (descoberta do ESP32)
  server->getAdvertising()->start();
}

void BLUETOOTH::stop() {
    bleDevice->deinit();
}

/*void ServerCallbacks::onConnect(BLEServer* pServer){
    bluetoothConnected = true;
}

void ServerCallbacks::onDisconnect(BLEServer* pServer){
    bluetoothConnected = false;
}*/

void BLUETOOTH::startBLE(){
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan(); //create new scan

    pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);  // less or equal setInterval value
}

void BLUETOOTH::scanBLE(){
    foundDevices = pBLEScan->start(scanTime, false);
}

void BLUETOOTH::clearBLE(){
    pBLEScan->clearResults();
}

bool BLUETOOTH::findNameBLE(String name){
    //BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
    scanBLE();
    int deviceCount = foundDevices.getCount();
    String deviceName;
    for (uint32_t i = 0; i < deviceCount; i++)
    {
        BLEAdvertisedDevice device = foundDevices.getDevice(i);
        //if(strcmp(device.getName().c_str(),name)==0){
        deviceName = String(device.getName().c_str());
        if(deviceName==name){
            clearBLE();
            return true;
        }
    }
    clearBLE();
    return false;
}

bool BLUETOOTH::findMacBLE(String mac){
        //BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
    scanBLE();
    int deviceCount = foundDevices.getCount();
    String deviceMac;
    for (uint32_t i = 0; i < deviceCount; i++)
    {
        BLEAdvertisedDevice device = foundDevices.getDevice(i);
        deviceMac = String(device.getAddress().toString().c_str());
        if(deviceMac==mac){
            clearBLE();
            return true;
        }
    }
    clearBLE();
    return false;
}
