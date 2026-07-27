/*
 * FullFunction.ino
 * FULL FUNCTIONALITY DEMO FOR BD37033 LIBRARY
 * 
 * This example demonstrates ALL functions of the BD37033 library with
 * audible changes so you can hear the effect of each parameter.
 * 
 * Wiring (ESP32):
 *   BD37033 SDA → GPIO27
 *   BD37033 SCL → GPIO22
 *   BD37033 VDD → 3.3V
 *   BD37033 GND → GND
 * 
 * Wiring (Arduino Uno/Nano):
 *   BD37033 SDA → A4
 *   BD37033 SCL → A5
 *   BD37033 VDD → 5V
 *   BD37033 GND → GND
 * 
 * Author: Kirill Primus
 * License: MIT License
 */

#include <BD37033.h>
#include <Wire.h>

// ==================================================
// I2C PINS (ESP32 default)
// ==================================================
#define I2C_SDA 27
#define I2C_SCL 22

// ==================================================
// GLOBAL VARIABLES
// ==================================================

int demoStep = 0;
bool demoRunning = true;
const int DEFAULT_VOLUME_DB = -30;  // -30 dB = comfortable listening level

// ==================================================
// SETUP
// ==================================================

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n==========================================");
    Serial.println("  BD37033 FULL FUNCTION DEMO");
    Serial.println("  Hear the changes as they happen!");
    Serial.println("  All values are in dB (decibels)");
    Serial.println("==========================================\n");

    // ---- I2C INIT ----
    Serial.println("[I2C] Initializing...");
    Wire.begin(I2C_SDA, I2C_SCL);
    Wire.setClock(100000);
    Serial.printf("      SDA=GPIO%d, SCL=GPIO%d\n", I2C_SDA, I2C_SCL);

    // ---- BD37033 INIT ----
    Serial.println("[BD37033] Initializing...");
    if (!BD37033_init()) {
        Serial.println("    ❌ BD37033 NOT found!");
        Serial.println("    Check wiring: SDA→GPIO27, SCL→GPIO22, VDD→3.3V, GND→GND");
        while (1) delay(1000);
    }
    Serial.println("    ✅ BD37033 found!");

    // ---- STARTUP SETTINGS ----
    Serial.println("\n[SETUP] Applying startup settings...");
    BD37033_fullReset();
    BD37033_setInput(BD37033_INPUT_A);
    BD37033_setVolume(DEFAULT_VOLUME_DB);  // -30 dB
    BD37033_setMute(false);
    Serial.printf("    ✅ Input: A, Volume: %d dB, Mute: OFF\n", DEFAULT_VOLUME_DB);
    Serial.println("    🔊 Sound should be audible now.\n");

    Serial.println("==========================================");
    Serial.println("  Starting demo in 3 seconds...");
    Serial.println("  Each step will show changes in Serial");
    Serial.println("  and you will hear the difference!");
    Serial.println("==========================================\n");
    delay(3000);
}

// ==================================================
// LOOP
// ==================================================

void loop() {
    if (!demoRunning) return;

    switch (demoStep) {
        case 0:  demoVolumeSweep(); break;
        case 1:  demoBassSweep(); break;
        case 2:  demoMiddleSweep(); break;
        case 3:  demoTrebleSweep(); break;
        case 4:  demoBalanceSweep(); break;
        case 5:  demoInputSelect(); break;
        case 6:  demoFader(); break;
        case 7:  demoSubwoofer(); break;
        case 8:  demoInputGain(); break;
        case 9:  demoAdvancedTone(); break;
        case 10: demoInputType(); break;
        default:
            demoStep = 0;
            Serial.println("\n=== 🔄 DEMO RESTARTED ===\n");
            delay(1000);
            return;
    }
    delay(1500);
}

// ==================================================
// HELPERS
// ==================================================

void restoreVolume() {
    BD37033_setVolume(DEFAULT_VOLUME_DB);
    Serial.printf("    🔈 Volume restored to %d dB\n", DEFAULT_VOLUME_DB);
}

// ==================================================
// DEMO FUNCTIONS
// ==================================================

