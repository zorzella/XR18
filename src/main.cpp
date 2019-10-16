// ***************************************************************
// *        Behringer XR18 Arduino MIDI Remote Control           *
// *                   By Zorzella                               *
// *  I started this based on code By Vern Graner -- thank you!  *
// ***************************************************************

#include <Arduino.h>

#include <OSCMessage.h>  // For OSC support
#include <WiFi.h>        // For WiFi
#include <WiFiUdp.h>     // For WiFi

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

IPAddress xrIp;  // IP of the XR18 in Comma Separated Octets, NOT dots!
const unsigned int XR_PORT =
    10024;  // remote port to receive OSC X-AIR is 10024, X32 is 10023

// If true, we will print extra debug information about WIFI
const bool DEBUG_WIFI = true;

const char *M_XINFO = "/xinfo";
const char *M_STATUS = "/status";
const char *M_XREMOTE = "/xremote";

int waitForConnection() {
  unsigned long timeoutAt = millis() + 2000;
  // TODO: timeout and do something (look at more networks?). Also
  // maybe there's a way to reset WiFi etc (I've seen it get stuck and
  // need to have the tires kicked)...
  while (millis() < timeoutAt) {
    wl_status_t status = WiFi.status();
    if (DEBUG_WIFI) {
      Serial.print("[");
      Serial.print(status);
      Serial.print("]");
    }
    // TODO: deal with other statuses
    // See https://www.arduino.cc/en/Reference/WiFiStatus
    if (status == WL_CONNECTED) {
      Serial.println();
      return 0;
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
  return 1;
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

// TODO: this needs to timeout etc
void receiveOsc(OSCMessage &msg) {
  char buffer[SIZE_OF_RECEIVE_BUFFER];
  memset(buffer, 0, SIZE_OF_RECEIVE_BUFFER);
  while (true) {
    receiveOscIfAny(msg);
    if (msg.size() > 0) {
      return;
    }
  }
}

// TODO: this needs to timeout etc
void receiveOscWithAddress(OSCMessage &msg, const std::string &address) {
  char buffer[SIZE_OF_RECEIVE_BUFFER];
  memset(buffer, 0, SIZE_OF_RECEIVE_BUFFER);
  while (true) {
    receiveOscIfAny(msg);
    if (msg.size() > 0) {
      msg.getAddress(buffer);
      // TODO: if we receive more than, say, 3 messages, and none is the
      // reply to our UDP, we need to give up etc
      if (!address.compare(buffer)) {
        return;
      }
      msg.empty();
    }
  }
}

void printOsc(OSCMessage &msg) {
  char buffer[SIZE_OF_RECEIVE_BUFFER];
  memset(buffer, 0, SIZE_OF_RECEIVE_BUFFER);

  if (!msg.hasError()) {
    msg.getAddress(buffer);
    Serial.print("<<< ");
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

void sendUdp(const IPAddress &ip, OSCMessage &msg) {
  // TODO: check result of beginPacket
  wifiUdp.beginPacket(ip, XR_PORT);
  msg.send(wifiUdp);
  // TODO: check result of endPacket
  wifiUdp.endPacket();
  msg.empty();
}

void send1(const IPAddress &ip, const char *&mess) {
  OSCMessage msg(mess);
  Serial.print(">>> ");
  Serial.println(mess);
  sendUdp(ip, msg);
}

void send2(const IPAddress &ip, const char *&one, const char *&two) {
  Serial.print(">>> ");
  Serial.print(one);
  Serial.print(" ");
  Serial.println(two);

  OSCMessage msg(one);
  msg.add(two);
  sendUdp(ip, msg);
}

void send3(const IPAddress &ip, const char *one, const char *two,
           const int &three
           // ,
           // const char* &four
) {
  Serial.print(">>> ");
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

IPAddress discoverXrIp() {
  IPAddress broadcastIp = WiFi.broadcastIP();
  send1(broadcastIp, M_STATUS);
  OSCMessage msg;
  receiveOscWithAddress(msg, M_STATUS);

  char buffer[SIZE_OF_RECEIVE_BUFFER];
  memset(buffer, 0, SIZE_OF_RECEIVE_BUFFER);
  msg.getString(1, buffer);
  Serial.print("Remote IP: ");
  Serial.println(buffer);
  IPAddress result;
  result.fromString(buffer);

  return result;
}

int connectThru(const char *ssid, const char *pass) {
  unsigned long start = millis();
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  int result = waitForConnection();
  int elapsed = millis() - start;
  Serial.print("Elapsed: ");
  Serial.println(elapsed);
  if (result) {
    Serial.println("Failed to connect");
    return result;
  }

  Serial.println("");

  Serial.print("Connected to WiFi: ");
  Serial.println(ssid);

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
  // TODO: understand better
  int counter = 0;
  Serial.begin(115200);  // DEBUG window
  while (!Serial) {
    counter++;
  }

  Serial.print("Counter before: ");
  Serial.println(counter);

  if (DEBUG_WIFI) {
    // Give a second before doing anything, so the terminal is active
    delay(1000);
  }

  Serial.print("Counter: ");
  Serial.println(counter);

  // Setp pin mode for buttons
  for (int i = 0; i < buttonCount; i++) {
    pinMode(myButtons[i], INPUT);  // initialize the button pin as a input
  }

  // Setp pin mode for LEDs
  for (int i = 0; i < ledCount; i++) {
    pinMode(myLeds[i], OUTPUT);  // initialize the LED as an output
  }
}

void sendReceive(std::vector<const char *> ary, const char *&msg) {
  for (int z = 0; z < ary.size(); z++) {
    Serial.println("vvvvvvv");
    receiveAndPrintOscIfAny();

    OSCMessage query;

    send1(xrIp, ary[z]);
    receiveOsc(query);
    printOsc(query);
    query.empty();

    send2(xrIp, ary[z], msg);
    receiveAndPrintOscIfAny();

    send1(xrIp, ary[z]);
    receiveOsc(query);
    printOsc(query);
    query.empty();
    Serial.println("^^^^^^^");

    delay(2000);
  }
}

std::vector<const char *> CHANNELS_TO_TURN_ON_AND_OFF{
    "/ch/01/mix/on", "/ch/03/mix/on",
    //"/rtn/aux/mix/on",
};

const char *CHANNEL_OFF = "OFF";  // i.e. Mute on
const char *CHANNEL_ON = "ON";    // i.e. Mute off

std::vector<const char *> CHANNEL_MSGS_TO_CHANGE_SEND_LEVEL{
    "/ch/01/mix/01/level",
    "/ch/03/mix/01/level",
};

const char *LEVEL_ON = "0";
const char *LEVEL_OFF = "-127";

void sendABunchOfMessages() {
  sendReceive(CHANNEL_MSGS_TO_CHANGE_SEND_LEVEL, LEVEL_ON);
  sendReceive(CHANNELS_TO_TURN_ON_AND_OFF, CHANNEL_OFF);
  sendReceive(CHANNEL_MSGS_TO_CHANGE_SEND_LEVEL, LEVEL_OFF);
  sendReceive(CHANNELS_TO_TURN_ON_AND_OFF, CHANNEL_ON);
}

void loop() {
    // TODO: move to "loop"?
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("wifi down. Reconnecting.");
    // TODO: handle multiple networks
    connectThru(aSSID, aSSID_PASS);
    xrIp = discoverXrIp();
    delay(200);
    return;
  } else {
    Serial.println("wifi ok");
  }
  sendABunchOfMessages();
}  // End of main loop
