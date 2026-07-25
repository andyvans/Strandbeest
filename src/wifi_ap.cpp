#include "wifi_ap.h"
#include <WiFi.h>

void setupWiFiAP() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP("ESP32_Robot", "12345678");

    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());
}
