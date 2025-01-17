//  FARM DATA RELAY SYSTEM
//
//  TIPPING BUCKET RAINFALL SENSOR MODULE
//
//  Developed by Timm Bogner (bogner1@gmail.com) for Sola Gratia Farm in Urbana, Illinois, USA.
//  Each reading is assigned a two-byte identifier along with a one-byte sensor type
//
#define REED_PIN 2

#include "fdrs_sensor.h"
unsigned int theCount = 0;
unsigned long lastTrigger = 0;
boolean clicked = false;

// Checks if motion was detected, sets LED HIGH and starts a timer
ICACHE_RAM_ATTR void detectsMovement() {
  clicked = true;
  lastTrigger = millis();
}

void setup() {
beginFDRS();
  pinMode(REED_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(REED_PIN), detectsMovement, FALLING);
}

void loop() {
  if (clicked && millis() - lastTrigger > 100) {
    theCount++;
    Serial.print("CLICK.");
    Serial.println(theCount);
    clicked = false;
    loadFDRS(theCount, RAINFALL_T);
    sendFDRS();
  }

}
