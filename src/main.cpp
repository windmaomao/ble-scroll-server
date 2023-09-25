#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>

#define LED_PIN 13
#define BUTTON_PIN 26

#define BLE_SERVER_NAME "Scroll Server"
#define SERVICE_UUID BLEUUID("FF10")
#define CHARACTERISTIC_UUID BLEUUID("FF11")

BLECharacteristic *pCharacteristic;
BLEDescriptor *pDescriptor;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  BLEDevice::init(BLE_SERVER_NAME);
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE);
  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
}

void loop()
{
  u_int8_t v = digitalRead(BUTTON_PIN);
  if (v == LOW)
  {
    digitalWrite(LED_PIN, HIGH);
    pCharacteristic->setValue(&v, 1);
    pCharacteristic->notify();
  }
  else
  {
    digitalWrite(LED_PIN, LOW);
    pCharacteristic->setValue(&v, 1);
    pCharacteristic->notify();
  }
}