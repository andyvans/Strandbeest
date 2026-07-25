#pragma once

namespace constants {

    constexpr int BOARD_LED_PIN = 4;
    constexpr int BOARD_LED_PWM_CHANNEL = 0;
    constexpr int BOARD_LED_PWM_FREQUENCY = 5000;
    constexpr int BOARD_LED_PWM_RESOLUTION_BITS = 8;
    constexpr int BOARD_LED_FLASH_DUTY = 24;

    constexpr int MOTOR_IN1_PIN = 12;
    constexpr int MOTOR_IN2_PIN = 13;
    constexpr int MOTOR_IN3_PIN = 14;
    constexpr int MOTOR_IN4_PIN = 15;

    constexpr int CAM_PIN_D0 = 5;
    constexpr int CAM_PIN_D1 = 18;
    constexpr int CAM_PIN_D2 = 19;
    constexpr int CAM_PIN_D3 = 21;
    constexpr int CAM_PIN_D4 = 36;
    constexpr int CAM_PIN_D5 = 39;
    constexpr int CAM_PIN_D6 = 34;
    constexpr int CAM_PIN_D7 = 35;
    constexpr int CAM_PIN_XCLK = 0;
    constexpr int CAM_PIN_PCLK = 22;
    constexpr int CAM_PIN_VSYNC = 25;
    constexpr int CAM_PIN_HREF = 23;
    constexpr int CAM_PIN_SCCB_SDA = 26;
    constexpr int CAM_PIN_SCCB_SCL = 27;
    constexpr int CAM_PIN_PWDN = 32;
    constexpr int CAM_PIN_RESET = -1;

} // namespace constants
