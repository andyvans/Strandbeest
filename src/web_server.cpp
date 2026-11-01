#include "web_server.h"
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "motor_control.h"
#include "constants.h"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

namespace {

    bool lightEnabled = false;

    void applyBoardLedState() {
        const bool level = lightEnabled != constants::BOARD_LED_ACTIVE_LOW;
        digitalWrite(constants::BOARD_LED_PIN, level ? HIGH : LOW);
    }

} // namespace

void handleWSMessage(void* arg, uint8_t* data, size_t len) {
    String cmd((const char*)data, len);

    if (cmd == "light") {
        lightEnabled = !lightEnabled;
        applyBoardLedState();
        Serial.printf("[LIGHT] %s\n", lightEnabled ? "on" : "off");
        return;
    }

    if (cmd == "forward") driveForward();
    else if (cmd == "back") driveBackward();
    else if (cmd == "left") turnLeft();
    else if (cmd == "right") turnRight();
    else stopMotors();
}

void setupWebServer() {
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS mount failed");
        return;
    }

    pinMode(constants::BOARD_LED_PIN, OUTPUT);
    applyBoardLedState();

    ws.onEvent([](AsyncWebSocket* server, AsyncWebSocketClient* client,
        AwsEventType type, void* arg, uint8_t* data, size_t len) {
            if (type == WS_EVT_DATA) handleWSMessage(arg, data, len);
        });

    server.addHandler(&ws);
    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
    server.begin();
}
