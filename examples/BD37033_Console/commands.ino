/*
 * commands.ino
 * UART COMMAND PROCESSING
 * 
 * Parses incoming commands and calls the appropriate function.
 */

void processCommand(String cmd) {
    cmd.trim();
    if (cmd.length() == 0) return;

    int space = cmd.indexOf(' ');
    String command = cmd;
    String arg = "";

    if (space > 0) {
        command = cmd.substring(0, space);
        arg = cmd.substring(space + 1);
        arg.trim();
    }

    command.toLowerCase();

    // ---- help ----
    if (command == "help" || command == "h") {
        printHelp();
        return;
    }

    // ---- reset ----
    if (command == "reset") {
        resetAllSettings();
        return;
    }

    // ---- readreg ----
    if (command == "readreg" || command == "rr") {
        if (arg.length() == 0) {
            Serial.println("[ERROR] Missing register address. Example: readreg 0x20");
            return;
        }
        int reg = strtol(arg.c_str(), NULL, 0);
        if (reg < 0 || reg > 0x7F) {
            Serial.println("[ERROR] Register must be 0x00..0x7F");
            return;
        }
        uint8_t regValue = BD37033_readRegister((uint8_t)reg);
        Serial.printf("[REG 0x%02X] = 0x%02X (%d)\n", reg, regValue, regValue);
        return;
    }

    // ---- COMMANDS WITH ARGUMENTS ----
    if (arg.length() == 0) {
        Serial.println("[ERROR] Missing argument. Type 'help'.");
        return;
    }

    int val = arg.toInt();

    // ---- BASIC ----
    if (command == "vol" || command == "volume") { setVolume(val); return; }
    if (command == "input" || command == "in") { setInput(val); return; }
    if (command == "bass") { setBass(val); return; }
    if (command == "middle" || command == "mid") { setMiddle(val); return; }
    if (command == "treble" || command == "treb") { setTreble(val); return; }
    if (command == "balance" || command == "bal") { setBalance(val); return; }
    if (command == "loudness" || command == "loud") { setLoudness(val); return; }
    if (command == "mute") { setMute(val); return; }
    if (command == "sub" || command == "subwoofer") { setSubwoofer(val); return; }

    // ---- ADVANCED TONE ----
    if (command == "bassf0") { setBassF0(val); return; }
    if (command == "bassq") { setBassQ(val); return; }
    if (command == "midf0") { setMidF0(val); return; }
    if (command == "midq") { setMidQ(val); return; }
    if (command == "treblef0" || command == "trebf0") { setTrebleF0(val); return; }
    if (command == "trebleq" || command == "trebq") { setTrebleQ(val); return; }

    Serial.print("[ERROR] Unknown command: ");
    Serial.println(command);
}

// ==================================================
// UTILITY FUNCTIONS
// ==================================================

void printHelp() {
    Serial.println("\n========== COMMANDS ==========");
    Serial.println("  help          - show this help");
    Serial.println("  vol -30       - volume (-79..+15 dB)");
    Serial.println("  input 0       - input (0=A, 1=B, 2=C, 3=D, 4=E)");
    Serial.println("  bass 5        - bass (-15..+15 dB)");
    Serial.println("  middle -3     - middle (-15..+15 dB)");
    Serial.println("  treble 7      - treble (-15..+15 dB)");
    Serial.println("  balance -5    - balance (-10..+10)");
    Serial.println("  loudness 1    - loudness (0=OFF, 1=ON)");
    Serial.println("  mute 1        - mute (0=OFF, 1=ON)");
    Serial.println("  sub 2         - subwoofer (0=OFF, 1=55Hz, 2=85Hz, 3=120Hz, 4=160Hz)");
    Serial.println("  reset         - full reset to defaults");
    Serial.println("  readreg 0x20  - read register (returns 0xFF if not supported)");
    Serial.println("\n--- ADVANCED TONE ---");
    Serial.println("  bassf0 0      - bass frequency (0=60Hz, 1=80Hz, 2=100Hz, 3=120Hz)");
    Serial.println("  bassq 1       - bass Q (0=1.0, 1=1.5, 2=2.0)");
    Serial.println("  midf0 1       - middle frequency (0=500Hz, 1=1kHz, 2=1.5kHz, 3=2.5kHz)");
    Serial.println("  midq 1        - middle Q (0=0.75, 1=1.0, 2=1.25)");
    Serial.println("  treblef0 1    - treble frequency (0=7.5kHz, 1=10kHz, 2=12.5kHz, 3=15kHz)");
    Serial.println("  trebleq 0     - treble Q (0=0.75, 1=1.25)");
    Serial.println("================================\n");
}

void resetAllSettings() {
    BD37033_fullReset();
    Serial.println("[RESET] All registers reset to defaults");
    Serial.println("   Don't forget: set input, volume and mute!");
}