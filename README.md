# BD37033 Library for Arduino/ESP32

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Library for controlling **BD37033FV-M** audio processor from Rohm Semiconductor via I2C.

---

## 🎯 Purpose

This library provides a simple and intuitive interface for the BD37033FV-M.  
It abstracts away the complexity of register manipulation and allows you to control the processor with easy-to-use functions.

---

## 📦 Compatibility

| Platform | Status |
|----------|--------|
| ESP32 | ✅ Full support |
| Arduino Uno/Nano/Mega | ✅ Full support |
| ESP8266 | ⚠️ Needs testing (I2C works) |

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
📚 Examples
The library includes two ready-to-use examples:

Example	Description
BD37033_Console	Control BD37033 via Serial Monitor (UART). Full command set: volume, input, tone controls, balance, loudness, mute, subwoofer, and advanced tone settings (F0/Q).
Demo_Cycle	Automated demo cycling through all functions. Hear the changes in real-time.
📖 Full Function List
Initialization
Function	Description
bool BD37033_init(addr = 0x40, sda = -1, scl = -1)	Initialize I2C and BD37033. Returns true if chip found.
bool BD37033_isConnected()	Check if chip is present on the bus.
void BD37033_fullReset()	Reset all registers to default values.
Core Functions
Function	Description	Range
void BD37033_setVolume(db)	Master volume	-79..+15 dB
void BD37033_setInput(input)	Select input (with click protection)	0=A, 1=B, 2=C, 3=D, 4=E
void BD37033_setInputDirect(input)	Select input (without protection)	0=A, 1=B, 2=C, 3=D, 4=E
void BD37033_setMute(on, gain)	Enable/disable mute	0/1, Input Gain 0..16
void BD37033_setBalance(bal)	Balance (negative = left louder)	-10..+10
void BD37033_setFader(value)	Front/Rear fader	-100..+100
Tone Controls
Function	Description	Range
void BD37033_setBass(db)	Bass level	-15..+15 dB
void BD37033_setMiddle(db)	Middle level	-15..+15 dB
void BD37033_setTreble(db)	Treble level	-15..+15 dB
Advanced Tone Settings
Function	Description	Range
void BD37033_setBassF0(f0)	Bass cutoff frequency	0=60Hz, 1=80Hz, 2=100Hz, 3=120Hz
void BD37033_setBassQ(q)	Bass Q factor	0=1.0, 1=1.5, 2=2.0
void BD37033_setMidF0(f0)	Middle cutoff frequency	0=500Hz, 1=1kHz, 2=1.5kHz, 3=2.5kHz
void BD37033_setMidQ(q)	Middle Q factor	0=0.75, 1=1.0, 2=1.25
void BD37033_setTrebleF0(f0)	Treble cutoff frequency	0=7.5kHz, 1=10kHz, 2=12.5kHz, 3=15kHz
void BD37033_setTrebleQ(q)	Treble Q factor	0=0.75, 1=1.25
Loudness
Function	Description	Range
void BD37033_setLoudness(on)	Enable/disable Loudness	true/false
void BD37033_setLoudnessGain(gain)	Loudness gain	0..20 dB
void BD37033_setLoudnessHiCut(cut)	HiCut frequency	0=1.2kHz, 1=2.4kHz, 2=4.0kHz
Inputs
Function	Description	Range
void BD37033_setInputGain(gain)	Input signal gain	0..16 dB
void BD37033_setInputType(diff, input)	Input type	true=Differential, false=Single-ended
Subwoofer
Function	Description	Range
void BD37033_setSubLPF(fc)	LPF cutoff frequency	0=OFF, 1=55Hz, 2=85Hz, 3=120Hz, 4=160Hz
void BD37033_setSubPhase(invert)	Phase inversion	true=180°, false=0°
void BD37033_setSubInput(sel)	Subwoofer source	0=Fader, 1=Front+Rear
void BD37033_setSubOutput(sel)	Subwoofer output	0=OUTS, 1=MIX
void BD37033_setSubwooferSource(sel)	Extended source selection	0=LPF, 1=Front, 2=Rear, 3=Sub
Mixer
Function	Description	Range
void BD37033_setMixingGain(gain)	Mixer level	-79..+15 dB
void BD37033_setMixingInput(input)	Mixer input select	0=A, 1=B, 2=C, 3=D, 4=E
Additional Functions
Function	Description
void BD37033_setAdvancedSwitch(value)	Direct write to advanced register (0x01)
void BD37033_setChannelVolume(ch, db)	Individual channel volume (0..4, -79..+15 dB)
Diagnostics
Function	Description
uint8_t BD37033_readRegister(reg)	Read any register (returns 0xFF if not supported)
void BD37033_printRegisters()	Print all registers to Serial
⚠️ Important: BD37033 does NOT support reading registers. readRegister() always returns 0xFF. This is normal behavior for this chip.

🐞 Debugging
By default, the library outputs debug messages to Serial.

To disable logging, change in BD37033.h:

cpp
#define BD37033_DEBUG 0   // 0 — logs disabled
📜 License
MIT License — free to use, modify, and distribute.
See LICENSE for details.

✍️ Author
Kirill Primus
GitHub: PrimusKirill

⭐ If this library was helpful — give it a star on GitHub!
