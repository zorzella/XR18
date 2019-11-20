#include "ZrDebug.h"

#include <OSCMessage.h>
#include <string>

#include "ZoscValue.h"
#include "ZrComm.h"
#include "ZrGlobal.h"
#include "ZrNavigation.h"
#include "networks.h"

WiFiUDP wifiUdp;  // A UDP instance to let us send and receive packets over UDP

IPAddress m_xrIp{INADDR_NONE};  // IP of the XR18

unsigned long sendOkCount = 0;
unsigned long sendErrorCount = 0;
unsigned long recOkCount = 0;
unsigned long recErrorCount = 0;

ZrComm m_instance;

ZrComm &ZrComm::instance() { return m_instance; }

bool m_isConnectedToNetwork = false;

bool ZrComm::isConnectedToNetwork() { return m_isConnectedToNetwork; }

bool m_isConnectedToXr = false;

bool ZrComm::isConnectedToXr() { return m_isConnectedToXr; }

static const int MAX_NAME_SIZE = 50;

char m_networkName[MAX_NAME_SIZE]{"UNKNOWN"};
char m_xrName[MAX_NAME_SIZE]{"UNKNOWN"};

char *ZrComm::networkName() { return m_networkName; }

char *ZrComm::xrName() { return m_xrName; }

long m_reconnectionToNetworkStartedAtTimestamp = 0;
long m_reconnectionToXrStartedAtTimestamp = 0;

void ZrComm::ensureConnection() {
  if (WiFi.status() == WL_CONNECTED) {
    m_isConnectedToNetwork = true;
  } else {
    m_isConnectedToNetwork = false;
    m_isConnectedToXr = false;
  }

  if (xrIp() == INADDR_NONE) {
    m_isConnectedToXr = false;
  } else {
    m_isConnectedToXr = true;
  }

  if (m_isConnectedToXr) {
    return;
  }

  if (!m_isConnectedToNetwork) {
    tryToReconnectToNetwork();
  } else {
    tryToReconnectToXr();
  }
}

int m_indexOfNetworkCurrentlyBeingTried = -1;

void ZrComm::tryToReconnectToNetwork() {
  int oldIndex = m_indexOfNetworkCurrentlyBeingTried;
  // TODO: start with the last successful network
  if (m_indexOfNetworkCurrentlyBeingTried == -1) {
    // TODO: work for other networks.
    m_indexOfNetworkCurrentlyBeingTried = 0;
  }

  int i = m_indexOfNetworkCurrentlyBeingTried;
  std::string ssid = XR_NETWORKS[i].ssid;
  std::string pass = XR_NETWORKS[i].password;
  if (oldIndex == -1) {
    memcpy(m_networkName, ssid.c_str(), MAX_NAME_SIZE);
  }
  connectThru2(ssid.c_str(), pass.c_str());
}

void ZrComm::tryToReconnectToXr() {
  if (m_reconnectionToXrStartedAtTimestamp == 0) {
    Serial.println("Trying to find and connect to an XR");
    m_reconnectionToXrStartedAtTimestamp = millis();
  }
  triggerDiscoverXrIp(m_xrIp);
}

void ZrComm::triggerDiscoverXrIp(IPAddress &result) {
  // TODO: handle XR_NAMES
  IPAddress broadcastIp = WiFi.broadcastIP();
  unsigned long timeoutAt = millis() + 1000;
  while (millis() < timeoutAt) {
    // TODO: try once, return to try again later
    if (!send1To(broadcastIp, M_STATUS)) {
      continue;
    }

    // TODO: remove delay
    delay(500);
  }
}

void ZrComm::connectThru2(const std::string &ssid, const std::string &pass) {
  if (m_reconnectionToNetworkStartedAtTimestamp == 0) {
    Serial.println("Wifi down. Reconnecting.");
    m_reconnectionToNetworkStartedAtTimestamp = millis();
    // Connect to WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.print(ssid.c_str());
    Serial.print(" ");
    WiFi.begin(ssid.c_str(), pass.c_str());
  }
}

void ZrComm::mStatusReceived(OSCMessage &msg) {
  TRACE();
  char buffer[SIZE_OF_RECEIVE_BUFFER];
  memset(buffer, 0, SIZE_OF_RECEIVE_BUFFER);
  msg.getString(1, buffer);
  Serial.print("Remote IP: ");
  Serial.println(buffer);
  m_xrIp.fromString(buffer);
  msg.getString(2, m_xrName, MAX_NAME_SIZE);
  Serial.print("Name: ");
  Serial.println(m_xrName);
}

IPAddress &xrIp() { return m_xrIp; };

/*
 * Receives a OSC message from the network, if any.
 *
 * Return true if a message was there to be received, false otherwise.
 */
void receiveOscIfAny(OSCMessage &msg) {
  int size = wifiUdp.parsePacket();

  if (size == 0) {
    return;
  }
  while (size--) {
    msg.fill(wifiUdp.read());
  }

  ZrNavigation::instance().updateCachedValue(msg);
}

bool receiveOsc(OSCMessage &msg) {
  unsigned long timeoutAt = millis() + 300;
  //   char buffer[SIZE_OF_RECEIVE_BUFFER];
  //   memset(buffer, 0, SIZE_OF_RECEIVE_BUFFER);
  while (millis() < timeoutAt) {
    receiveOscIfAny(msg);
    if (msg.size() > 0) {
      recOkCount++;
      return true;
    }
    delay(10);
  }
  recErrorCount++;
  return false;
}

