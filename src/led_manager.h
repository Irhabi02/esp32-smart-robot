#ifndef LED_MANAGER_H
#define LED_MANAGER_H

#include <Arduino.h>
#include "config.h"

class LEDManager {
private:
    bool isBlinking = false;
    unsigned long lastBlink = 0;
    const unsigned long blinkInterval = 200; // 200ms
    
public:
    void begin() {
        pinMode(LED_PIN, OUTPUT);
        digitalWrite(LED_PIN, LOW);
        Serial.println("LED Manager initialized");
    }
    
    void on() {
        digitalWrite(LED_PIN, HIGH);
        isBlinking = false;
    }
    
    void off() {
        digitalWrite(LED_PIN, LOW);
        isBlinking = false;
    }
    
    void blink() {
        // Single blink
        on();
        delay(100);
        off();
        delay(100);
    }
    
    void startBlinking() {
        isBlinking = true;
        lastBlink = millis();
    }
    
    void stopBlinking() {
        isBlinking = false;
        off();
    }
    
    void update() {
        if (isBlinking) {
            unsigned long now = millis();
            if (now - lastBlink >= blinkInterval) {
                if (digitalRead(LED_PIN) == LOW) {
                    digitalWrite(LED_PIN, HIGH);
                } else {
                    digitalWrite(LED_PIN, LOW);
                }
                lastBlink = now;
            }
        }
    }
    
    void pulse(uint8_t intensity) {
        // PWM pulse effect
        ledcWrite(0, intensity);
    }
};

#endif
