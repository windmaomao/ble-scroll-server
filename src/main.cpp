#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>

#define LED_PIN 13
#define BUTTON_PIN 26
#define SERVICE_UUID BLEUUID("FF10")
#define CHARACTERISTIC_UUID BLEUUID("FF11")
#define DESCRIPTOR_UUID BLEUUID("State")

BLECharacteristic *pCharacteristic;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  BLEDevice::init("Scroll Server");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic->setValue("Hello World");
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
  int v = digitalRead(BUTTON_PIN);
  if (v == LOW)
  {
    digitalWrite(LED_PIN, HIGH);
    pCharacteristic->setValue("ON");
  }
  else
  {
    digitalWrite(LED_PIN, LOW);
    pCharacteristic->setValue("OFF");
  }
}