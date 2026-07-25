#include "motor_control.h"
#include <Arduino.h>
#include "constants.h"

void setupMotors() {
    pinMode(constants::MOTOR_IN1_PIN, OUTPUT);
    pinMode(constants::MOTOR_IN2_PIN, OUTPUT);
    pinMode(constants::MOTOR_IN3_PIN, OUTPUT);
    pinMode(constants::MOTOR_IN4_PIN, OUTPUT);
    Serial.println("[MOTOR] setup complete");
}

void driveForward() {
    Serial.println("[MOTOR] forward");
    digitalWrite(constants::MOTOR_IN1_PIN, HIGH);
    digitalWrite(constants::MOTOR_IN2_PIN, LOW);
    digitalWrite(constants::MOTOR_IN3_PIN, HIGH);
    digitalWrite(constants::MOTOR_IN4_PIN, LOW);
}

void driveBackward() {
    Serial.println("[MOTOR] backward");
    digitalWrite(constants::MOTOR_IN1_PIN, LOW);
    digitalWrite(constants::MOTOR_IN2_PIN, HIGH);
    digitalWrite(constants::MOTOR_IN3_PIN, LOW);
    digitalWrite(constants::MOTOR_IN4_PIN, HIGH);
}

void turnLeft() {
    Serial.println("[MOTOR] left");
    digitalWrite(constants::MOTOR_IN1_PIN, LOW);
    digitalWrite(constants::MOTOR_IN2_PIN, HIGH);
    digitalWrite(constants::MOTOR_IN3_PIN, HIGH);
    digitalWrite(constants::MOTOR_IN4_PIN, LOW);
}

void turnRight() {
    Serial.println("[MOTOR] right");
    digitalWrite(constants::MOTOR_IN1_PIN, HIGH);
    digitalWrite(constants::MOTOR_IN2_PIN, LOW);
    digitalWrite(constants::MOTOR_IN3_PIN, LOW);
    digitalWrite(constants::MOTOR_IN4_PIN, HIGH);
}

void stopMotors() {
    Serial.println("[MOTOR] stop");
    digitalWrite(constants::MOTOR_IN1_PIN, LOW);
    digitalWrite(constants::MOTOR_IN2_PIN, LOW);
    digitalWrite(constants::MOTOR_IN3_PIN, LOW);
    digitalWrite(constants::MOTOR_IN4_PIN, LOW);
}
