#ifndef XR_GLOBAL_INIT_h
#define XR_GLOBAL_INIT_h

#include "XRNavigation.h"

const std::string M_STATUS = "/status";

XRNavigation &navigation();

bool waitForConnection();

bool discoverXrIp(IPAddress &result);

bool connectThru(const std::string &ssid, const std::string &pass);

bool tryToReconnectWifi();

#endif