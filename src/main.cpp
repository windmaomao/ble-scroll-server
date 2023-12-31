#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>

#define LED_PIN 13
#define BUTTON_PIN 26

#define BLE_SERVER_NAME "Scroll Server"
#define SERVICE_UUID BLEUUID("FF10")
#define CHARACTERISTIC_UUID BLEUUID("FF11")

bool deviceConnected = false;
BLECharacteristic *pCharacteristic;
BLEDescriptor *pDescriptor;

class ServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;

    BLEAdvertising *pAdvertising;
    pAdvertising = pServer->getAdvertising();
    pAdvertising->start();
  }
};

class CharacteristicCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0)
    {
      Serial.println("*********");
      Serial.print("Received Value: ");
      for (int i = 0; i < rxValue.length(); i++)
      {
        Serial.print(rxValue[i], HEX);
        if (rxValue[i] == '0')
        {
          digitalWrite(LED_PIN, LOW);
        }
        else
        {
          digitalWrite(LED_PIN, HIGH);
        }
      }
      Serial.println();
      Serial.println("*********"); }
  }
};

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting ");

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  BLEDevice::init(BLE_SERVER_NAME);
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE);
  pCharacteristic->setCallbacks(new CharacteristicCallbacks());
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
  // u_int8_t v = digitalRead(BUTTON_PIN);
  // if (v == LOW)
  // {
  //   pCharacteristic->setValue(&v, 1);
  //   pCharacteristic->notify();
  // }
  // else
  // {
  //   pCharacteristic->setValue(&v, 1);
  //   pCharacteristic->notify();
  // }

  // if (deviceConnected)
  // {
  //   digitalWrite(LED_PIN, HIGH);
  // }
  // else
  // {
  //   digitalWrite(LED_PIN, LOW);
  // }
}