// ***************************************************************
// *        Behringer XR18 Arduino MIDI Remote Control           *
// *                   By Zorzella                               *
// *  I started this based on code By Vern Graner -- thank you!  *
// ***************************************************************

#include "ZrDebug.h"

#include <Arduino.h>
#include <ArduinoTrace.h>
#include <OSCMessage.h>  // For OSC support
#include <WiFi.h>        // For WiFi
#include <WiFiUdp.h>     // For WiFi
#include <iostream>
#include <sstream>
#include <string>

#include "Test5110.h"
#include "ZrComm.h"
#include "ZrDisplay.h"
#include "ZrGlobal.h"
#include "ZrKeypad.h"
#include "ZrTestScript.h"
#include "heltecfac.h"
#include "myheltec.h"
#include "mykeypad.h"

enum Mode {
  XR,
  MY_KEYPAD,
  // MY_HELTEC,
  HELTEC_FAC,
  NOKIA_5110,
};

static Mode m_mode = XR;

static bool WAIT_FOR_BUTTON_33 = false;

static int BUTTON_33 = 33;

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

  switch (m_mode) {
    case XR:
      // Let it continue
      break;
    case MY_KEYPAD:
      setup_mykeypad();
      return;
    case HELTEC_FAC:
      setup_heltecfac();
      return;
    case NOKIA_5110:
      delay(1000);
      setup_5110();
      return;
    default:
      Serial.println("Unimplemented");
      return;
  }

  if (DEBUG_WIFI) {
    // Give a second before doing anything, so the terminal is active
    delay(1000);
  }

  // Setp button pins as input pullup
  for (int i = 0; i < buttonCount; i++) {
    pinMode(myButtons[i], INPUT_PULLUP);
  }

  // Setp LEDs as output
  for (int i = 0; i < ledCount; i++) {
    pinMode(myLeds[i], OUTPUT);
  }

  pinMode(BUTTON_33, INPUT_PULLUP);

  setupDisplay();
  ZrNavigation::instance().init();
}

void loop() {
  switch (m_mode) {
    case XR:
      // Let it continue
      break;
    case MY_KEYPAD:
      loop_mykeypad();
      return;
    case HELTEC_FAC:
      loop_heltecfac();
      return;
    case NOKIA_5110:
      loop_5110();
      return;
    default:
      Serial.println("Unimplemented");
      return;
  }

  if (WAIT_FOR_BUTTON_33) {
    Serial.print("Waiting for button click.");
    while (digitalRead(BUTTON_33) == HIGH) {
      Serial.print(".");
      delay(1000);
    }
    Serial.println();
  }

  if (WiFi.status() != WL_CONNECTED || xrIp() == INADDR_NONE) {
    Serial.println("Wifi down or XR unreachable. Reconnecting.");
    if (!tryToReconnectWifi()) {
      Serial.println("Wifi reconnection failed. Will try again in 200ms.");
      delay(200);
      return;
    }
    Serial.println("Wifi reconnection succeeded.");
  }

  // Serial.println("Wifi ok");
  // sendABunchOfMessages();
  // runTestScript();

  ZrNavigation navigation = ZrNavigation::instance();

  navigation.currentFunction().triggerCacheUpdateIfNeeded();

  char currentKey = zrKeypad.getKey();
  if (currentKey) {
    switch (currentKey) {
      case KEY_UP:
        navigation.goUp();
        break;
      case KEY_DOWN:
        navigation.goDown();
        break;
      case KEY_LEFT:
        navigation.goLeft();
        break;
      case KEY_RIGHT:
        navigation.goRight();
        break;
      case KEY_PLUS:
        navigation.clickPlus();
        break;
      case KEY_MINUS:
        navigation.clickMinus();
        break;
    }
  }

  OSCMessage msg;
  receiveOscIfAny(msg);

  refreshDisplay();
}  // End of main loop
