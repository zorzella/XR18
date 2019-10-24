#ifndef NETWORKS_h
#define NETWORKS_h

#include <vector>
#include <string>

struct WifiNetwork {
    const std::string ssid;
    const std::string password;
};

// The networks to look for to connect to your XR
std::vector<WifiNetwork> XR_NETWORKS {
  WifiNetwork {"SSID1", "pass"},
  WifiNetwork {"SSID2", "pass"},
};

// The name of your XR box. This is not strictly
// necessary, but will be used to disambiguate if
// multiple XRs are found.
std::vector<std::string> XR_NAMES {
  "name of your XR",
};

#endif