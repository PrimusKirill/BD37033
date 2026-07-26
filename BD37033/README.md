# BD37033 Library for Arduino/ESP32

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Library for controlling **BD37033FV-M** audio processor from Rohm Semiconductor via I2C bus.

---

## 🎯 Purpose

This library provides a simple and intuitive interface for the BD37033FV-M.  
It abstracts away the complexity of register manipulation and allows you to control the processor with easy-to-use functions.

---

## 📦 Compatibility

| Platform   | Status |
|------------|--------|
| ESP32      | ✅ Full support |
| Arduino Uno/Nano/Mega | ✅ Full support |
| ESP8266    | ⚠️ Needs testing (I2C works) |

---

## 🔌 Wiring

I2C connection (only 4 wires):

| BD37033 Pin | Board (Arduino/ESP32) |
|-------------|------------------------|
| `SDA`       | SDA (A4 or GPIO21)    |
| `SCL`       | SCL (A5 or GPIO22)    |
| `VDD`       | 3.3V or 5V            |
| `GND`       | GND                   |

> ⚠️ **Important:** Pull up SDA and SCL lines with 2.2–10 kΩ resistors to VCC if not already pulled up on your board.

---

## 🚀 Quick Start

```cpp
#include <BD37033.h>

void setup() {
    Serial.begin(115200);

    if (BD37033_init()) {
        Serial.println("BD37033 found!");
    } else {
        Serial.println("BD37033 NOT found!");
        while(1);
    }

    // Select input A
    BD37033_setInput(BD37033_INPUT_A);
    
    // Set volume to 30 (0–54 scale)
    BD37033_setVolume(30);
    
    // Boost bass by +5 dB
    BD37033_setBass(5);
    
    // Enable Loudness
    BD37033_setLoudness(true);
}

void loop() {
    // Your code...
}