#ifndef MOTOR_MANAGER_H
#define MOTOR_MANAGER_H

#include <Arduino.h>
#include "config.h"

class Motor {
private:
    uint8_t in1, in2, en;
    uint8_t pwmChannel;
    
public:
    Motor(uint8_t _in1, uint8_t _in2, uint8_t _en, uint8_t _channel) 
        : in1(_in1), in2(_in2), en(_en), pwmChannel(_channel) {
        pinMode(in1, OUTPUT);
        pinMode(in2, OUTPUT);
        pinMode(en, OUTPUT);
        ledcSetup(pwmChannel, 5000, 8); // 5kHz PWM, 8-bit resolution
        ledcAttachPin(en, pwmChannel);
    }
    
    void forward(uint8_t speed = MOTOR_MAX_SPEED) {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        ledcWrite(pwmChannel, speed);
    }
    
    void backward(uint8_t speed = MOTOR_MAX_SPEED) {
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        ledcWrite(pwmChannel, speed);
    }
    
    void stop() {
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        ledcWrite(pwmChannel, 0);
    }
    
    void setSpeed(uint8_t speed) {
        ledcWrite(pwmChannel, speed);
    }
};

class MotorManager {
private:
    Motor* motor1;
    Motor* motor2;
    bool isMoving = false;
    
public:
    void begin() {
        motor1 = new Motor(MOTOR1_IN1, MOTOR1_IN2, MOTOR1_EN, 0);
        motor2 = new Motor(MOTOR2_IN1, MOTOR2_IN2, MOTOR2_EN, 1);
        Serial.println("Motor Manager initialized");
    }
    
    void moveForward(uint8_t speed = MOTOR_MAX_SPEED) {
        motor1->forward(speed);
        motor2->forward(speed);
        isMoving = true;
        Serial.println("Moving forward");
    }
    
    void moveBackward(uint8_t speed = MOTOR_MAX_SPEED) {
        motor1->backward(speed);
        motor2->backward(speed);
        isMoving = true;
        Serial.println("Moving backward");
    }
    
    void turnLeft(uint8_t speed = MOTOR_MAX_SPEED) {
        motor1->backward(speed);
        motor2->forward(speed);
        isMoving = true;
        Serial.println("Turning left");
    }
    
    void turnRight(uint8_t speed = MOTOR_MAX_SPEED) {
        motor1->forward(speed);
        motor2->backward(speed);
        isMoving = true;
        Serial.println("Turning right");
    }
    
    void stop() {
        motor1->stop();
        motor2->stop();
        isMoving = false;
        Serial.println("Motors stopped");
    }
    
    void setSpeed(uint8_t leftSpeed, uint8_t rightSpeed) {
        motor1->setSpeed(leftSpeed);
        motor2->setSpeed(rightSpeed);
    }
    
    bool getIsMoving() {
        return isMoving;
    }
};

#endif
