/*
 * Basic.ino
 * BASIC EXAMPLE FOR BD37033 LIBRARY
 * 
 * Wiring:
 *   BD37033 SDA → Arduino/ESP32 SDA (A4 / GPIO21)
 *   BD37033 SCL → Arduino/ESP32 SCL (A5 / GPIO22)
 *   BD37033 VDD → 3.3V or 5V
 *   BD37033 GND → GND
 * 
 * Author: Kirill Primus
 * License: MIT License
 */

#include <BD37033.h>

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n==========================================");
    Serial.println("  BD37033 Library Example");
    Serial.println("==========================================\n");

    // ---- 1. INITIALIZATION ----
    if (BD37033_init()) {
        Serial.println("✅ BD37033 found!");
    } else {
        Serial.println("❌ BD37033 NOT found!");
        Serial.println("   Check wiring and power.");
        while (1) {
            delay(1000);
        }
    }

    // ---- 2. INPUT SELECTION ----
    BD37033_setInput(BD37033_INPUT_A);
    Serial.println("   Input: A");

    // ---- 3. VOLUME ----
    BD37033_setVolume(30);
    Serial.println("   Volume: 30 (-49 dB)");

    // ---- 4. TONE CONTROLS ----
    BD37033_setBass(5);      // +5 dB
    BD37033_setMiddle(0);    // 0 dB
    BD37033_setTreble(-3);   // -3 dB
    Serial.println("   Bass: +5 dB, Middle: 0 dB, Treble: -3 dB");

    // ---- 5. LOUDNESS ----
    BD37033_setLoudness(true);
    BD37033_setLoudnessGain(10);
    BD37033_setLoudnessHiCut(BD37033_HICUT_2_4KHZ);
    Serial.println("   Loudness: ON, Gain: 10 dB, HiCut: 2.4 kHz");

    // ---- 6. BALANCE ----
    BD37033_setBalance(0);
    Serial.println("   Balance: 0 (center)");

    // ---- 7. MUTE ----
    BD37033_setMute(false);
    Serial.println("   Mute: OFF");

    Serial.println("\n✅ Setup complete! Sound is on.");
    Serial.println("==========================================\n");
}

void loop() {
    // Smooth volume sweep example
    static int direction = 1;
    static int volume = 20;

    volume += direction;
    if (volume >= 50) direction = -1;
    if (volume <= 5) direction = 1;

    BD37033_setVolume(volume);
    Serial.printf("Volume: %d\n", volume);

    delay(1000);
}