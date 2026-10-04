# Live Dashboard

**Author:** suriyakumar P

## Task

Interface an HC-SR04 ultrasonic sensor and display live distance readings on a self-refreshing ESP32 web page.

## Components

- ESP32 DevKit V4
- HC-SR04 ultrasonic distance sensor

## Connections

| HC-SR04 | ESP32 |
|---|---|
| VCC | 5V |
| TRIG | GPIO 5 |
| ECHO | GPIO 18 |
| GND | GND |

## Working

The ESP32 connects to the Wokwi WiFi network and starts a web server.

The HC-SR04 measures the distance continuously. Open the ESP32 web server address in the Wokwi browser/serial output to view the live dashboard.

The dashboard automatically requests the latest distance reading every 1 second without manually refreshing the page.

## Serial Monitor

Set the Serial Monitor baud rate to **115200**.

The ESP32 prints its IP address and the current distance reading.

## Wokwi

This project is designed to run in the Wokwi ESP32 simulator.
