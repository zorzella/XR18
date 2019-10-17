// ***************************************************************
// *        Behringer XR18 Arduino MIDI Remote Control           *
// *                   By Zorzella                               *
// *  I started this based on code By Vern Graner -- thank you!  *
// ***************************************************************

#include <Arduino.h>

#include <OSCMessage.h>  // For OSC support
#include <WiFi.h>        // For WiFi
#include <WiFiUdp.h>     // For WiFi

#include "networks.h"

// Setup button pins
int myButtons[] = {39, 34, 25, 26};  // define the pins for the buttons
int buttonCount = 4;                 // set the number of buttons in the loop

// Define program Variables
int buttonState[] = {0, 0, 0, 0};      // current state of the button
int lastButtonState[] = {0, 0, 0, 0};  // previous state of the button

// Setup LED pins
int myLeds[] = {32, 15, 33, 27};  // define the pins for the LEDs
int ledCount = 4;                 // set the number of LEDs in the loop

WiFiUDP wifiUdp;  // A UDP instance to let us send and receive packets over UDP

IPAddress xrIp{
    INADDR_NONE};  // IP of the XR18 in Comma Separated Octets, NOT dots!
const unsigned int XR_PORT =
    10024;  // remote port to receive OSC X-AIR is 10024, X32 is 10023

// If true, we will print extra debug information about WIFI
const bool DEBUG_WIFI = true;

const std::string M_XINFO = "/xinfo";
const std::string M_STATUS = "/status";
const std::string M_XREMOTE = "/xremote";

unsigned long sendOk = 0;
unsigned long sendError = 0;
unsigned long recOk = 0;
unsigned long recError = 0;

bool waitForConnection() {
  unsigned long timeoutAt = millis() + 4000;
  // TODO: timeout and do something (look at more networks?). Also
  // maybe there's a way to reset WiFi etc (I've seen it get stuck and
  // need to have the tires kicked)...
  while (millis() < timeoutAt) {
    wl_status_t status = WiFi.status();
    if (DEBUG_WIFI) {
      Serial.print(status);
    }
    // TODO: deal with other statuses
    // See https://www.arduino.cc/en/Reference/WiFiStatus
    if (status == WL_CONNECTED) {
      Serial.println();
      return true;
    } else if (status == WL_DISCONNECTED) {
      // Fairly normal for starters, we should start here
    } else if (status == WL_IDLE_STATUS) {
      // Fairly normal for starters, it will be in this status
      // while it tries to connect
    }
    delay(100);
    Serial.print(".");
  }
  Serial.println();
  return false;
}

void handleStatus(OSCMessage &msg) {
  const int len = 50;
  char buffer[len];
  memset(buffer, 0, len);

  if (msg.getString(1, buffer, len) == len) {
    Serial.println("FULL");
  }
  Serial.print("IP: ");
  Serial.println(buffer);
}

const int SIZE_OF_RECEIVE_BUFFER = 100;

void receiveOscIfAny(OSCMessage &msg) {
  int size = wifiUdp.parsePacket();

  if (size == 0) {
    return;
  }
  while (size--) {
    msg.fill(wifiUdp.read());
  }
}

bool receiveOsc(OSCMessage &msg) {
  unsigned long timeoutAt = millis() + 100;
  char buffer[SIZE_OF_RECEIVE_BUFFER];
  memset(buffer, 0, SIZE_OF_RECEIVE_BUFFER);
  while (millis() < timeoutAt) {
    receiveOscIfAny(msg);
    if (msg.size() > 0) {
      recOk++;
      return true;
    }
  }
  recError++;
  return false;
}

bool receiveOscWithAddress(OSCMessage &msg, const std::string &address) {
  unsigned long timeoutAt = millis() + 100;
  char buffer[SIZE_OF_RECEIVE_BUFFER];
  memset(buffer, 0, SIZE_OF_RECEIVE_BUFFER);
  while (millis() < timeoutAt) {
    receiveOscIfAny(msg);
    if (msg.size() > 0) {
      msg.getAddress(buffer);
      // TODO: if we receive more than, say, 3 messages, and none is the
      // reply to our UDP, we need to give up etc
      if (!address.compare(buffer)) {
        return true;
      }
      msg.empty();
    }
  }
  return false;
}

