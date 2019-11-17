#ifndef ZR_GLOBAL_INIT_h
#define ZR_GLOBAL_INIT_h

#include "ZrDebug.h"

#include "ZrNavigation.h"

const std::string M_STATUS = "/status";

ZrNavigation &navigation();

bool waitForConnection();

bool discoverXrIp(IPAddress &result);

bool connectThru(const std::string &ssid, const std::string &pass);

bool tryToReconnectWifi();

#endif