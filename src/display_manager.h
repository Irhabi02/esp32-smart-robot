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
    
    void showHappyFace() {
        display.clearDisplay();
        
        // Draw happy face
        // Face circle
        display.drawCircle(64, 32, 30, SSD1306_WHITE);
        
        // Eyes
        display.fillCircle(50, 24, 4, SSD1306_WHITE);  // Left eye
        display.fillCircle(78, 24, 4, SSD1306_WHITE);  // Right eye
        
        // Happy mouth (arc)
        for (int x = 48; x <= 80; x++) {
            int y = 35 + (x - 48) * (x - 48) / 256;
            display.drawPixel(x, y, SSD1306_WHITE);
        }
        
        display.display();
    }
    
    void showSadFace() {
        display.clearDisplay();
        
        // Draw sad face
        display.drawCircle(64, 32, 30, SSD1306_WHITE);
        
        // Eyes
        display.fillCircle(50, 24, 4, SSD1306_WHITE);
        display.fillCircle(78, 24, 4, SSD1306_WHITE);
        
        // Sad mouth (inverted arc)
        for (int x = 48; x <= 80; x++) {
            int y = 48 - (x - 48) * (x - 48) / 256;
            display.drawPixel(x, y, SSD1306_WHITE);
        }
        
        display.display();
    }
    
    void showAngryFace() {
        display.clearDisplay();
        
        display.drawCircle(64, 32, 30, SSD1306_WHITE);
        
        // Angry eyes (slanted)
        display.drawLine(46, 20, 54, 28, SSD1306_WHITE);
        display.drawLine(74, 20, 82, 28, SSD1306_WHITE);
        
        // Angry mouth
        display.drawLine(48, 45, 80, 45, SSD1306_WHITE);
        
        display.display();
    }
    
    void showConfusedFace() {
        display.clearDisplay();
        
        display.drawCircle(64, 32, 30, SSD1306_WHITE);
        
        // Confused eyes (circles with inner dots)
        display.drawCircle(50, 24, 3, SSD1306_WHITE);
        display.drawCircle(78, 24, 3, SSD1306_WHITE);
        display.fillCircle(50, 24, 1, SSD1306_WHITE);
        display.fillCircle(78, 24, 1, SSD1306_WHITE);
        
        // Question mark mouth
        display.drawCircle(64, 42, 3, SSD1306_WHITE);
        display.drawPixel(64, 48, SSD1306_WHITE);
        
        display.display();
    }
    
    void showMovingAnimation() {
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(25, 10);
        display.println("MOVING...");
        
        // Draw moving robot
        display.drawRect(30, 30, 20, 15, SSD1306_WHITE);
        display.drawLine(30, 35, 20, 40, SSD1306_WHITE);
        display.drawLine(50, 35, 60, 40, SSD1306_WHITE);
        
        display.display();
    }
    
    void showTalkingAnimation() {
        display.clearDisplay();
        
        // Draw talking face with animated mouth
        display.drawCircle(64, 32, 30, SSD1306_WHITE);
        display.fillCircle(50, 24, 4, SSD1306_WHITE);
        display.fillCircle(78, 24, 4, SSD1306_WHITE);
        display.fillCircle(64, 42, 5, SSD1306_WHITE);
        
        display.display();
    }
    
    void clear() {
        display.clearDisplay();
        display.display();
    }
};

#endif
