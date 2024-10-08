#ifndef SERVERManager_h
#define SERVERManager_h

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "SPIFFS.h"

class ServerManager {
  public:
    ServerManager(const char* ssidFilePath, const char* passFilePath);
    void begin();
    void connectWiFi();
    void checkWiFi();
    String getssid();
    String getpass();
    String getAPI();
    

  private:
    AsyncWebServer server;
    const char* ssidPath;
    const char* passPath;
    String ssid;
    String pass;
    const int WiFiLED = 12; // Green LED

    void initSPIFFS();
    String readFile(fs::FS &fs, const char *path);
    void writeFile(fs::FS &fs, const char *path, const char *message);
    bool initWiFi();
    void createServer();
    String generateDeviceID();
};

#endif
