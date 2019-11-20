#ifndef ZR_COMM_h
#define ZR_COMM_h

#include "ZrDebug.h"

#include <OSCMessage.h>  // For OSC support
#include <WiFi.h>        // For WiFi
#include <WiFiUdp.h>     // For WiFi
#include <string>

#include "ZoscValue.h"

const unsigned int XR_PORT =
    10024;  // remote port to receive OSC X-AIR is 10024, X32 is 10023

const int SIZE_OF_RECEIVE_BUFFER = 100;

// If true, we will print extra debug information about WIFI
const bool DEBUG_WIFI = false;

class ZrComm {
 public:
  static ZrComm &instance();

  bool isConnectedToNetwork();
  bool isConnectedToXr();

  char *networkName();
  char *xrName();

  void ensureConnection();

  void mStatusReceived(OSCMessage& osc);

 private:
  void tryToReconnectToNetwork();
  void tryToReconnectToXr();
  void connectThru2(const std::string &ssid, const std::string &pass);
  void triggerDiscoverXrIp(IPAddress &result);
};

// IP of the XR18
IPAddress &xrIp();

void receiveOscIfAny(OSCMessage &msg);

bool receiveOsc(OSCMessage &msg);

bool sendUdp(const IPAddress &ip, OSCMessage &msg);

bool send1To(const IPAddress &ip, const std::string &mess);

bool send1(const std::string &mess);

bool send2(const std::string &one, const ZoscValue &two);

bool send2(const std::string &one, const int &two);

bool send2(const std::string &one, const std::string &two);

// prints an OSCMessage
void printMsg(OSCMessage &msg);

// prints a received message.
void printRec(OSCMessage &msg);

void receiveAndPrintOscIfAny();

bool receiveOscWithAddress(OSCMessage &msg, const std::string &address);

void warmup();

void receiveAndPrintIfAny();

void monitorOsc();

#endif