bool sendUdp(const IPAddress &ip, OSCMessage &msg) {
  unsigned long timeoutAt = millis() + 100;
  while (millis() < timeoutAt) {
    if (!wifiUdp.beginPacket(ip, XR_PORT)) {
      sendErrorCount++;
      continue;
    }
    msg.send(wifiUdp);
    int endPacketResult = wifiUdp.endPacket();
    msg.empty();
    if (!endPacketResult) {
      sendErrorCount++;
      continue;
    }
    sendOkCount++;
    return true;
  }
  return false;
}

bool send1To(const IPAddress &ip, const std::string &mess) {
  OSCMessage msg(mess.c_str());
  Serial.print(">>> [");
  Serial.print(sendOkCount);
  Serial.print(",");
  Serial.print(sendErrorCount);
  Serial.print("] ");
  Serial.println(mess.c_str());
  bool result = sendUdp(ip, msg);
  return result;
}

bool send1(const std::string &mess) { return send1To(m_xrIp, mess); }

bool send2(const std::string &one, const ZoscValue &two) {
  Serial.print(">>> [");
  Serial.print(sendOkCount);
  Serial.print(",");
  Serial.print(sendErrorCount);
  Serial.print("] ");

  Serial.print(one.c_str());
  Serial.print(" ");
  Serial.print(two.asStrOsc().c_str());

  OSCMessage msg(one.c_str());
  two.addItselfTo(msg);

  bool result = sendUdp(m_xrIp, msg);
  return result;
}

bool send2(const std::string &one, const int &two) {
  Serial.print(">>> [");
  Serial.print(sendOkCount);
  Serial.print(",");
  Serial.print(sendErrorCount);
  Serial.print("] ");

  Serial.print(one.c_str());
  Serial.print(" ");
  Serial.println(two);

  OSCMessage msg(one.c_str());
  msg.add(two);

  bool result = sendUdp(m_xrIp, msg);
  return result;
}

bool send2(const std::string &one, const std::string &two) {
  Serial.print(">>> [");
  Serial.print(sendOkCount);
  Serial.print(",");
  Serial.print(sendErrorCount);
  Serial.print("] ");

  Serial.print(one.c_str());
  Serial.print(" ");
  Serial.println(two.c_str());

  OSCMessage msg(one.c_str());
  msg.add(two.c_str());

  bool result = sendUdp(m_xrIp, msg);
  return result;
}

// prints an OSCMessage
void printMsg(OSCMessage &msg) {
  if (msg.hasError()) {
    OSCErrorCode error = msg.getError();
    Serial.print("Error Code: ");
    Serial.println(error);
    return;
  }

  char buffer[SIZE_OF_RECEIVE_BUFFER];
  //   memset(buffer, 0, SIZE_OF_RECEIVE_BUFFER);

  msg.getAddress(buffer);
  Serial.print(buffer);

  for (int i = 0; i < msg.size(); i++) {
    Serial.print(" [");
    if (msg.isString(i)) {
      msg.getString(i, buffer);
      Serial.print("s - ");
      Serial.print(buffer);
    } else if (msg.isInt(i)) {
      Serial.print("i - ");
      Serial.print(msg.getInt(i));
    } else if (msg.isFloat(i)) {
      Serial.print("f - ");
      Serial.print(msg.getFloat(i));
    } else {
      Serial.print("type: ");
      Serial.print(msg.getType(i));
    }
    Serial.print("]");
  }
}

// prints a received message.
void printRec(OSCMessage &msg) {
  Serial.print("<<< [");
  Serial.print(recOkCount);
  Serial.print(",");
  Serial.print(recErrorCount);
  Serial.print("] ");
  printMsg(msg);
  Serial.println();
}

void receiveAndPrintOscIfAny() {
  OSCMessage msg;
  while (true) {
    receiveOscIfAny(msg);
    if (msg.size() == 0) {
      return;
    }
    printRec(msg);
    msg.empty();
  }
}

bool receiveOscWithAddress(OSCMessage &msg, const std::string &address) {
  unsigned long startTs = millis();
  unsigned long timeoutAt = startTs + 400;

  char buffer[SIZE_OF_RECEIVE_BUFFER];
  memset(buffer, 0, SIZE_OF_RECEIVE_BUFFER);
  while (millis() < timeoutAt) {
    if (receiveOsc(msg)) {
      msg.getAddress(buffer);
      if (address.compare(buffer)) {
        Serial.print("Unexpected ");
        printRec(msg);
      } else {
        recOkCount++;
        Serial.print(millis() - startTs);
        Serial.print("ms ");
        return true;
      }
      msg.empty();
    } else {
      delay(10);
    }
  }
  recErrorCount++;
  return false;
}

void warmup() {
  std::string someMessage = "/headamp/01/gain";
  // The first message (after /status) takes much longer than the
  // rest for some reason. This warms things up.
  send1(someMessage);
  OSCMessage query;
  receiveOscWithAddress(query, someMessage);
  printRec(query);
}

void receiveAndPrintIfAny() {
  OSCMessage query;
  receiveOscIfAny(query);
  printRec(query);
  query.empty();
}

void monitorOsc() {
  send1("/xremote");
  for (int i = 0; i < 150; i++) {
    receiveAndPrintIfAny();
    delay(100);
  }
}