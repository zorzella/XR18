#ifndef XR_TEST_SCRIPT_h
#define XR_TEST_SCRIPT_h

#include <OSCMessage.h>

#include "XRGlobal.h"

bool sendReceiveOne(const std::string &addr, const std::string &msg);
bool sendReceive(const std::vector<std::string> &ary, const std::string &msg);

void printCurrentFunction();

void runTestScript();

void sendABunchOfMessages();

#endif