void printOsc(OSCMessage &msg) {
  char buffer[SIZE_OF_RECEIVE_BUFFER];
  memset(buffer, 0, SIZE_OF_RECEIVE_BUFFER);

  if (!msg.hasError()) {
    msg.getAddress(buffer);
    Serial.print("<<< [");
    Serial.print(recOk);
    Serial.print(",");
    Serial.print(recError);
    Serial.print("] ");
    Serial.print(buffer);

    for (int i = 0; i < msg.size(); i++) {
      Serial.print(" [");
      if (msg.isString(i)) {
        msg.getString(i, buffer);
        Serial.print(buffer);
      } else if (msg.isInt(i)) {
        Serial.print(msg.getInt(i));
      } else if (msg.isFloat(i)) {
        Serial.print(msg.getFloat(i));
      } else {
        Serial.print("type: ");
        Serial.print(msg.getType(i));
      }
      Serial.print("]");
    }
    Serial.println();
  } else {
    OSCErrorCode error = msg.getError();
    Serial.print("error: ");
    Serial.println(error);
  }
}

void receiveAndPrintOscIfAny() {
  OSCMessage msg;
  while (true) {
    receiveOscIfAny(msg);
    if (msg.size() == 0) {
      return;
    }
    printOsc(msg);
    msg.empty();
  }
}

bool sendUdp(const IPAddress &ip, OSCMessage &msg) {
  unsigned long timeoutAt = millis() + 100;
  while (millis() < timeoutAt) {
    if (!wifiUdp.beginPacket(ip, XR_PORT)) {
      sendError++;
      continue;
    }
    msg.send(wifiUdp);
    int endPacketResult = wifiUdp.endPacket();
    msg.empty();
    if (!endPacketResult) {
      sendError++;
      continue;
    }
    sendOk++;
    return true;
  }
  sendError++;
  return false;
}

bool send1(const IPAddress &ip, const std::string &mess) {
  OSCMessage msg(mess.c_str());
  Serial.print(">>> [");
  Serial.print(sendOk);
  Serial.print(",");
  Serial.print(sendError);
  Serial.print("] ");
  Serial.println(mess.c_str());
  return sendUdp(ip, msg);
}

bool send2(const IPAddress &ip, const std::string &one,
           const std::string &two) {
  Serial.print(">>> [");
  Serial.print(sendOk);
  Serial.print(",");
  Serial.print(sendError);
  Serial.print("] ");
  Serial.print(one.c_str());
  Serial.print(" ");
  Serial.println(two.c_str());

  OSCMessage msg(one.c_str());
  msg.add(two.c_str());
  return sendUdp(ip, msg);
}

void send3(const IPAddress &ip, const char *one, const char *two,
           const int &three
           // ,
           // const char* &four
) {
  Serial.print(">>> [");
  Serial.print(sendOk);
  Serial.print(",");
  Serial.print(sendError);
  Serial.print("] ");
  Serial.print(one);
  Serial.print(" ");
  Serial.print(two);
  Serial.print(" ");
  Serial.println(three);
  // Serial.print(" ");
  // Serial.println(four);

  OSCMessage msg(one);
  msg.add(two);
  msg.add(three);
  // msg.add(four);
  sendUdp(ip, msg);
}

bool discoverXrIp(IPAddress &result) {
  // TODO: handle XR_NAMES
  IPAddress broadcastIp = WiFi.broadcastIP();
  unsigned long timeoutAt = millis() + 1000;
  while (millis() < timeoutAt) {
    if (!send1(broadcastIp, M_STATUS)) {
      continue;
    }
    OSCMessage msg;
    bool receiveResult = receiveOscWithAddress(msg, M_STATUS);
    if (receiveResult) {
      char buffer[SIZE_OF_RECEIVE_BUFFER];
      memset(buffer, 0, SIZE_OF_RECEIVE_BUFFER);
      msg.getString(1, buffer);
      Serial.print("Remote IP: ");
      Serial.println(buffer);
      result.fromString(buffer);
      return true;
    } else {
      Serial.println("Retrying");
    }
  }
  return false;
}

