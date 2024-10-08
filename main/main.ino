#include "ServerManager.h"

ServerManager serverManager("/ssid.txt", "/pass.txt");

void setup() {
  Serial.begin(115200);
  serverManager.begin(); //創造Server
  serverManager.connectWiFi(); //連網
  serverManager.checkWiFi(); //檢查網路，12燈亮
}

void loop() { 
}
