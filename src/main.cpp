#include <Arduino.h>

#define LED_PIN 13
#define BUTTON_PIN 26

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop()
{
  int v = digitalRead(BUTTON_PIN);
  if (v == LOW)
  {
    digitalWrite(LED_PIN, HIGH);
  }
  else
  {
    digitalWrite(LED_PIN, LOW);
  }
}