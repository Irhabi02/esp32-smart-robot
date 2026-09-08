#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include "config.h"
#include "wifi_manager.h"
#include "display_manager.h"
#include "audio_manager.h"
#include "motor_manager.h"
#include "xiaozhi_client.h"
#include "led_manager.h"

// Global instances
WiFiManager wifiManager;
DisplayManager display;
AudioManager audio;
MotorManager motors;
Xiaozhi xiaozhi;
LEDManager ledIndicator;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n=== Smart Robot Initialization ===");
  
  // Initialize components
  ledIndicator.begin();
  display.begin();
  audio.begin();
  motors.begin();
  
  // Display startup animation
  display.showStartupAnimation();
  delay(2000);
  
  // Start WiFi setup
  Serial.println("Starting WiFi setup...");
  display.showWiFiSetup();
  audio.speak("Please connect to wifi");
  
  // Create hotspot
  wifiManager.createHotspot(HOTSPOT_SSID, HOTSPOT_PASSWORD);
  
  // Wait for WiFi connection
  display.showWaitingForConnection();
  audio.playConnectingSound();
  
  int timeout = 60; // 60 seconds timeout
  while (!wifiManager.isConnected() && timeout > 0) {
    delay(1000);
    timeout--;
    Serial.printf("Waiting... %d seconds\n", timeout);
  }
  
  if (wifiManager.isConnected()) {
    handleWiFiConnected();
  } else {
    handleWiFiTimeout();
  }
}

void handleWiFiConnected() {
  Serial.println("WiFi Connected!");
  display.showConnectedAnimation();
  audio.playSuccessSound();
  
  // Generate 6-digit connection code
  String connectionCode = generateConnectionCode();
  
  // Display code and play it
  display.showConnectionCode(connectionCode);
  audio.speakNumber(connectionCode);
  audio.speak("to connect to xiaozhi dot me");
  
  // Show happy face animation
  display.showHappyFace();
  
  // Initialize Xiaozhi connection
  xiaozhi.begin(connectionCode);
  
  delay(3000);
}

void handleWiFiTimeout() {
  Serial.println("WiFi connection timeout");
  display.showSadFace();
  audio.speak("Connection failed, please try again");
  delay(2000);
  ESP.restart();
}

String generateConnectionCode() {
  String code = "";
  for (int i = 0; i < 6; i++) {
    code += String(random(0, 10));
  }
  return code;
}

void loop() {
  // Update WiFi status
  if (!wifiManager.isConnected()) {
    handleWiFiTimeout();
  }
  
  // Check for incoming commands from xiaozhi.me
  if (xiaozhi.hasCommand()) {
    String command = xiaozhi.getCommand();
    handleXiaozhiCommand(command);
  }
  
  // Listen for voice input
  if (audio.isListening()) {
    String audioData = audio.captureAudio();
    if (audioData.length() > 0) {
      ledIndicator.blink(); // LED blinks while speaking
      // Process audio
      xiaozhi.sendAudio(audioData);
    }
  }
  
  delay(100);
}

void handleXiaozhiCommand(String command) {
  Serial.printf("Xiaozhi Command: %s\n", command.c_str());
  
  if (command.indexOf("move_forward") >= 0) {
    motors.moveForward();
    display.showMovingAnimation();
  }
  else if (command.indexOf("move_backward") >= 0) {
    motors.moveBackward();
  }
  else if (command.indexOf("turn_left") >= 0) {
    motors.turnLeft();
  }
  else if (command.indexOf("turn_right") >= 0) {
    motors.turnRight();
  }
  else if (command.indexOf("stop") >= 0) {
    motors.stop();
  }
  else if (command.indexOf("speak") >= 0) {
    // Extract text and speak
    String text = command.substring(command.indexOf(":")+1);
    ledIndicator.startBlinking();
    audio.speak(text);
    display.showTalkingAnimation();
    ledIndicator.stopBlinking();
  }
  else if (command.indexOf("emotion") >= 0) {
    handleEmotionCommand(command);
  }
}

void handleEmotionCommand(String command) {
  if (command.indexOf("happy") >= 0) {
    display.showHappyFace();
    audio.playHappySound();
  }
  else if (command.indexOf("sad") >= 0) {
    display.showSadFace();
    audio.playSadSound();
  }
  else if (command.indexOf("angry") >= 0) {
    display.showAngryFace();
    audio.playAngrySound();
  }
  else if (command.indexOf("confused") >= 0) {
    display.showConfusedFace();
  }
}
