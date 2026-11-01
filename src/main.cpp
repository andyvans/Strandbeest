#include <Arduino.h>
#include "wifi_ap.h"
#include "video_stream.h"
#include "motor_control.h"
#include "web_server.h"

void setup() {
    Serial.begin(115200);
    delay(500);

    // Drive motor pins LOW before WiFi starts so boot-time pin glitches can't spin the motors
    setupMotors();
    stopMotors();
    setupWiFiAP();
    setupCamera();
    setupWebServer();

    Serial.println("Robot ready. Connect to WiFi: ESP32_Robot");
}

void loop() {
    // AsyncWebServer + WebSocket are event-driven; no blocking loop needed.
    // You can add periodic tasks here later (battery monitor, sensors, etc.).
    delay(10);
}
