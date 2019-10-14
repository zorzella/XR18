// ***************************************************************
// *        Behringer XR18 Arduino MIDI Remote Control           *
// *                   By Zorzella                               *
// *  I started this based on code By Vern Graner -- thank you!  *
// ***************************************************************

#include <Arduino.h>

#include <WiFi.h>             // For WiFi
#include <WiFiUdp.h>          // For WiFi
#include <OSCMessage.h>       // For OSC support

// Setup button pins
int myButtons[] = {39,34,25,26};    // define the pins for the buttons
int buttonCount = 4;                // set the number of buttons in the loop

// Define program Variables
int buttonState[] = {0,0,0,0};      // current state of the button
int lastButtonState[] = {0,0,0,0};  // previous state of the button

// Setup LED pins
int myLeds[]={32,15,33,27};        // define the pins for the LEDs
int ledCount=4;                    // set the number of LEDs in the loop

// // WiFi setup
// char ssid[] = aSSID;                     // your network SSID (name)
// char pass[] = aSSID_PASS;                     // your network password

// TODO: local var?
WiFiUDP wifiUdp;                                // A UDP instance to let us send and receive packets over UDP

const IPAddress XR_IP(192,168,86,80);       // IP of the XR18 in Comma Separated Octets, NOT dots!
const unsigned int XR_PORT = 10024;         // remote port to receive OSC X-AIR is 10024, X32 is 10023

// If true, we will print extra debug information about WIFI
const bool DEBUG_WIFI = true;

const char* M_INFO = "/info";
const char* M_STATUS = "/status";
const char* M_XREMOTE = "/xremote";

void waitForConnection() {
    while (true) {
      wl_status_t status = WiFi.status();
        Serial.print("[");
        Serial.print(status);
        Serial.print("]");
      if (status == WL_CONNECTED) {
        return;
      }
      delay(100);
      Serial.print(".");
    }
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

void receive() {

  OSCMessage msg;
  
  char buffer[SIZE_OF_RECEIVE_BUFFER];
  memset(buffer, 0, SIZE_OF_RECEIVE_BUFFER);

  int size = wifiUdp.parsePacket();

  if (size > 0) {
    while (size--) {
      msg.fill(wifiUdp.read());
    }
    if (!msg.hasError()) {

     if (msg.dispatch("/status", handleStatus)) {
     }

     msg.getAddress(buffer);
     Serial.print("<<< ");
     Serial.print(buffer);

     for (int i = 0; i < msg.size(); i++) {
       msg.getString(i, buffer);
       Serial.print(" [");
       Serial.print(buffer);
       Serial.print("]");
     }
     Serial.println();
    } else {
      OSCErrorCode error = msg.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
}

void sendUdp(OSCMessage &msg) {
  wifiUdp.beginPacket(XR_IP, XR_PORT);
  msg.send(wifiUdp);
  wifiUdp.endPacket();
  // TODO: understand what this does
  //msg.empty();
}

void send(const char* mess) {
  Serial.print(">>> ");
  Serial.print(mess);

  OSCMessage msg(mess);
  sendUdp(msg);
}

void send(const char* &one, const char* &two) {
  Serial.print(">>> ");
  Serial.print(one);
  Serial.print(" ");
  Serial.println(two);

  OSCMessage msg(one);
  msg.add(two);
  sendUdp(msg);
}

void connectThru(const char* ssid, const char* pass) {
 
    // TODO: do IP discovery by sending to .255 (Broadcast IP)
    // TODO: handle multiple networks
    // Connect to WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");     
    Serial.println(ssid);               
    WiFi.begin(ssid, pass);

    waitForConnection();

    Serial.println("");

    Serial.println("WiFi connected");

    Serial.println("Local IP: ");
    Serial.println(WiFi.localIP());
    Serial.println("Starting UDP");

    // TODO: understand what this does    
    // Udp.begin(XR_PORT);

    Serial.print("Broadcast IP: ");
    Serial.println(WiFi.broadcastIP());
    Serial.print("Network ID: ");
    Serial.println(WiFi.networkID());
    Serial.print("Gateway IP: ");
    Serial.println(WiFi.gatewayIP());
    Serial.println();
    Serial.println();
}

void setup() {
    Serial.begin(115200); // DEBUG window

    if (DEBUG_WIFI) {
      // Give a second before doing anything, so the terminal is active
      delay(1000);
    }

    //Setp pin mode for buttons
    for (int i=0;i<buttonCount; i++) { 
      pinMode(myButtons[i], INPUT); // initialize the button pin as a input
    }

    //Setp pin mode for LEDs
    for (int i=0; i<ledCount; i++) {
      pinMode(myLeds[i], OUTPUT);   // initialize the LED as an output
    }

    // TODO: move to "loop"?
    connectThru(aSSID, aSSID_PASS);

    // send(M_XREMOTE);
    // receive();
    send(M_INFO);
    receive();
    send(M_STATUS);
    receive();
}

template<std::size_t SIZE>
void sendReceive(std::array<const char*,SIZE> ary, const char* &msg) {
  for (int z = 0; z < ary.size(); z++) {
    send(ary[z], msg);
    receive();
    delay(2000);
  }
}

std::array<const char*,3> CHANNELS_TO_TURN_ON_AND_OFF {
  "/ch/01/mix/on",
  "/ch/03/mix/on",
  "/rtn/aux/mix/on",
};

const char* CHANNEL_OFF = "OFF"; // i.e. Mute on
const char* CHANNEL_ON = "ON";   // i.e. Mute off

std::array<const char*,2> CHANNEL_MSGS_TO_CHANGE_SEND_LEVEL {
  "/ch/01/mix/01/level",
  "/ch/03/mix/01/level",
  };

const char* LEVEL_ON = "0";
const char* LEVEL_OFF = "-127";

void sendABunchOfMessages() {
  sendReceive(CHANNEL_MSGS_TO_CHANGE_SEND_LEVEL, LEVEL_ON);
  sendReceive(CHANNELS_TO_TURN_ON_AND_OFF, CHANNEL_OFF);
  sendReceive(CHANNEL_MSGS_TO_CHANGE_SEND_LEVEL, LEVEL_OFF);
  sendReceive(CHANNELS_TO_TURN_ON_AND_OFF, CHANNEL_ON);
}

void loop() {
  sendABunchOfMessages();
} //End of main loop
