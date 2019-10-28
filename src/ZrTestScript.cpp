#include <OSCMessage.h>

#include "ZrComm.h"
#include "ZrGlobal.h"
#include "ZrTestScript.h"

bool sendReceiveOne(const std::string &addr, const std::string &msg) {
  OSCMessage query;

  if (!send1(addr)) {
    return false;
  }
  if (!receiveOscWithAddress(query, addr)) {
    return false;
  }
  printRec(query);
  query.empty();

  if (!send2(addr, msg)) {
    return false;
  }

  if (!send1(addr)) {
    return false;
  }
  if (!receiveOscWithAddress(query, addr)) {
    return false;
  }
  printRec(query);
  query.empty();
  return true;
}

bool sendReceive(const std::vector<std::string> &ary, const std::string &msg) {
  bool result = true;

  for (int z = 0; z < ary.size(); z++) {
    Serial.println("vvvvvvv");
    const std::string addr = ary[z];

    bool thisResult = sendReceiveOne(addr, msg);
    result &= thisResult;

    if (thisResult) {
      Serial.println("^^^^^^^");
    } else {
      Serial.println("^^XXX^^");
    }

    delay(2000);
  }
  return result;
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

void printCurrentFunction() {
  Serial.print("Name: ");
  Serial.print(navigation().currentFunction().name().c_str());
  Serial.print(", OSC: ");
  Serial.println(navigation().currentFunction().oscAddr().c_str());
}

void runTestScript() {
  warmup();
  // monitorOsc();

  // delay(3000);

  OSCMessage query;
  send1("/-snap/01/name");
  receiveOscWithAddress(query, "/-snap/01/name");
  printRec(query);
  query.empty();

  OSCMessage query3;
  send2("/-snap/load", 1);
  // TODO: this works, but returns an INVALID_OSC message. Why?
  receiveOscWithAddress(query3, "/-snap/load");
  printRec(query3);
  query3.empty();

  delay(5000);
  sendReceiveOne(CHANNELS_TO_TURN_ON_AND_OFF[0], CHANNEL_OFF);

  printCurrentFunction();
  navigation().goRight();
  ZrFunction& f = navigation().currentFunction();

  printCurrentFunction();
  navigation().goRight();
  navigation().clickPlus();

  printCurrentFunction();
  navigation().goRight();
  navigation().clickPlus();

  printCurrentFunction();
  navigation().goUp();
  navigation().clickMinus();

  printCurrentFunction();
  navigation().goUp();
  printCurrentFunction();
  navigation().goLeft();
  printCurrentFunction();
  navigation().goLeft();
  printCurrentFunction();
  navigation().goDown();
  printCurrentFunction();
  navigation().goDown();
  printCurrentFunction();
  navigation().goDown();
}

void sendABunchOfMessages() {
  warmup();
  sendReceive(CHANNEL_MSGS_TO_CHANGE_SEND_LEVEL, LEVEL_ON);
  sendReceive(CHANNELS_TO_TURN_ON_AND_OFF, CHANNEL_OFF);
  sendReceive(CHANNEL_MSGS_TO_CHANGE_SEND_LEVEL, LEVEL_OFF);
  sendReceive(CHANNELS_TO_TURN_ON_AND_OFF, CHANNEL_ON);
}
