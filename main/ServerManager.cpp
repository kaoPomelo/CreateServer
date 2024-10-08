#include "ServerManager.h"

ServerManager::ServerManager(const char* ssidFilePath, const char* passFilePath)
    : server(80), ssidPath(ssidFilePath), passPath(passFilePath) {}

void ServerManager::begin() {
    Serial.begin(115200);
    initSPIFFS();

    ssid = readFile(SPIFFS, ssidPath);
    pass = readFile(SPIFFS, passPath);

    Serial.println(ssid);
    Serial.println(pass);

    if (initWiFi()) {
        server.begin();
    } else {
        Serial.println("Setting AP (Access Point)");
        WiFi.softAP("ESP-WIFI-MANAGER", NULL);
        createServer();
    }
}

void ServerManager::initSPIFFS() {
    if (!SPIFFS.begin(true)) {
        Serial.println("An error has occurred while mounting SPIFFS");
    }
    Serial.println("SPIFFS mounted successfully");
}

String ServerManager::readFile(fs::FS &fs, const char *path) {
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if (!file || file.isDirectory()) {
        Serial.println("- failed to open file for reading");
        return String();
    }

    String fileContent;
    while (file.available()) {
        fileContent = file.readStringUntil('\n');
        break;
    }
    return fileContent;
}

void ServerManager::writeFile(fs::FS &fs, const char *path, const char *message) {
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file) {
        Serial.println("- failed to open file for writing");
        return;
    }
    if (file.print(message)) {
        Serial.println("- file written");
    } else {
        Serial.println("- write failed");
    }
}

bool ServerManager::initWiFi() {
    if (ssid == "") {
        Serial.println("Undefined SSID.");
        return false;
    }

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), pass.c_str());
    Serial.println("Connecting to WiFi...");

    unsigned long previousMillis = millis();
    const long interval = 10000;

    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - previousMillis >= interval) {
            Serial.println("Failed to connect.");
            return false;
        }
    }
    return true;
}

void ServerManager::createServer() {
    server.on("/", HTTP_GET, [this](AsyncWebServerRequest * request) {
        request->send(SPIFFS, "/wifimanager.html", "text/html");
    });

    server.serveStatic("/", SPIFFS, "/");

    server.on("/", HTTP_POST, [this](AsyncWebServerRequest * request) {
        int params = request->params();
        for (int i = 0; i < params; i++) {
            AsyncWebParameter* p = request->getParam(i);
            if (p->isPost()) {
                if (p->name() == "ssid") {
                    ssid = p->value().c_str();
                    Serial.print("SSID set to: ");
                    Serial.println(ssid);
                    writeFile(SPIFFS, ssidPath, ssid.c_str());
                }
                if (p->name() == "pass") {
                    pass = p->value().c_str();
                    Serial.print("Password set to: ");
                    Serial.println(pass);
                    writeFile(SPIFFS, passPath, pass.c_str());
                }
            }
        }
        request->send(200, "text/plain", "Done. ESP will restart.");
        delay(3000);
        ESP.restart();
    });

    server.begin();
}

void ServerManager::connectWiFi() {
    WiFi.begin(ssid.c_str(), pass.c_str());
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected!!!!!!!");
    Serial.printf("WiFi.RSSI:%d\n", WiFi.RSSI());
}

void ServerManager::checkWiFi() {
    pinMode(WiFiLED, OUTPUT);
    if (WiFi.status() == WL_CONNECTED) {
        digitalWrite(WiFiLED, HIGH);
    } else {
        digitalWrite(WiFiLED, LOW);
    }
}