void demoVolumeSweep() {
    Serial.println("\n--- 🔊 VOLUME SWEEP (dB) ---");
    Serial.println("    Volume will go from quiet to loud and back");
    Serial.println("    Range: -79 dB (quiet) to 0 dB (loudest)");
    
    for (int v = -79; v <= 0; v += 5) {
        BD37033_setVolume(v);
        Serial.printf("    Volume: %d dB\n", v);
        delay(400);
    }
    for (int v = -5; v >= -79; v -= 5) {
        BD37033_setVolume(v);
        Serial.printf("    Volume: %d dB\n", v);
        delay(400);
    }
    restoreVolume();
    demoStep++;
}

void demoBassSweep() {
    Serial.println("\n--- 🎵 BASS SWEEP (dB) ---");
    Serial.println("    Listen to the low frequencies change");
    Serial.println("    Range: -15 dB (cut) to +15 dB (boost)");
    
    for (int b = -15; b <= 15; b += 5) {
        BD37033_setBass(b);
        Serial.printf("    Bass: %d dB\n", b);
        delay(600);
    }
    for (int b = 10; b >= -15; b -= 5) {
        BD37033_setBass(b);
        Serial.printf("    Bass: %d dB\n", b);
        delay(600);
    }
    BD37033_setBass(0);
    Serial.println("    ✅ Bass back to 0 dB");
    restoreVolume();
    demoStep++;
}

void demoMiddleSweep() {
    Serial.println("\n--- 🎵 MIDDLE SWEEP (dB) ---");
    Serial.println("    Listen to the mid frequencies change");
    Serial.println("    Range: -15 dB (cut) to +15 dB (boost)");
    
    for (int m = -15; m <= 15; m += 5) {
        BD37033_setMiddle(m);
        Serial.printf("    Middle: %d dB\n", m);
        delay(600);
    }
    for (int m = 10; m >= -15; m -= 5) {
        BD37033_setMiddle(m);
        Serial.printf("    Middle: %d dB\n", m);
        delay(600);
    }
    BD37033_setMiddle(0);
    Serial.println("    ✅ Middle back to 0 dB");
    restoreVolume();
    demoStep++;
}

void demoTrebleSweep() {
    Serial.println("\n--- 🎵 TREBLE SWEEP (dB) ---");
    Serial.println("    Listen to the high frequencies change");
    Serial.println("    Range: -15 dB (cut) to +15 dB (boost)");
    
    for (int t = -15; t <= 15; t += 5) {
        BD37033_setTreble(t);
        Serial.printf("    Treble: %d dB\n", t);
        delay(600);
    }
    for (int t = 10; t >= -15; t -= 5) {
        BD37033_setTreble(t);
        Serial.printf("    Treble: %d dB\n", t);
        delay(600);
    }
    BD37033_setTreble(0);
    Serial.println("    ✅ Treble back to 0 dB");
    restoreVolume();
    demoStep++;
}

void demoBalanceSweep() {
    Serial.println("\n--- 🔊 BALANCE SWEEP ---");
    Serial.println("    Listen to the sound move left and right");
    Serial.println("    Range: -10 (left) to +10 (right)");
    
    for (int b = -10; b <= 10; b += 2) {
        BD37033_setBalance(b);
        Serial.printf("    Balance: %d\n", b);
        delay(500);
    }
    for (int b = 8; b >= -10; b -= 2) {
        BD37033_setBalance(b);
        Serial.printf("    Balance: %d\n", b);
        delay(500);
    }
    BD37033_setBalance(0);
    Serial.println("    ✅ Balance back to center (0)");
    restoreVolume();
    demoStep++;
}

void demoInputSelect() {
    Serial.println("\n--- 🔄 INPUT SELECTION ---");
    Serial.println("    Switching between inputs A, B, C, D, E");
    
    const char* inputs[] = {"A", "B", "C", "D", "E"};
    for (int i = 0; i < 5; i++) {
        BD37033_setInput(i);
        Serial.printf("    Input: %s\n", inputs[i]);
        delay(800);
    }
    BD37033_setInput(BD37033_INPUT_A);
    Serial.println("    ✅ Back to Input A");
    restoreVolume();
    demoStep++;
}

