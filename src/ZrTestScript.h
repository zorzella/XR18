#ifndef ZR_TEST_SCRIPT_h
#define ZR_TEST_SCRIPT_h

#include <OSCMessage.h>

#include "ZrGlobal.h"

bool sendReceiveOne(const std::string &addr, const std::string &msg);
bool sendReceive(const std::vector<std::string> &ary, const std::string &msg);

void printCurrentFunction();

void runTestScript();

void sendABunchOfMessages();

#endif