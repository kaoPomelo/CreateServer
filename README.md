# CreateServer
This is an object-oriented program related to creating SERVER using in ESP32.

## Main程式說明
(使用前，請先將data燒錄進SPIFFFs中)
1. 先創建Server，讓使用者填寫WiFi帳號密碼。
2. 創建完後，用connectWiFi()連線。
3. 用完connectWiFi()後才能使checkWiFi()，如果有連線，腳位12亮。

## .h public : 說明
* void begin();
此方法用於啟動Server。

* void connectWiFi();
試著連線WiFi()。

* void checkWiFi();
檢查是否連線WiFi。
