#include "ZrDebug.h"

#include <OSCMessage.h>
#include <string>

#include "ZoscValue.h"
#include "ZrComm.h"
#include "ZrGlobal.h"
#include "ZrNavigation.h"

WiFiUDP wifiUdp;  // A UDP instance to let us send and receive packets over UDP

IPAddress m_xrIp{INADDR_NONE};  // IP of the XR18

unsigned long sendOkCount = 0;
unsigned long sendErrorCount = 0;
unsigned long recOkCount = 0;
unsigned long recErrorCount = 0;

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

  navigation().updateCachedValue(msg);
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