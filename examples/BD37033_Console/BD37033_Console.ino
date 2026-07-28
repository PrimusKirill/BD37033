/*
 * BD37033_Console.ino
 * CONTROL BD37033 VIA SERIAL MONITOR
 * 
 * Type 'help' to see all available commands.
 * 
 * Wiring:
 *   BD37033 SDA → GPIO27 (ESP32) or A4 (Arduino)
 *   BD37033 SCL → GPIO22 (ESP32) or A5 (Arduino)
 *   BD37033 VDD → 3.3V (ESP32) or 5V (Arduino)
 *   BD37033 GND → GND
 */

#include <BD37033.h>
#include <Wire.h>

#define I2C_SDA 27
#define I2C_SCL 22

String cmdBuffer = "";

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n==========================================");
  Serial.println("  BD37033 Console Controller");
  Serial.println("  Type 'help' for commands");
  Serial.println("==========================================\n");

  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(100000);

  if (!BD37033_init()) {
    Serial.println("[ERROR] BD37033 not found!");
    while (1) delay(1000);
  }

  // ★★★ MINIMUM SETTINGS TO GET SOUND ★★★
  BD37033_fullReset();
  BD37033_setInput(0);
  BD37033_setVolume(-20);
  BD37033_setMute(false);

  Serial.println("[SETUP]  Sound is ON (Input A, -20 dB)");
  Serial.println("Type 'help' for commands\n");

  // ★★★ АВТОМАТИЧЕСКИЙ ВЫВОД HELP ★★★
  printHelp();
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      if (cmdBuffer.length() > 0) {
        processCommand(cmdBuffer);
        cmdBuffer = "";
      }
    } else if (c != '\r') {
      cmdBuffer += c;
    }
  }
}

// ==================================================
// BASIC CONTROL FUNCTIONS
// ==================================================

// Управление громкостью (-79..+15 dB)
void setVolume(int value) {
  BD37033_setVolume(value);
  Serial.printf("[VOLUME] %d dB\n", value);
}

// Выбор входа (0=A, 1=B, 2=C, 3=D, 4=E)
void setInput(int value) {
  if (value < 0 || value > 4) {
    Serial.println("[ERROR] Input must be 0..4 (0=A, 1=B, 2=C, 3=D, 4=E)");
    return;
  }
  BD37033_setInput(value);
  Serial.printf("[INPUT] %c\n", 'A' + value);
}

// Управление басом (-15..+15 dB)
void setBass(int value) {
  BD37033_setBass(value);
  Serial.printf("[BASS] %d dB\n", value);
}

// Управление средними частотами (-15..+15 dB)
void setMiddle(int value) {
  BD37033_setMiddle(value);
  Serial.printf("[MIDDLE] %d dB\n", value);
}

// Управление высокими частотами (-15..+15 dB)
void setTreble(int value) {
  BD37033_setTreble(value);
  Serial.printf("[TREBLE] %d dB\n", value);
}

// Управление балансом (-10..+10)
void setBalance(int value) {
  BD37033_setBalance(value);
  Serial.printf("[BALANCE] %d\n", value);
}

// Включение/выключение Loudness (0=OFF, 1=ON)
void setLoudness(int value) {
  BD37033_setLoudness(value != 0);
  Serial.printf("[LOUDNESS] %s\n", value ? "ON" : "OFF");
}

// Включение/выключение Mute (0=OFF, 1=ON)
void setMute(int value) {
  BD37033_setMute(value != 0);
  Serial.printf("[MUTE] %s\n", value ? "ON (no sound)" : "OFF (sound on)");
}

// Управление сабвуфером (0=OFF, 1=55Hz, 2=85Hz, 3=120Hz, 4=160Hz)
void setSubwoofer(int value) {
  if (value < 0 || value > 4) {
    Serial.println("[ERROR] Sub must be 0..4 (0=OFF, 1=55Hz, 2=85Hz, 3=120Hz, 4=160Hz)");
    return;
  }
  BD37033_setSubLPF(value);
  const char* names[] = { "OFF", "55Hz", "85Hz", "120Hz", "160Hz" };
  Serial.printf("[SUB LPF] %s\n", names[value]);
}

// ==================================================
// ADVANCED TONE SETTINGS (for professionals)
// ==================================================

// Управление частотой среза баса (0=60Hz, 1=80Hz, 2=100Hz, 3=120Hz)
void setBassF0(int value) {
  if (value < 0 || value > 3) {
    Serial.println("[ERROR] Bass F0 must be 0..3 (0=60Hz, 1=80Hz, 2=100Hz, 3=120Hz)");
    return;
  }
  BD37033_setBassF0(value);
  const char* names[] = { "60Hz", "80Hz", "100Hz", "120Hz" };
  Serial.printf("[BASS F0] %s\n", names[value]);
}

// Управление добротностью баса (0=1.0, 1=1.5, 2=2.0)
void setBassQ(int value) {
  if (value < 0 || value > 2) {
    Serial.println("[ERROR] Bass Q must be 0..2 (0=1.0, 1=1.5, 2=2.0)");
    return;
  }
  BD37033_setBassQ(value);
  const char* names[] = { "1.0", "1.5", "2.0" };
  Serial.printf("[BASS Q] %s\n", names[value]);
}

// Управление частотой среза средних (0=500Hz, 1=1kHz, 2=1.5kHz, 3=2.5kHz)
void setMidF0(int value) {
  if (value < 0 || value > 3) {
    Serial.println("[ERROR] Mid F0 must be 0..3 (0=500Hz, 1=1kHz, 2=1.5kHz, 3=2.5kHz)");
    return;
  }
  BD37033_setMidF0(value);
  const char* names[] = { "500Hz", "1kHz", "1.5kHz", "2.5kHz" };
  Serial.printf("[MID F0] %s\n", names[value]);
}

// Управление добротностью средних (0=0.75, 1=1.0, 2=1.25)
void setMidQ(int value) {
  if (value < 0 || value > 2) {
    Serial.println("[ERROR] Mid Q must be 0..2 (0=0.75, 1=1.0, 2=1.25)");
    return;
  }
  BD37033_setMidQ(value);
  const char* names[] = { "0.75", "1.0", "1.25" };
  Serial.printf("[MID Q] %s\n", names[value]);
}

// Управление частотой среза высоких (0=7.5kHz, 1=10kHz, 2=12.5kHz, 3=15kHz)
void setTrebleF0(int value) {
  if (value < 0 || value > 3) {
    Serial.println("[ERROR] Treble F0 must be 0..3 (0=7.5kHz, 1=10kHz, 2=12.5kHz, 3=15kHz)");
    return;
  }
  BD37033_setTrebleF0(value);
  const char* names[] = { "7.5kHz", "10kHz", "12.5kHz", "15kHz" };
  Serial.printf("[TREBLE F0] %s\n", names[value]);
}

// Управление добротностью высоких (0=0.75, 1=1.25)
void setTrebleQ(int value) {
  if (value < 0 || value > 1) {
    Serial.println("[ERROR] Treble Q must be 0..1 (0=0.75, 1=1.25)");
    return;
  }
  BD37033_setTrebleQ(value);
  const char* names[] = { "0.75", "1.25" };
  Serial.printf("[TREBLE Q] %s\n", names[value]);
}