# IoT Project: Konsentrator dan Jembatan (ESP32 + ESP8266)

![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)

## 📝 Description

This project implements an IoT-based concentrator and bridge system using ESP32 and ESP8266 microcontrollers. The system demonstrates the communication and data handling capabilities between ESP32 (as concentrator) and ESP8266 (as bridge) devices through MQTT protocol.

## 🌟 Features

- ESP32 as Concentrator node
- ESP8266 as Bridge node
- MQTT-based communication
- Web dashboard interface
- Real-time data monitoring
- Inter-device communication

## 🛠️ Hardware Requirements

- ESP32 Development Board
- ESP8266 Development Board
- Power Supply Units
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

```plaintext
tubesIOT/
├── ESP32TUBESIOT/        # ESP32 Concentrator firmware
│   └── ESP32TUBESIOT.ino # Main ESP32 sketch
├── ESP8266TUBESIOT/      # ESP8266 Bridge firmware
│   └── ESP8266TUBESIOT.ino # Main ESP8266 sketch
├── iot/                  # Web application
│   ├── app_api.py       # Backend API
│   └── templates/       # Frontend templates
│       └── dashboard.html # Dashboard interface
└── mqtt_dashboard.html   # MQTT monitoring dashboard
```

## 🚀 Setup Instructions

1. Clone this repository
2. Upload ESP32.ino to your ESP32 board
3. Configure WiFi credentials
4. Set up MQTT broker
5. Open the web dashboard

## 📊 System Architecture

The system consists of:

- ESP32 acting as a concentrator node for data processing
- ESP8266 serving as a bridge for data transmission
- MQTT broker for message handling and routing
- Web interface for system monitoring and control
- Python-based backend for data processing

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
