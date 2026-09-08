#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "config.h"

class DisplayManager {
private:
    Adafruit_SSD1306 display;
    int animationFrame = 0;
    
public:
    DisplayManager() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}
    
    void begin() {
        Wire.begin(I2C_SDA, I2C_SCL);
        if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
            Serial.println("SSD1306 allocation failed");
            while (1);
        }
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.println("Initializing...");
        display.display();
    }
    
    void showStartupAnimation() {
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(20, 25);
        display.println("ROBOT");
        display.setCursor(5, 45);
        display.println("STARTING...");
        display.display();
    }
    
    void showWiFiSetup() {
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(15, 10);
        display.println("WiFi Setup Mode");
        display.setCursor(0, 25);
        display.println("SSID: SmartRobot_");
        display.println("Setup");
        display.setCursor(0, 45);
        display.println("Waiting for");
        display.println("connection...");
        display.display();
    }
    
    void showWaitingForConnection() {
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println("Connecting to WiFi");
        
        // Draw animated dots
        animationFrame = (animationFrame + 1) % 4;
        display.setCursor(20, 25);
        for (int i = 0; i < animationFrame; i++) {
            display.print(".");
        }
        
        display.setCursor(0, 45);
        display.println("Press WPS button");
        display.println("on your router");
        display.display();
    }
    
    void showConnectedAnimation() {
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(25, 15);
        display.println("WiFi");
        display.setCursor(20, 35);
        display.println("OK!");
        display.display();
        delay(1000);
    }
    
    void showConnectionCode(String code) {
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(10, 5);
        display.println("Connection Code:");
        display.setTextSize(2);
        display.setCursor(20, 25);
        display.println(code);
        display.setTextSize(1);
        display.setCursor(0, 50);
        display.println("Visit: xiaozhi.me");
        display.display();
    }
    
    // Helper function to draw square eyes
    void drawSquareEyes(int leftEyeX, int leftEyeY, int eyeSize) {
        // Left eye (square/rectangle)
        display.drawRect(leftEyeX - eyeSize/2, leftEyeY - eyeSize/2, eyeSize, eyeSize, SSD1306_WHITE);
        
        // Right eye (square/rectangle)
        display.drawRect(leftEyeX + 30, leftEyeY - eyeSize/2, eyeSize, eyeSize, SSD1306_WHITE);
    }
    
    void showHappyFace() {
        display.clearDisplay();
        
        // Draw only square eyes (happy = normal square eyes)
        drawSquareEyes(42, 32, 14);
        
        display.display();
    }
    
    void showSadFace() {
        display.clearDisplay();
        
        // Draw square eyes with small inner marks to show sadness
        display.drawRect(35, 25, 14, 14, SSD1306_WHITE);
        display.drawRect(65, 25, 14, 14, SSD1306_WHITE);
        
        // Small tear marks
        display.drawPixel(42, 40, SSD1306_WHITE);
        display.drawPixel(42, 41, SSD1306_WHITE);
        display.drawPixel(72, 40, SSD1306_WHITE);
        display.drawPixel(72, 41, SSD1306_WHITE);
        
        display.display();
    }
    
    void showAngryFace() {
        display.clearDisplay();
        
        // Angry eyes - filled squares
        display.fillRect(35, 25, 14, 14, SSD1306_WHITE);
        display.fillRect(65, 25, 14, 14, SSD1306_WHITE);
        
        // Diagonal line on eyes to show anger
        display.drawLine(35, 25, 49, 39, SSD1306_BLACK);
        display.drawLine(65, 25, 79, 39, SSD1306_BLACK);
        
        display.display();
    }
    
    void showConfusedFace() {
        display.clearDisplay();
        
        // Confused eyes - squares with dots in center
        display.drawRect(35, 25, 14, 14, SSD1306_WHITE);
        display.drawRect(65, 25, 14, 14, SSD1306_WHITE);
        
        // Center dots for confused expression
        display.fillCircle(42, 32, 1, SSD1306_WHITE);
        display.fillCircle(72, 32, 1, SSD1306_WHITE);
        
        // Extra question mark indicator
        display.drawPixel(64, 50, SSD1306_WHITE);
        display.drawPixel(64, 51, SSD1306_WHITE);
        
        display.display();
    }
    
    void showMovingAnimation() {
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(25, 10);
        display.println("MOVING...");
        
        // Draw moving robot with square eyes
        display.drawRect(30, 30, 20, 15, SSD1306_WHITE);
        
        // Square eyes on the robot body
        display.drawRect(37, 35, 4, 4, SSD1306_WHITE);
        display.drawRect(45, 35, 4, 4, SSD1306_WHITE);
        
        // Movement lines
        display.drawLine(30, 35, 20, 40, SSD1306_WHITE);
        display.drawLine(50, 35, 60, 40, SSD1306_WHITE);
        
        display.display();
    }
    
    void showTalkingAnimation() {
        display.clearDisplay();
        
        // Draw talking face with animated square eyes
        // Left eye
        display.drawRect(35, 25, 14, 14, SSD1306_WHITE);
        
        // Right eye
        display.drawRect(65, 25, 14, 14, SSD1306_WHITE);
        
        // Talking indicator - animated mouth representation with simple dot patterns
        animationFrame = (animationFrame + 1) % 3;
        
        if (animationFrame == 0) {
            display.drawPixel(64, 45, SSD1306_WHITE);
            display.drawPixel(64, 46, SSD1306_WHITE);
        } else if (animationFrame == 1) {
            display.drawPixel(60, 45, SSD1306_WHITE);
            display.drawPixel(68, 45, SSD1306_WHITE);
        } else {
            display.drawPixel(62, 46, SSD1306_WHITE);
            display.drawPixel(66, 46, SSD1306_WHITE);
        }
        
        display.display();
    }
    
    void clear() {
        display.clearDisplay();
        display.display();
    }
};

#endif
