#include "web_server.h"
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <Ticker.h>
#include "motor_control.h"
#include "constants.h"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

namespace {

    Ticker ledOffTimer;
    constexpr int kLedPwmMaxDuty = (1 << constants::BOARD_LED_PWM_RESOLUTION_BITS) - 1;
    bool lightEnabled = false;

    void writeBoardLedDuty(int duty) {
        ledcWrite(constants::BOARD_LED_PWM_CHANNEL, kLedPwmMaxDuty - duty);
    }

    void applyBoardLedState() {
        if (lightEnabled) {
            writeBoardLedDuty(kLedPwmMaxDuty);
        }
        else {
            writeBoardLedDuty(0);
        }
    }

    void turnBoardLedOff() {
        applyBoardLedState();
    }

    void flashBoardLed() {
        if (lightEnabled) {
            return;
        }
        writeBoardLedDuty(constants::BOARD_LED_FLASH_DUTY);
        ledOffTimer.detach();
        ledOffTimer.once_ms(40, turnBoardLedOff);
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

    flashBoardLed();

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

    ledcSetup(
        constants::BOARD_LED_PWM_CHANNEL,
        constants::BOARD_LED_PWM_FREQUENCY,
        constants::BOARD_LED_PWM_RESOLUTION_BITS
    );
    ledcAttachPin(constants::BOARD_LED_PIN, constants::BOARD_LED_PWM_CHANNEL);
    turnBoardLedOff();

    ws.onEvent([](AsyncWebSocket* server, AsyncWebSocketClient* client,
        AwsEventType type, void* arg, uint8_t* data, size_t len) {
            if (type == WS_EVT_DATA) handleWSMessage(arg, data, len);
        });

    server.addHandler(&ws);
    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
    server.begin();
}
