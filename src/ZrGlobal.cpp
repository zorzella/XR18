#include "ZrDebug.h"

#include "ZrGlobal.h"
#include "ZrComm.h"
#include "ZrNavigation.h"
#include "networks.h"

// static ZrNavigation m_navigation;

// ZrNavigation &navigation() { return ZrNavigation::instance(); }

bool waitForConnection() {
  unsigned long timeoutAt = millis() + 4000;
  wl_status_t lastStatus{WL_NO_SHIELD};
  // TODO: timeout and do something (look at more networks?). Also
  // maybe there's a way to reset WiFi etc (I've seen it get stuck and
  // need to have the tires kicked)...
  while (millis() < timeoutAt) {
    wl_status_t status = WiFi.status();
    if (DEBUG_WIFI) {
      if (status != lastStatus) {
        Serial.print(status);
      }
      lastStatus = status;
    }
    // TODO: deal with other statuses
    // See https://www.arduino.cc/en/Reference/WiFiStatus
    if (status == WL_CONNECTED) {
      Serial.println();
      return true;
    } else if (status == WL_DISCONNECTED) {
      // Fairly normal for starters, we should start here
    } else if (status == WL_IDLE_STATUS) {
      // Fairly normal for starters, it will be in this status
      // while it tries to connect
    }
    delay(100);
    Serial.print(".");
  }
  Serial.println();
  return false;
}

bool discoverXrIp(IPAddress &result) {
  // TODO: handle XR_NAMES
  IPAddress broadcastIp = WiFi.broadcastIP();
  unsigned long timeoutAt = millis() + 1000;
  while (millis() < timeoutAt) {
    if (!send1To(broadcastIp, M_STATUS)) {
      continue;
    }
    OSCMessage msg;
    bool receiveResult = receiveOscWithAddress(msg, M_STATUS);
    if (receiveResult) {
      char buffer[SIZE_OF_RECEIVE_BUFFER];
      memset(buffer, 0, SIZE_OF_RECEIVE_BUFFER);
      msg.getString(1, buffer);
      Serial.print("Remote IP: ");
      Serial.println(buffer);
      result.fromString(buffer);
      return true;
    } else {
      Serial.println("Retrying");
    }
  }
  return false;
}

bool connectThru(const std::string &ssid, const std::string &pass) {
  unsigned long start = millis();
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(ssid.c_str());
  Serial.print(" ");
  WiFi.begin(ssid.c_str(), pass.c_str());

  bool result = waitForConnection();
  int elapsed = millis() - start;
  if (result) {
    Serial.print("Success in: ");
    Serial.println(elapsed);
    return result;
  } else {
    Serial.print("Failed to connect in: ");
    Serial.println(elapsed);
    return result;
  }
  Serial.print("Connected to WiFi: ");
  Serial.println(ssid.c_str());

  Serial.print("Local IP / Broadcast / Gateway: ");
  Serial.println(WiFi.localIP());
  Serial.print(" / ");
  Serial.println(WiFi.broadcastIP());
  Serial.print(" / ");
  Serial.println(WiFi.gatewayIP());
  Serial.println();
  Serial.println();

  return result;
}

bool tryToReconnectWifi() {
  bool result = false;
  // TODO: start with the last successful network
  for (int i = 0; i < XR_NETWORKS.size(); i++) {
    std::string ssid = XR_NETWORKS[i].ssid;
    std::string pass = XR_NETWORKS[i].password;
    bool connectResult = connectThru(ssid.c_str(), pass.c_str());
    if (connectResult) {
      bool discoverResult = discoverXrIp(xrIp());
      if (discoverResult) {
        return true;
      } else {
        result = discoverResult;
      }
    }
  }
  return result;
}