void demoFader() {
    Serial.println("\n--- 🔊 FADER SWEEP ---");
    Serial.println("    Front/Rear balance (if connected)");
    Serial.println("    Range: -100 (rear) to +100 (front)");
    
    for (int f = -100; f <= 100; f += 25) {
        BD37033_setFader(f);
        Serial.printf("    Fader: %d\n", f);
        delay(400);
    }
    BD37033_setFader(0);
    Serial.println("    ✅ Fader back to center (0)");
    
    // ★★★ ПРИНУДИТЕЛЬНОЕ ВОССТАНОВЛЕНИЕ ЗВУКА ★★★
    BD37033_setChannelVolume(0, 0);
    BD37033_setChannelVolume(1, 0);
    BD37033_setChannelVolume(2, 0);
    BD37033_setChannelVolume(3, 0);
    BD37033_setChannelVolume(4, 0);
    BD37033_setBalance(0);
    BD37033_setVolume(DEFAULT_VOLUME_DB);
    BD37033_setMute(false);
    Serial.printf("    🔈 Sound restored: Volume %d dB, all channels 0 dB\n", DEFAULT_VOLUME_DB);
    demoStep++;
}

void demoSubwoofer() {
    Serial.println("\n--- 🔊 SUBWOOFER LPF ---");
    Serial.println("    Changing subwoofer low-pass filter");
    
    struct {
        int val;
        const char* name;
    } lpf[] = {
        {BD37033_SUB_OFF, "OFF"},
        {BD37033_SUB_55HZ, "55 Hz"},
        {BD37033_SUB_85HZ, "85 Hz"},
        {BD37033_SUB_120HZ, "120 Hz"},
        {BD37033_SUB_160HZ, "160 Hz"}
    };
    
    for (int i = 0; i < 5; i++) {
        BD37033_setSubLPF(lpf[i].val);
        Serial.printf("    Sub LPF: %s\n", lpf[i].name);
        delay(600);
    }
    BD37033_setSubLPF(BD37033_SUB_OFF);
    Serial.println("    ✅ Sub LPF back to OFF");
    restoreVolume();
    demoStep++;
}

void demoInputGain() {
    Serial.println("\n--- 🔊 INPUT GAIN (dB) ---");
    Serial.println("    Changes input signal level");
    Serial.println("    Range: 0 dB to +16 dB (can cause distortion at high values)");
    
    for (int g = 0; g <= 16; g += 4) {
        BD37033_setInputGain(g);
        Serial.printf("    Input Gain: %d dB\n", g);
        delay(500);
    }
    BD37033_setInputGain(0);
    Serial.println("    ✅ Input Gain back to 0 dB");
    restoreVolume();
    demoStep++;
}

void demoAdvancedTone() {
    Serial.println("\n--- 🎵 ADVANCED TONE SETTINGS ---");
    Serial.println("    Changing Bass frequency and Q");
    
    BD37033_setBass(5);
    
    Serial.println("    Bass +5 dB, F0: 60 Hz (deep)");
    BD37033_setBassF0(BD37033_BASS_60HZ);
    delay(800);
    
    Serial.println("    Bass +5 dB, F0: 120 Hz (tight)");
    BD37033_setBassF0(BD37033_BASS_120HZ);
    delay(800);
    
    Serial.println("    Bass +5 dB, Q: 1.0 (wide)");
    BD37033_setBassQ(BD37033_BASS_Q_1_0);
    delay(800);
    
    Serial.println("    Bass +5 dB, Q: 2.0 (narrow)");
    BD37033_setBassQ(BD37033_BASS_Q_2_0);
    delay(800);
    
    BD37033_setBass(0);
    BD37033_setBassF0(BD37033_BASS_100HZ);
    BD37033_setBassQ(BD37033_BASS_Q_1_5);
    Serial.println("    ✅ Bass back to 0 dB, F0: 100 Hz, Q: 1.5");
    restoreVolume();
    demoStep++;
}

void demoInputType() {
    Serial.println("\n--- 🔌 INPUT TYPE ---");
    Serial.println("    Switching between Single-ended and Differential");
    Serial.println("    (Effect depends on your source)");
    
    BD37033_setInputType(false, BD37033_INPUT_A);
    Serial.println("    Input A: Single-ended");
    delay(1000);
    
    BD37033_setInputType(true, BD37033_INPUT_A);
    Serial.println("    Input A: Differential");
    delay(1000);
    
    BD37033_setInputType(false, BD37033_INPUT_A);
    Serial.println("    ✅ Back to Single-ended");
    restoreVolume();
    
    Serial.println("\n=== ✅ DEMO COMPLETE ===");
    Serial.println("    Cycle will restart in 3 seconds...");
    demoRunning = false;
    delay(3000);
    demoRunning = true;
    demoStep = 0;
}
