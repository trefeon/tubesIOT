# IoT-Based Flood Detection System

![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)

## 📝 Description

This project implements an IoT-based flood detection system using ESP32 microcontroller, equipped with water level sensors and real-time notification capabilities. The system helps in early flood detection and warning dissemination.

## 🌟 Features

- Real-time water level monitoring
- MQTT-based communication
- Web dashboard interface
- Automated alert system
- ESP32 microcontroller integration

## 🛠️ Hardware Requirements

- ESP32 Development Board
- Water Level Sensors
- Power Supply Unit
- Connection cables
- Internet connectivity

## 💻 Software Requirements

- Arduino IDE
- Required Libraries:
  - PubSubClient (MQTT)
  - ESP32 Board Support
  - WiFi
- Web browser for dashboard access

## 📦 Project Structure

```
tubesIOT/
├── ESP32/                 # ESP32 firmware
│   └── ESP32.ino         # Main Arduino sketch
├── web/                   # Web dashboard
│   ├── css/              # Styling files
│   ├── js/               # JavaScript files
│   └── index.html        # Main dashboard
└── docs/                 # Documentation
```

## 🚀 Setup Instructions

1. Clone this repository
2. Upload ESP32.ino to your ESP32 board
3. Configure WiFi credentials
4. Set up MQTT broker
5. Open the web dashboard

## 📊 System Architecture

The system consists of:
- ESP32 with sensors for data collection
- MQTT broker for message handling
- Web interface for monitoring
- Alert system for notifications

## 🔧 Configuration

1. WiFi Setup:
   - Update SSID and password in the code
2. MQTT Settings:
   - Configure broker address
   - Set up topics
3. Sensor Calibration:
   - Adjust threshold values
   - Set sampling rate

## 👥 Contributors

- Team 4
- Student ID: 1103210041

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 📞 Contact

For any queries regarding this project, please contact the team members listed in the documentation.
