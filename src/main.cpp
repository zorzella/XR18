// ***************************************************************
// *        Behringer XR18 Arduino MIDI Remote Control           *
// *                   By Zorzella                               *
// *  I started this based on code By Vern Graner -- thank you!  *
// ***************************************************************

#include <Arduino.h>
#include <ArduinoTrace.h>
#include <OSCMessage.h>  // For OSC support
#include <WiFi.h>        // For WiFi
#include <WiFiUdp.h>     // For WiFi
#include <iostream>
#include <sstream>
#include <string>

// #include "Test5110.h"
// #include <esp32-hal-touch.h>
// #include "esp32-hal.h"

#include "ZrComm.h"
#include "ZrGlobal.h"
#include "ZrTestScript.h"

#define BRACELET_ON GPIO_NUM_27
#define TOUCH GPIO_NUM_32

static const int touchPin = 32;

// Setup button pins
int myButtons[] = {};  // 39, 34, 25, 26};  // define the pins for the buttons
int buttonCount = 0;   // set the number of buttons in the loop

// Define program Variables
int buttonState[] = {0, 0, 0, 0};      // current state of the button
int lastButtonState[] = {0, 0, 0, 0};  // previous state of the button

// Setup LED pins
int myLeds[] = {};  // 32, 15, 33, 27};  // define the pins for the LEDs
int ledCount = 0;   // set the number of LEDs in the loop

const std::string M_XINFO = "/xinfo";
const std::string M_XREMOTE = "/xremote";

void setup() {

  Serial.begin(115200);  // DEBUG window
  while (!Serial) {
    ;
  }

  // if (true) {
  //   delay(1000);
  //   setup5110();
  // }

  if (DEBUG_WIFI) {
    // Give a second before doing anything, so the terminal is active
    delay(1000);
  }

  // Setp pin mode for buttons
  for (int i = 0; i < buttonCount; i++) {
    pinMode(myButtons[i],
            INPUT_PULLUP);  // initialize the button pin as a input
  }

  // Setp pin mode for LEDs
  for (int i = 0; i < ledCount; i++) {
    pinMode(myLeds[i], OUTPUT);  // initialize the LED as an output
  }

  pinMode(33, INPUT_PULLUP);

  navigation().init();
}

void loop() {
  Serial.print("Waiting click button.");
  while (digitalRead(33) == HIGH) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();

  if (WiFi.status() != WL_CONNECTED || xrIp() == INADDR_NONE) {
    Serial.println("Wifi down or XR unreachable. Reconnecting.");
    if (!tryToReconnectWifi()) {
      Serial.println("Wifi reconnection failed. Will try again in 200ms.");
      delay(200);
      return;
    }
    Serial.println("Wifi reconnection succeeded.");
  }

  Serial.println("Wifi ok");
  // sendABunchOfMessages();
  runTestScript();
}  // End of main loop
