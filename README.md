# Smart Robot - ESP32-S3 with Xiaozhi.me Integration

## Overview
Smart robot yang dapat berinteraksi dengan platform Xiaozhi.me. Robot ini dilengkapi dengan:
- WiFi connectivity
- Voice interaction (text-to-speech & speech-to-text)
- Expressive OLED display dengan animasi wajah
- Motor control untuk movement
- LED indicator untuk feedback visual

## Hardware Components

### Main Controller
- **ESP32-S3 I16N8** - Microcontroller dengan WiFi/BLE

### Display & Feedback
- **OLED Display 0.96 inch** - Tampilan animasi wajah robot
- **LED 3V** - Indikator status (berkedip saat berbicara)

### Audio I/O
- **MAX98357 I2S DAC** - Audio output driver
- **Speaker 3W** - Output suara
- **INMP441 I2S Microphone** - Audio input untuk listening

### Motor Control
- **L298N Motor Driver** - PWM control untuk motor DC
- **2x DC Motors** - Penggerak robot

## Pin Configuration

```
I2C Display:
- SDA: GPIO 8
- SCL: GPIO 9
- Address: 0x3C

I2S Speaker (MAX98357):
- DOUT: GPIO 13
- BCLK: GPIO 12
- LRC: GPIO 11

I2S Microphone (INMP441):
- DIN: GPIO 3
- BCLK: GPIO 2
- WS: GPIO 1

LED Indicator:
- LED_PIN: GPIO 10

Motor Control (L298N):
- Motor 1: IN1=GPIO46, IN2=GPIO45, EN=GPIO44
- Motor 2: IN1=GPIO43, IN2=GPIO42, EN=GPIO41
```

## Features

### 1. WiFi Setup Mode
- ESP32 membuat hotspot saat pertama kali dinyalakan
- Robot berbicara: "Please connect to wifi"
- Display menampilkan instruksi koneksi

### 2. Connection Code Generation
- Setelah terhubung ke WiFi, robot menampilkan 6-digit code
- Robot mengucapkan code tersebut untuk digunakan di xiaozhi.me
- Display menunjukkan code dan instruksi: "Visit: xiaozhi.me"

### 3. Expressive Animation
- **Happy Face**: Mata bulat, mulut tersenyum
- **Sad Face**: Mata bulat, mulut menunduk
- **Angry Face**: Mata slanted, mulut lurus
- **Confused Face**: Mata dengan titik, mulut tanda tanya
- **Talking Animation**: Mulut yang bergerak saat berbicara
- **Moving Animation**: Robot bergerak

### 4. Voice Interaction
- Text-to-Speech (TTS) untuk output suara
- Microphone untuk input suara
- LED berkedip saat robot berbicara
- Integration dengan Xiaozhi.me API

### 5. Motor Control
- Gerakan maju, mundur, belok kiri, belok kanan
- Kontrol kecepatan via PWM
- Dapat dikendalikan dari Xiaozhi.me

## Instalasi & Setup

### Prerequisites
- VSCode dengan PlatformIO
- Arduino IDE (opsional)
- PlatformIO CLI

### Langkah-langkah:

1. Clone repository ini:
```bash
git clone https://github.com/Irhabi02/esp32-smart-robot.git
cd esp32-smart-robot
```

2. Install dependencies:
```bash
pio lib install
```

3. Upload ke ESP32-S3:
```bash
pio run -t upload
```

4. Monitor serial output:
```bash
pio device monitor
```

## Workflow

### First Time Setup
1. Sambungkan ESP32 ke komputer via USB
2. Upload kode
3. Monitor Serial Console untuk melihat progress
4. Robot akan membuat hotspot "SmartRobot_Setup"
5. Hubungkan smartphone/laptop ke hotspot tersebut
6. Robot akan menampilkan kode 6 digit dan mengucapkannya
7. Kunjungi xiaozhi.me dan masukkan kode tersebut

### Normal Operation
1. Robot terhubung ke WiFi
2. Tunggu command dari Xiaozhi.me
3. Robot akan merespons dengan gerakan, suara, dan animasi
4. LED berkedip saat robot berbicara
5. Display menampilkan emosi/status robot

## Perintah yang Didukung

### Motor Commands
```json
{"command": "move_forward", "speed": 200}
{"command": "move_backward", "speed": 200}
{"command": "turn_left", "speed": 200}
{"command": "turn_right", "speed": 200}
{"command": "stop"}
```

### Audio Commands
```json
{"command": "speak", "text": "Hello World"}
{"command": "listen"}
```

### Animation Commands
```json
{"command": "emotion", "type": "happy"}
{"command": "emotion", "type": "sad"}
{"command": "emotion", "type": "angry"}
{"command": "emotion", "type": "confused"}
```

## Project Structure

```
esp32-smart-robot/
├── src/
│   ├── main.cpp              # Main program
│   ├── config.h              # Pin configuration
│   ├── wifi_manager.h        # WiFi connectivity
│   ├── display_manager.h     # OLED display control
│   ├── audio_manager.h       # Audio I/O (TTS & speech)
│   ├── motor_manager.h       # Motor control
│   ├── led_manager.h         # LED indicator
│   ├── xiaozhi_client.h      # Xiaozhi.me API client
│   └── tts_engine.h          # Text-to-Speech engine
├── platformio.ini            # PlatformIO configuration
└── README.md                 # Documentation
```

## Troubleshooting

### Robot tidak berbunyi
- Cek koneksi MAX98357 dan speaker
- Verify pin I2S configuration
- Check volume level di audio_manager.h

### Display tidak menampilkan animasi
- Cek koneksi I2C (SDA, SCL)
- Verify I2C address (0x3C)
- Cek power supply ke OLED

### Motor tidak bergerak
- Cek koneksi L298N driver
- Verify GPIO pin configuration
- Test motor dengan multimeter

### WiFi tidak connect
- Robot akan membuat hotspot jika tidak menemukan WiFi
- Hubungkan ke "SmartRobot_Setup"
- Atau sesuaikan SSID dan password di config.h

### Tidak terhubung ke Xiaozhi.me
- Pastikan WiFi sudah connect
- Verify connection code yang ditampilkan
- Cek API endpoint di config.h

## Future Enhancements

- [ ] Cloud-based TTS integration
- [ ] Advanced speech recognition
- [ ] Object detection dengan camera
- [ ] Cloud storage untuk learning
- [ ] Mobile app untuk kontrol
- [ ] Battery management system
- [ ] OTA (Over-The-Air) updates
- [ ] Machine learning untuk behavior

## License
MIT License - Feel free to use and modify

## Support
Untuk pertanyaan atau issues, silakan buat GitHub issue atau hubungi author.

## Author
Developed for Smart Robot Project
