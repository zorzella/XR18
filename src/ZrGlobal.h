#ifndef ZR_GLOBAL_INIT_h
#define ZR_GLOBAL_INIT_h

#include "ZrDebug.h"

#include "ZrNavigation.h"

const std::string M_STATUS = "/status";
const std::string M_XINFO = "/xinfo";
const std::string M_XREMOTE = "/xremote";

bool waitForConnection();

bool discoverXrIp(IPAddress &result);

bool connectThru(const std::string &ssid, const std::string &pass);

bool tryToReconnectWifi();

#endif