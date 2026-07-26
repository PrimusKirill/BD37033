/*
 * BD37033.h
 * LIBRARY FOR CONTROLLING BD37033FV-M AUDIO PROCESSOR
 * 
 * Author: Kirill Primus
 * License: MIT License
 * 
 * Version: 1.0.0
 * Date: 27.07.2026
 * 
 * Description:
 *   Library for controlling BD37033FV-M audio processor.
 *   Supports: volume, tone control (Bass/Middle/Treble), balance,
 *   Loudness, input selection, subwoofer and mixer management.
 * 
 * Example usage:
 *   #include <BD37033.h>
 *   
 *   void setup() {
 *       Serial.begin(115200);
 *       if (BD37033_init()) {
 *           Serial.println("BD37033 found!");
 *           BD37033_setInput(BD37033_INPUT_A);
 *           BD37033_setVolume(50);
 *           BD37033_setBass(5);
 *       }
 *   }
 */

#ifndef BD37033_H
#define BD37033_H

#include <Arduino.h>
#include <Wire.h>
#include "BD37033_registers.h"

// ==================================================
// DEBUG (enable/disable serial logs)
// ==================================================
#ifndef BD37033_DEBUG
#define BD37033_DEBUG 1   // 1 = logs enabled, 0 = logs disabled
#endif

// ==================================================
// LOGGING MACROS
// ==================================================
#if BD37033_DEBUG
    #define BD37033_LOG(fmt, ...) Serial.printf(fmt, ##__VA_ARGS__)
#else
    #define BD37033_LOG(fmt, ...)
#endif

// ==================================================
// USER-FRIENDLY CONSTANTS
// ==================================================

// Inputs
#define BD37033_INPUT_A   0
#define BD37033_INPUT_B   1
#define BD37033_INPUT_C   2
#define BD37033_INPUT_D   3
#define BD37033_INPUT_E   4

// Subwoofer LPF
#define BD37033_SUB_OFF   0
#define BD37033_SUB_55HZ  1
#define BD37033_SUB_85HZ  2
#define BD37033_SUB_120HZ 3
#define BD37033_SUB_160HZ 4

// Subwoofer Phase
#define BD37033_SUB_PHASE_0    0
#define BD37033_SUB_PHASE_180  1

// Loudness HiCut
#define BD37033_HICUT_1_2KHZ  0
#define BD37033_HICUT_2_4KHZ  1
#define BD37033_HICUT_4_0KHZ  2

// Bass F0
#define BD37033_BASS_60HZ   0
#define BD37033_BASS_80HZ   1
#define BD37033_BASS_100HZ  2
#define BD37033_BASS_120HZ  3

// Middle F0
#define BD37033_MID_500HZ   0
#define BD37033_MID_1KHZ    1
#define BD37033_MID_1_5KHZ  2
#define BD37033_MID_2_5KHZ  3

// Treble F0
#define BD37033_TREBLE_7_5KHZ  0
#define BD37033_TREBLE_10KHZ   1
#define BD37033_TREBLE_12_5KHZ 2
#define BD37033_TREBLE_15KHZ   3

// Bass Q
#define BD37033_BASS_Q_1_0   0
#define BD37033_BASS_Q_1_5   1
#define BD37033_BASS_Q_2_0   2

// Middle Q
#define BD37033_MID_Q_0_75   0
#define BD37033_MID_Q_1_0    1
#define BD37033_MID_Q_1_25   2

// Treble Q
#define BD37033_TREBLE_Q_0_75  0
#define BD37033_TREBLE_Q_1_25  1

// ==================================================
// FUNCTION PROTOTYPES
// ==================================================

#ifdef __cplusplus
extern "C" {
#endif

// ---- INITIALIZATION ----
bool BD37033_init(uint8_t addr = BD37033_ADDR_DEFAULT, int sda = -1, int scl = -1);
bool BD37033_isConnected();
void BD37033_fullReset();

// ---- CORE FUNCTIONS ----
void BD37033_setVolume(int8_t db);              // -79..+15 dB
void BD37033_setInput(uint8_t input);           // 0-4 (A-E) with click protection
void BD37033_setInputDirect(uint8_t input);     // 0-4 (A-E) without protection
void BD37033_setMute(bool enable, uint8_t inputGain = 0);
void BD37033_setBalance(int8_t bal);            // -10..+10
void BD37033_setFader(int8_t value);            // -100..+100

// ---- TONE CONTROLS ----
void BD37033_setBass(int8_t db);                // -15..+15 dB
void BD37033_setMiddle(int8_t db);              // -15..+15 dB
void BD37033_setTreble(int8_t db);              // -15..+15 dB

// ---- ADVANCED TONE SETTINGS ----
void BD37033_setBassF0(uint8_t f0);             // 0-3
void BD37033_setBassQ(uint8_t q);               // 0-2
void BD37033_setMidF0(uint8_t f0);              // 0-3
void BD37033_setMidQ(uint8_t q);                // 0-2
void BD37033_setTrebleF0(uint8_t f0);           // 0-3
void BD37033_setTrebleQ(uint8_t q);             // 0-1

// ---- LOUDNESS ----
void BD37033_setLoudness(bool enable);
void BD37033_setLoudnessGain(uint8_t gain);     // 0-20 dB
void BD37033_setLoudnessHiCut(uint8_t cut);     // 0-2

// ---- INPUTS ----
void BD37033_setInputGain(uint8_t gain);        // 0-16 dB
void BD37033_setInputType(bool diff, uint8_t input);

// ---- SUBWOOFER ----
void BD37033_setSubLPF(uint8_t fc);             // 0-4
void BD37033_setSubPhase(bool invert);
void BD37033_setSubInput(uint8_t sel);          // 0-1
void BD37033_setSubOutput(uint8_t sel);         // 0-1
void BD37033_setSubwooferSource(uint8_t sel);   // 0-3

// ---- MIXER ----
void BD37033_setMixingGain(int8_t gain);        // -79..+15 dB
void BD37033_setMixingInput(uint8_t input);     // 0-4

// ---- ADDITIONAL ----
void BD37033_setAdvancedSwitch(uint8_t value);
void BD37033_setChannelVolume(uint8_t channel, int8_t db);

// ---- READ FUNCTIONS (DIAGNOSTICS) ----
int8_t  BD37033_getVolume();
uint8_t BD37033_getInput();
bool    BD37033_getMute();
int8_t  BD37033_getBalance();
int8_t  BD37033_getBass();
int8_t  BD37033_getMiddle();
int8_t  BD37033_getTreble();
bool    BD37033_getLoudness();
uint8_t BD37033_getInputGain();
bool    BD37033_getInputType();
uint8_t BD37033_getSubLPF();
bool    BD37033_getSubPhase();
uint8_t BD37033_getSubInput();
uint8_t BD37033_getSubOutput();
int8_t  BD37033_getMixingGain();
uint8_t BD37033_getMixingInput();
uint8_t BD37033_getTrebleF0();
uint8_t BD37033_getTrebleQ();

// ---- DIAGNOSTICS ----
uint8_t BD37033_readRegister(uint8_t reg);
void    BD37033_printRegisters();

#ifdef __cplusplus
}
#endif

#endif // BD37033_H