bool connectThru(const std::string &ssid, const std::string &pass) {
  unsigned long start = millis();
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid.c_str());
  WiFi.begin(ssid.c_str(), pass.c_str());

  bool result = waitForConnection();
  int elapsed = millis() - start;
  Serial.print("Elapsed: ");
  Serial.println(elapsed);
  if (!result) {
    Serial.println("Failed to connect");
    return result;
  }

  Serial.println("");

  Serial.print("Connected to WiFi: ");
  Serial.println(ssid.c_str());

  Serial.println("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Broadcast IP: ");
  Serial.println(WiFi.broadcastIP());
  Serial.print("Gateway IP: ");
  Serial.println(WiFi.gatewayIP());
  Serial.println();
  Serial.println();

  return result;
}

void setup() {
  Serial.begin(115200);  // DEBUG window
  while (!Serial) {
    ;
  }

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
}

bool sendReceive(const std::vector<std::string> &ary, const std::string &msg) {
  for (int z = 0; z < ary.size(); z++) {
    Serial.println("vvvvvvv");
    receiveAndPrintOscIfAny();

    OSCMessage query;

    if (!send1(xrIp, ary[z])) {
      return false;
    }
    if (!receiveOsc(query)) {
      return false;
    }
    printOsc(query);
    query.empty();

    if (!send2(xrIp, ary[z], msg)) {
      return false;
    }
    receiveAndPrintOscIfAny();

    if (!send1(xrIp, ary[z])) {
      return false;
    }
    if (!receiveOsc(query)) {
      return false;
    }
    printOsc(query);
    query.empty();
    Serial.println("^^^^^^^");

    delay(2000);
  }
  return true;
}

const std::vector<std::string> CHANNELS_TO_TURN_ON_AND_OFF{
    "/ch/01/mix/on", "/ch/03/mix/on",
    //"/rtn/aux/mix/on",
};

const std::string CHANNEL_OFF = "OFF";  // i.e. Mute on
const std::string CHANNEL_ON = "ON";    // i.e. Mute off

const std::vector<std::string> CHANNEL_MSGS_TO_CHANGE_SEND_LEVEL{
    "/ch/01/mix/01/level",
    "/ch/03/mix/01/level",
};

const std::string LEVEL_ON = "0";
const std::string LEVEL_OFF = "-127";

void sendABunchOfMessages() {
  sendReceive(CHANNEL_MSGS_TO_CHANGE_SEND_LEVEL, LEVEL_ON);
  sendReceive(CHANNELS_TO_TURN_ON_AND_OFF, CHANNEL_OFF);
  sendReceive(CHANNEL_MSGS_TO_CHANGE_SEND_LEVEL, LEVEL_OFF);
  sendReceive(CHANNELS_TO_TURN_ON_AND_OFF, CHANNEL_ON);
}

bool tryToReconnectWifi() {
  bool result = false;
  // TODO: start with the last successful network
  for (int i = 0; i < XR_NETWORKS.size(); i++) {
    std::string ssid = XR_NETWORKS[i].ssid;
    std::string pass = XR_NETWORKS[i].password;
    bool connectResult = connectThru(ssid.c_str(), pass.c_str());
    if (connectResult) {
      bool discoverResult = discoverXrIp(xrIp);
      if (discoverResult) {
        return true;
      } else {
        result = discoverResult;
      }
    }
  }
  return result;
}

void loop() {
  // TODO: split XR unreachable from wifi down
  if (WiFi.status() != WL_CONNECTED || xrIp == INADDR_NONE) {
    Serial.println("wifi down or XR unreachable. Reconnecting.");
    if (tryToReconnectWifi()) {
      Serial.println("wifi reconnection failed. Will try again in 200ms.");
      delay(200);
      return;
    }
    Serial.println("wifi reconnection succeeded.");
  } else {
    Serial.println("wifi ok");
    sendABunchOfMessages();
  }
}  // End of main loop
