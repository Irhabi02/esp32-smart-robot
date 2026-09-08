#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <Arduino.h>
#include <driver/i2s.h>
#include "config.h"
#include "tts_engine.h"

class AudioManager {
private:
    bool listeningMode = false;
    TTSEngine tts;
    uint8_t i2sBuffer[4096];
    size_t bytesRead = 0;
    
public:
    void begin() {
        Serial.println("Initializing Audio Manager...");
        
        // Initialize I2S for speaker (MAX98357)
        initI2SSpeaker();
        
        // Initialize I2S for microphone (INMP441)
        initI2SMicrophone();
        
        // Initialize TTS Engine
        tts.begin();
        
        Serial.println("Audio Manager initialized");
    }
    
    void initI2SSpeaker() {
        i2s_config_t i2s_config = {
            .mode = I2S_MODE_MASTER | I2S_MODE_TX,
            .sample_rate = SAMPLE_RATE,
            .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
            .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
            .communication_format = I2S_COMM_FORMAT_STAND_I2S,
            .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
            .dma_buf_count = 4,
            .dma_buf_len = 1024,
            .use_apll = false,
            .tx_desc_auto_clear = true,
            .fixed_mclk = -1
        };
        
        i2s_pin_config_t pin_config = {
            .bck_io_num = I2S_BCLK,
            .ws_io_num = I2S_LRC,
            .data_out_num = I2S_DOUT,
            .data_in_num = I2S_PIN_NO_CHANGE,
            .mck_io_num = I2S_PIN_NO_CHANGE
        };
        
        i2s_driver_install(I2S_PORT_SPEAKER, &i2s_config, 0, NULL);
        i2s_set_pin(I2S_PORT_SPEAKER, &pin_config);
        
        Serial.println("I2S Speaker initialized");
    }
    
    void initI2SMicrophone() {
        i2s_config_t i2s_config = {
            .mode = I2S_MODE_MASTER | I2S_MODE_RX,
            .sample_rate = SAMPLE_RATE,
            .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
            .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
            .communication_format = I2S_COMM_FORMAT_STAND_I2S,
            .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
            .dma_buf_count = 4,
            .dma_buf_len = 1024,
            .use_apll = false,
            .tx_desc_auto_clear = false,
            .fixed_mclk = -1
        };
        
        i2s_pin_config_t pin_config = {
            .bck_io_num = I2S_MIC_BCLK,
            .ws_io_num = I2S_MIC_WS,
            .data_out_num = I2S_PIN_NO_CHANGE,
            .data_in_num = I2S_MIC_DIN,
            .mck_io_num = I2S_PIN_NO_CHANGE
        };
        
        i2s_driver_install(I2S_PORT_MIC, &i2s_config, 0, NULL);
        i2s_set_pin(I2S_PORT_MIC, &pin_config);
        
        Serial.println("I2S Microphone initialized");
    }
    
    void speak(const char* text) {
        Serial.printf("Speaking: %s\n", text);
        
        // Generate audio from text
        uint8_t* audioData = tts.synthesize(text);
        size_t audioLength = tts.getAudioLength();
        
        // Play audio
        playAudio(audioData, audioLength);
        
        free(audioData);
    }
    
    void speakNumber(String number) {
        Serial.printf("Speaking number: %s\n", number.c_str());
        
        for (int i = 0; i < number.length(); i++) {
            char digit = number[i];
            speak(&digit);
            delay(200); // Pause between digits
        }
    }
    
    void playAudio(uint8_t* audioData, size_t length) {
        size_t bytesWritten = 0;
        i2s_write(I2S_PORT_SPEAKER, audioData, length, &bytesWritten, portMAX_DELAY);
        Serial.printf("Played %d bytes\n", bytesWritten);
    }
    
    void playConnectingSound() {
        // Generate connecting beep sound
        generateTone(1000, 200); // 1kHz for 200ms
        delay(100);
        generateTone(1000, 200);
    }
    
    void playSuccessSound() {
        // Two ascending tones
        generateTone(800, 150);
        delay(50);
        generateTone(1200, 150);
    }
    
    void playHappySound() {
        generateTone(1000, 100);
        delay(50);
        generateTone(1200, 100);
        delay(50);
        generateTone(1400, 100);
    }
    
    void playSadSound() {
        generateTone(600, 200);
        delay(50);
        generateTone(400, 200);
    }
    
    void playAngrySound() {
        for (int i = 0; i < 3; i++) {
            generateTone(800, 100);
            delay(50);
        }
    }
    
    void generateTone(uint16_t frequency, uint16_t duration) {
        // Generate sine wave tone
        uint32_t samples = (SAMPLE_RATE * duration) / 1000;
        uint8_t buffer[samples * 2];
        
        for (uint32_t i = 0; i < samples; i++) {
            // Generate sine wave
            int16_t sample = (int16_t)(10000 * sin(2.0 * PI * frequency * i / SAMPLE_RATE));
            buffer[i * 2] = sample & 0xFF;
            buffer[i * 2 + 1] = (sample >> 8) & 0xFF;
        }
        
        size_t bytesWritten = 0;
        i2s_write(I2S_PORT_SPEAKER, buffer, samples * 2, &bytesWritten, portMAX_DELAY);
    }
    
    bool isListening() {
        return listeningMode;
    }
    
    void startListening() {
        listeningMode = true;
        Serial.println("Listening...");
    }
    
    void stopListening() {
        listeningMode = false;
        Serial.println("Stop listening");
    }
    
    String captureAudio() {
        if (!listeningMode) return "";
        
        memset(i2sBuffer, 0, sizeof(i2sBuffer));
        i2s_read(I2S_PORT_MIC, i2sBuffer, sizeof(i2sBuffer), &bytesRead, portMAX_DELAY);
        
        // Convert audio buffer to string (simplified)
        String audioData = "";
        for (int i = 0; i < bytesRead && i < 100; i++) {
            audioData += String(i2sBuffer[i], HEX);
        }
        
        return audioData;
    }
};

#endif
