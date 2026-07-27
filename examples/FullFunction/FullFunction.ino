/*
 * FullFunction.ino
 * FULL FUNCTIONALITY EXAMPLE FOR BD37033 LIBRARY
 * 
 * This example demonstrates ALL functions of the BD37033 library.
 * Use it as a reference to understand how to use each feature.
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

// ==================================================
// GLOBAL VARIABLES
// ==================================================

// For demo purposes
int currentStep = 0;
unsigned long lastStepTime = 0;

// ==================================================
// SETUP
// ==================================================

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n==========================================");
    Serial.println("  BD37033 FULL FUNCTION EXAMPLE");
    Serial.println("==========================================\n");

    // ---- 1. INITIALIZATION ----
    Serial.println("[1] INITIALIZATION");
    if (BD37033_init()) {
        Serial.println("    ✅ BD37033 found!");
    } else {
        Serial.println("    ❌ BD37033 NOT found!");
        Serial.println("       Check wiring and power.");
        while (1) {
            delay(1000);
        }
    }

    // ---- 2. FULL RESET ----
    Serial.println("\n[2] FULL RESET");
    BD37033_fullReset();
    Serial.println("    ✅ All registers reset to default");

    // ---- 3. DEMONSTRATE ALL FUNCTIONS ----
    Serial.println("\n[3] DEMONSTRATING ALL FUNCTIONS");
    demonstrateAllFunctions();

    Serial.println("\n==========================================");
    Serial.println("  ✅ Setup complete! Starting demo loop...");
    Serial.println("==========================================\n");
}

// ==================================================
// LOOP
// ==================================================

void loop() {
    // Run a simple demo cycling through different settings
    runDemoCycle();
    
    delay(2000);  // Wait 2 seconds between steps
}

// ==================================================
// DEMONSTRATE ALL FUNCTIONS
// ==================================================

void demonstrateAllFunctions() {
    // ---- VOLUME ----
    Serial.println("    --- VOLUME ---");
    BD37033_setVolume(30);   // -49 dB
    Serial.println("    Volume set to 30 (-49 dB)");
    
    delay(500);

    // ---- INPUT SELECTION ----
    Serial.println("    --- INPUT SELECTION ---");
    BD37033_setInput(BD37033_INPUT_A);
    Serial.println("    Input: A");
    BD37033_setInput(BD37033_INPUT_B);
    Serial.println("    Input: B");
    BD37033_setInput(BD37033_INPUT_C);
    Serial.println("    Input: C");
    BD37033_setInput(BD37033_INPUT_D);
    Serial.println("    Input: D");
    BD37033_setInput(BD37033_INPUT_E);
    Serial.println("    Input: E");
    
    delay(500);

    // ---- TONE CONTROLS ----
    Serial.println("    --- TONE CONTROLS ---");
    BD37033_setBass(5);
    Serial.println("    Bass: +5 dB");
    BD37033_setMiddle(0);
    Serial.println("    Middle: 0 dB");
    BD37033_setTreble(-3);
    Serial.println("    Treble: -3 dB");
    
    delay(500);

    // ---- ADVANCED TONE SETTINGS ----
    Serial.println("    --- ADVANCED TONE SETTINGS ---");
    BD37033_setBassF0(BD37033_BASS_80HZ);
    Serial.println("    Bass F0: 80 Hz");
    BD37033_setBassQ(BD37033_BASS_Q_1_5);
    Serial.println("    Bass Q: 1.5");
    
    BD37033_setMidF0(BD37033_MID_1KHZ);
    Serial.println("    Mid F0: 1 kHz");
    BD37033_setMidQ(BD37033_MID_Q_1_0);
    Serial.println("    Mid Q: 1.0");
    
    BD37033_setTrebleF0(BD37033_TREBLE_10KHZ);
    Serial.println("    Treble F0: 10 kHz");
    BD37033_setTrebleQ(BD37033_TREBLE_Q_0_75);
    Serial.println("    Treble Q: 0.75");
    
    delay(500);

    // ---- BALANCE ----
    Serial.println("    --- BALANCE ---");
    BD37033_setBalance(0);
    Serial.println("    Balance: 0 (center)");
    BD37033_setBalance(-5);
    Serial.println("    Balance: -5 (left louder)");
    BD37033_setBalance(5);
    Serial.println("    Balance: +5 (right louder)");
    BD37033_setBalance(0);
    Serial.println("    Balance: 0 (center)");
    
    delay(500);

    // ---- FADER ----
    Serial.println("    --- FADER ---");
    BD37033_setFader(0);
    Serial.println("    Fader: 0 (center)");
    BD37033_setFader(-50);
    Serial.println("    Fader: -50 (rear quieter)");
    BD37033_setFader(50);
    Serial.println("    Fader: +50 (front quieter)");
    BD37033_setFader(0);
    Serial.println("    Fader: 0 (center)");
    
    delay(500);

    // ---- LOUDNESS ----
    Serial.println("    --- LOUDNESS ---");
    BD37033_setLoudness(true);
    Serial.println("    Loudness: ON");
    BD37033_setLoudnessGain(10);
    Serial.println("    Loudness Gain: 10 dB");
    BD37033_setLoudnessHiCut(BD37033_HICUT_2_4KHZ);
    Serial.println("    Loudness HiCut: 2.4 kHz");
    
    delay(500);

    // ---- INPUT GAIN ----
    Serial.println("    --- INPUT GAIN ---");
    BD37033_setInputGain(0);
    Serial.println("    Input Gain: 0 dB");
    BD37033_setInputGain(8);
    Serial.println("    Input Gain: 8 dB");
    BD37033_setInputGain(0);
    Serial.println("    Input Gain: 0 dB");
    
    delay(500);

    // ---- INPUT TYPE ----
    Serial.println("    --- INPUT TYPE ---");
    BD37033_setInputType(false, BD37033_INPUT_A);  // Single-ended
    Serial.println("    Input A: Single-ended");
    BD37033_setInputType(true, BD37033_INPUT_B);   // Differential
    Serial.println("    Input B: Differential");
    BD37033_setInputType(false, BD37033_INPUT_C);
    Serial.println("    Input C: Single-ended");
    
    delay(500);

    // ---- SUBWOOFER ----
    Serial.println("    --- SUBWOOFER ---");
    BD37033_setSubLPF(BD37033_SUB_85HZ);
    Serial.println("    Sub LPF: 85 Hz");
    BD37033_setSubPhase(BD37033_SUB_PHASE_0);
    Serial.println("    Sub Phase: 0°");
    BD37033_setSubInput(0);
    Serial.println("    Sub Input: Fader");
    BD37033_setSubOutput(0);
    Serial.println("    Sub Output: OUTS");
    
    delay(500);

    // ---- MIXER ----
    Serial.println("    --- MIXER ---");
    BD37033_setMixingGain(0);
    Serial.println("    Mixing Gain: 0 dB");
    BD37033_setMixingInput(BD37033_INPUT_A);
    Serial.println("    Mixing Input: A");
    
    delay(500);

    // ---- MUTE ----
    Serial.println("    --- MUTE ---");
    BD37033_setMute(true);
    Serial.println("    Mute: ON");
    delay(500);
    BD37033_setMute(false);
    Serial.println("    Mute: OFF");
    
    delay(500);

    // ---- ADVANCED SWITCH ----
    Serial.println("    --- ADVANCED SWITCH ---");
    BD37033_setAdvancedSwitch(0b11100010);
    Serial.println("    Advanced Switch: 0b11100010 (RF protection)");
    
    delay(500);

    // ---- READ FUNCTIONS ----
    Serial.println("    --- READ FUNCTIONS (DIAGNOSTICS) ---");
    int8_t vol = BD37033_getVolume();
    uint8_t input = BD37033_getInput();
    bool mute = BD37033_getMute();
    int8_t bass = BD37033_getBass();
    int8_t mid = BD37033_getMiddle();
    int8_t treb = BD37033_getTreble();
    bool loud = BD37033_getLoudness();
    uint8_t gain = BD37033_getInputGain();
    bool type = BD37033_getInputType();
    
    Serial.printf("    Volume: %d dB\n", vol);
    Serial.printf("    Input: %c\n", 'A' + input);
    Serial.printf("    Mute: %s\n", mute ? "ON" : "OFF");
    Serial.printf("    Bass: %d dB\n", bass);
    Serial.printf("    Middle: %d dB\n", mid);
    Serial.printf("    Treble: %d dB\n", treb);
    Serial.printf("    Loudness: %s\n", loud ? "ON" : "OFF");
    Serial.printf("    Input Gain: %d dB\n", gain);
    Serial.printf("    Input Type: %s\n", type ? "Differential" : "Single-ended");
    
    // ---- PRINT REGISTERS ----
    Serial.println("    --- PRINTING ALL REGISTERS ---");
    BD37033_printRegisters();

    // ---- FINAL STATE ----
    Serial.println("    --- FINAL STATE (all settings applied) ---");
    BD37033_setVolume(30);
    BD37033_setInput(BD37033_INPUT_A);
    BD37033_setBass(0);
    BD37033_setMiddle(0);
    BD37033_setTreble(0);
    BD37033_setBalance(0);
    BD37033_setLoudness(false);
    BD37033_setMute(false);
    Serial.println("    All settings reset to neutral");
}

// ==================================================
// DEMO CYCLE
// ==================================================

void runDemoCycle() {
    switch (currentStep) {
        case 0:
            Serial.println("\n--- Demo: Volume sweep ---");
            for (int v = 5; v <= 50; v += 5) {
                BD37033_setVolume(v);
                Serial.printf("Volume: %d\n", v);
                delay(300);
            }
            for (int v = 45; v >= 5; v -= 5) {
                BD37033_setVolume(v);
                Serial.printf("Volume: %d\n", v);
                delay(300);
            }
            BD37033_setVolume(30);
            break;
            
        case 1:
            Serial.println("\n--- Demo: Bass sweep ---");
            for (int b = -15; b <= 15; b += 3) {
                BD37033_setBass(b);
                Serial.printf("Bass: %d dB\n", b);
                delay(300);
            }
            BD37033_setBass(0);
            break;
            
        case 2:
            Serial.println("\n--- Demo: Treble sweep ---");
            for (int t = -15; t <= 15; t += 3) {
                BD37033_setTreble(t);
                Serial.printf("Treble: %d dB\n", t);
                delay(300);
            }
            BD37033_setTreble(0);
            break;
            
        case 3:
            Serial.println("\n--- Demo: Balance sweep ---");
            for (int b = -10; b <= 10; b += 2) {
                BD37033_setBalance(b);
                Serial.printf("Balance: %d\n", b);
                delay(300);
            }
            BD37033_setBalance(0);
            break;
            
        case 4:
            Serial.println("\n--- Demo: Input cycling ---");
            for (int i = 0; i < 5; i++) {
                BD37033_setInput(i);
                Serial.printf("Input: %c\n", 'A' + i);
                delay(500);
            }
            BD37033_setInput(BD37033_INPUT_A);
            break;
            
        case 5:
            Serial.println("\n--- Demo: Loudness toggle ---");
            BD37033_setLoudness(true);
            Serial.println("Loudness: ON");
            delay(1000);
            BD37033_setLoudness(false);
            Serial.println("Loudness: OFF");
            break;
            
        case 6:
            Serial.println("\n--- Demo: Mute toggle ---");
            BD37033_setMute(true);
            Serial.println("Mute: ON");
            delay(1000);
            BD37033_setMute(false);
            Serial.println("Mute: OFF");
            break;
            
        case 7:
            Serial.println("\n--- Demo: Subwoofer LPF cycling ---");
            int lpfValues[] = {BD37033_SUB_OFF, BD37033_SUB_55HZ, BD37033_SUB_85HZ, BD37033_SUB_120HZ, BD37033_SUB_160HZ};
            const char* lpfNames[] = {"OFF", "55 Hz", "85 Hz", "120 Hz", "160 Hz"};
            for (int i = 0; i < 5; i++) {
                BD37033_setSubLPF(lpfValues[i]);
                Serial.printf("Sub LPF: %s\n", lpfNames[i]);
                delay(500);
            }
            BD37033_setSubLPF(BD37033_SUB_OFF);
            break;
            
        default:
            break;
    }
    
    currentStep++;
    if (currentStep > 7) {
        currentStep = 0;
        Serial.println("\n=== Demo cycle restarted ===");
    }
}
