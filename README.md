# BD37033 Library for Arduino/ESP32

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Library for controlling **BD37033FV-M** audio processor from Rohm Semiconductor via I2C.

---

## 🎯 Purpose

This library provides a simple and intuitive interface for the BD37033FV-M.  
It abstracts away the complexity of register manipulation and allows you to control the processor with easy-to-use functions.

---

## 📦 Compatibility

| Platform              |    Status |
|-----------------------|------------------------------|
| ESP32                 | ✅ Full support              |
| Arduino Uno/Nano/Mega | ✅ Full support              |
| ESP8266               | ⚠️ Needs testing (I2C works) |

---

## 🔌 Wiring

I2C connection (only 4 wires):

| BD37033 Pin | Board (Arduino/ESP32) |
|-------------|-----------------------|
| `SDA` | SDA (A4 or GPIO21) |
| `SCL` | SCL (A5 or GPIO22) |
| `VDD` | 3.3V or 5V |
| `GND` | GND |

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

    // ★★★ MINIMUM SETTINGS TO GET SOUND ★★★
    BD37033_fullReset();                // Reset all registers
    BD37033_setInput(BD37033_INPUT_A);  // Select input A
    BD37033_setVolume(-20);             // Volume -20 dB (comfortable level)
    BD37033_setMute(false);             // Disable Mute (turn sound on)

    // Optional: set tone controls to neutral
    BD37033_setBass(0);
    BD37033_setMiddle(0);
    BD37033_setTreble(0);
}

void loop() {
    // Your code...
}



