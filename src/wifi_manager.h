#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

class WiFiManager {
private:
    bool connected = false;
    String ssid;
    String password;
    
public:
    void createHotspot(const char* ap_ssid, const char* ap_password) {
        Serial.printf("Creating hotspot: %s\n", ap_ssid);
        WiFi.mode(WIFI_AP);
        WiFi.softAP(ap_ssid, ap_password);
        
        IPAddress IP = WiFi.softAPIP();
        Serial.print("AP IP address: ");
        Serial.println(IP);
    }
    
    void connectToWiFi(const char* wifi_ssid, const char* wifi_password) {
        ssid = wifi_ssid;
        password = wifi_password;
        
        Serial.printf("Connecting to WiFi: %s\n", wifi_ssid);
        WiFi.mode(WIFI_STA);
        WiFi.begin(wifi_ssid, wifi_password);
        
        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < 20) {
            delay(500);
            Serial.print(".");
            attempts++;
        }
        
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\nWiFi connected!");
            Serial.print("IP: ");
            Serial.println(WiFi.localIP());
            connected = true;
        } else {
            Serial.println("\nFailed to connect to WiFi");
            connected = false;
        }
    }
    
    bool isConnected() {
        return WiFi.status() == WL_CONNECTED;
    }
    
    String getLocalIP() {
        return WiFi.localIP().toString();
    }
    
    String getSSID() {
        return WiFi.SSID();
    }
};

#endif
