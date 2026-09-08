#ifndef XIAOZHI_CLIENT_H
#define XIAOZHI_CLIENT_H

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "config.h"

class Xiaozhi {
private:
    WiFiClientSecure client;
    String connectionCode;
    bool connected = false;
    String lastCommand = "";
    bool hasNewCommand = false;
    
public:
    void begin(String code) {
        connectionCode = code;
        Serial.printf("Xiaozhi Client initialized with code: %s\n", code.c_str());
        client.setInsecure(); // For testing only
    }
    
    bool connect() {
        Serial.printf("Connecting to %s:%d\n", XIAOZHI_SERVER, XIAOZHI_PORT);
        
        if (client.connect(XIAOZHI_SERVER, XIAOZHI_PORT)) {
            Serial.println("Connected to Xiaozhi server");
            
            // Send registration request
            String request = "POST /api/register HTTP/1.1\r\n";
            request += "Host: ";
            request += XIAOZHI_SERVER;
            request += "\r\nConnection: close\r\nContent-Type: application/json\r\n";
            
            // Create JSON payload
            StaticJsonDocument<256> doc;
            doc["device_id"] = connectionCode;
            doc["device_name"] = "SmartRobot";
            doc["device_type"] = "robot";
            
            String payload;
            serializeJson(doc, payload);
            
            request += "Content-Length: ";
            request += payload.length();
            request += "\r\n\r\n";
            request += payload;
            
            client.print(request);
            
            connected = true;
            return true;
        } else {
            Serial.println("Failed to connect to Xiaozhi server");
            connected = false;
            return false;
        }
    }
    
    bool hasCommand() {
        if (client.available()) {
            String response = "";
            while (client.available()) {
                response += (char)client.read();
            }
            
            // Parse command from response
            if (response.length() > 0) {
                parseCommand(response);
                return hasNewCommand;
            }
        }
        return false;
    }
    
    String getCommand() {
        hasNewCommand = false;
        return lastCommand;
    }
    
    void parseCommand(String response) {
        // Simple JSON parsing
        int commandStart = response.indexOf("\"command\":");
        if (commandStart != -1) {
            int quoteStart = response.indexOf('"', commandStart + 10);
            int quoteEnd = response.indexOf('"', quoteStart + 1);
            if (quoteStart != -1 && quoteEnd != -1) {
                lastCommand = response.substring(quoteStart + 1, quoteEnd);
                hasNewCommand = true;
                Serial.printf("Received command: %s\n", lastCommand.c_str());
            }
        }
    }
    
    void sendAudio(String audioData) {
        if (!connected) return;
        
        StaticJsonDocument<512> doc;
        doc["type"] = "audio";
        doc["device_id"] = connectionCode;
        doc["data"] = audioData;
        
        String payload;
        serializeJson(doc, payload);
        
        String request = "POST /api/audio HTTP/1.1\r\n";
        request += "Host: ";
        request += XIAOZHI_SERVER;
        request += "\r\nConnection: close\r\nContent-Type: application/json\r\n";
        request += "Content-Length: ";
        request += payload.length();
        request += "\r\n\r\n";
        request += payload;
        
        client.print(request);
        
        Serial.println("Audio data sent to Xiaozhi");
    }
    
    void sendStatus(String status) {
        if (!connected) return;
        
        StaticJsonDocument<256> doc;
        doc["type"] = "status";
        doc["device_id"] = connectionCode;
        doc["status"] = status;
        
        String payload;
        serializeJson(doc, payload);
        
        Serial.printf("Status sent: %s\n", status.c_str());
    }
    
    void disconnect() {
        if (client.connected()) {
            client.stop();
            connected = false;
            Serial.println("Disconnected from Xiaozhi");
        }
    }
};

#endif
