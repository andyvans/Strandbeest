# Strandbeest

ESP32-CAM controlled Strandbeest robot with live video streaming and a browser-based remote control.

## Features

- WiFi access point (`ESP32_Robot` / `12345678`)
- MJPEG camera stream at `/stream`
- Motor control over WebSocket at `/ws`
- Web UI served from SPIFFS (`data/index.html`)

## Hardware

- ESP32-CAM (AI-Thinker, with PSRAM)
- Dual H-bridge motor driver on GPIO 12, 13, 14, 15
- Onboard flash LED on GPIO 4

Pin assignments are in [src/constants.h](src/constants.h).

## Build & Flash

Requires [PlatformIO](https://platformio.org/).

```sh
pio run -t upload       # build and flash firmware
pio run -t uploadfs     # upload web UI (data/) to SPIFFS
pio device monitor      # serial monitor at 115200 baud
```

## Usage

1. Power on the robot.
2. Connect to the WiFi network `ESP32_Robot`.
3. Open `http://192.168.4.1` in a browser.
