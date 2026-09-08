#ifndef CONFIG_H
#define CONFIG_H

// WiFi Configuration
#define HOTSPOT_SSID "SmartRobot_Setup"
#define HOTSPOT_PASSWORD "12345678"
#define XIAOZHI_SERVER "api.xiaozhi.me"
#define XIAOZHI_PORT 443

// GPIO Pins - ESP32-S3
// I2C Display (OLED 0.96")
#define I2C_SDA 8
#define I2C_SCL 9
#define OLED_ADDRESS 0x3C

// Audio - I2S DAC (MAX98357)
#define I2S_DOUT 13   // Data OUT
#define I2S_BCLK 12   // Bit Clock
#define I2S_LRC 11    // Left/Right Clock

// Audio - I2S ADC (INMP441)
#define I2S_MIC_DIN 3   // Data IN from microphone
#define I2S_MIC_BCLK 2  // Bit Clock
#define I2S_MIC_WS 1    // Word Select

// LED Indicator (3V)
#define LED_PIN 10

// Motor Control (L298N Driver)
#define MOTOR1_IN1 46
#define MOTOR1_IN2 45
#define MOTOR1_EN 44

#define MOTOR2_IN1 43
#define MOTOR2_IN2 42
#define MOTOR2_EN 41

// I2S Ports
#define I2S_PORT_SPEAKER I2S_NUM_0
#define I2S_PORT_MIC I2S_NUM_1

// Audio Settings
#define SAMPLE_RATE 16000
#define BITS_PER_SAMPLE 16
#define CHANNELS 1

// Display Settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// Motor Speed
#define MOTOR_MAX_SPEED 255
#define MOTOR_MIN_SPEED 100

#endif
