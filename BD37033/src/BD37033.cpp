/*
 * BD37033.cpp
 * IMPLEMENTATION OF BD37033FV-M LIBRARY
 * 
 * Version: 1.0.0
 * Date: 27.07.2026
 * 
 * Author: Kirill Primus
 * License: MIT License
 */

#include "BD37033.h"
#include "BD37033_registers.h"

// ==================================================
// INTERNAL VARIABLES
// ==================================================

static uint8_t _i2cAddr = BD37033_ADDR_DEFAULT;
static bool _initialized = false;
static uint8_t _loudnessGain = 10;
static uint8_t _loudnessHiCut = 1;  // 1 = 2.4 kHz
static bool _loudnessEnabled = false;

// ==================================================
// INTERNAL I2C FUNCTIONS
// ==================================================

static bool _writeReg(uint8_t reg, uint8_t val) {
    if (!_initialized) return false;
    
    for (int i = 0; i < 3; i++) {
        Wire.beginTransmission(_i2cAddr);
        Wire.write(reg);
        Wire.write(val);
        if (Wire.endTransmission() == 0) {
            return true;
        }
        delay(5);
    }
    BD37033_LOG("[BD37033] ERROR: writeReg(0x%02X, 0x%02X) failed!\n", reg, val);
    return false;
}

static uint8_t _readReg(uint8_t reg) {
    if (!_initialized) return 0xFF;
    
    Wire.beginTransmission(_i2cAddr);
    Wire.write(reg);
    if (Wire.endTransmission(false) != 0) {
        return 0xFF;
    }
    delay(2);
    Wire.requestFrom(_i2cAddr, (uint8_t)1);
    if (Wire.available()) {
        return Wire.read();
    }
    return 0xFF;
}

// ==================================================
// CONVERSION HELPERS
// ==================================================

uint8_t BD37033_volumeDbToReg(int8_t db) {
    if (db >= 0) return 0x7F - db;
    else return 0x80 + (-db);
}

int8_t BD37033_volumeRegToDb(uint8_t reg) {
    if (reg >= 0x70 && reg <= 0x7F) return 0x7F - reg;
    else if (reg >= 0x80 && reg <= 0xCF) return -(reg - 0x80);
    else return 0;
}

uint8_t BD37033_toneDbToReg(int8_t db) {
    if (db >= 0) return db;
    else return 0x80 + (-db);
}

int8_t BD37033_toneRegToDb(uint8_t reg) {
    if (reg & 0x80) return -(reg & 0x7F);
    else return reg;
}

// ==================================================
// INITIALIZATION
// ==================================================

bool BD37033_init(uint8_t addr, int sda, int scl) {
    _i2cAddr = addr;
    _initialized = false;
    
    if (sda >= 0 && scl >= 0) {
        Wire.begin(sda, scl);
        BD37033_LOG("[BD37033] I2C started on SDA=%d, SCL=%d\n", sda, scl);
    } else {
        Wire.begin();
        BD37033_LOG("[BD37033] I2C started (default pins)\n");
    }
    Wire.setClock(100000);
    delay(100);
    
    if (!BD37033_isConnected()) {
        BD37033_LOG("[BD37033] ERROR: Device not found at 0x%02X!\n", _i2cAddr);
        return false;
    }
    
    _initialized = true;
    BD37033_LOG("[BD37033] OK: device found at 0x%02X\n", _i2cAddr);
    BD37033_fullReset();
    return true;
}

bool BD37033_isConnected() {
    Wire.beginTransmission(_i2cAddr);
    return (Wire.endTransmission() == 0);
}

void BD37033_fullReset() {
    if (!_initialized) return;
    
    BD37033_LOG("[BD37033] Full reset...\n");
    
    // Advanced Switch + Anti-Alias Filter
    _writeReg(BD37033_REG_ADVANCED_SWITCH, 
              BD37033_SWITCH_ADVANCED_EN | BD37033_SWITCH_ANTIALIAS_EN | 0x02);
    delay(10);
    
    // Channel gains (0 dB)
    _writeReg(BD37033_REG_FRONT_L, 0x7F);
    _writeReg(BD37033_REG_FRONT_R, 0x7F);
    _writeReg(BD37033_REG_REAR_L, 0x7F);
    _writeReg(BD37033_REG_REAR_R, 0x7F);
    _writeReg(BD37033_REG_SUB_L, 0x7F);
    _writeReg(BD37033_REG_MIX_GAIN, 0x7F);
    
    // Subwoofer: OFF
    _writeReg(BD37033_REG_SUBWOOFER, BD37033_SUB_LPF_OFF);
    
    // Mixer: OFF
    _writeReg(BD37033_REG_MIXER, 0x00);
    
    // Input: A
    _writeReg(BD37033_REG_INPUT_SELECT, BD37033_INPUT_A);
    
    // Mute ON, Input Gain 0
    _writeReg(BD37033_REG_MUTE_GAIN, BD37033_MUTE_BIT);
    
    // Master Volume: -79 dB
    _writeReg(BD37033_REG_MASTER_VOLUME, 0xCF);
    
    // Tone controls: 0 dB
    _writeReg(BD37033_REG_BASS_GAIN, 0x00);
    _writeReg(BD37033_REG_MID_GAIN, 0x00);
    _writeReg(BD37033_REG_TREBLE_GAIN, 0x00);
    
    // Frequencies and Q (neutral)
    _writeReg(BD37033_REG_BASS_FQ, 
              BD37033_MAKE_FQ_REG(BD37033_BASS_F0_100HZ, BD37033_BASS_Q_1_5));
    _writeReg(BD37033_REG_MID_FQ, 
              BD37033_MAKE_FQ_REG(BD37033_MID_F0_1KHZ, BD37033_MID_Q_1_0));
    _writeReg(BD37033_REG_TREBLE_FQ, 
              BD37033_MAKE_FQ_REG(BD37033_TREBLE_F0_10KHZ, BD37033_TREBLE_Q_1_25));
    
    // Loudness: OFF
    _writeReg(BD37033_REG_LOUDNESS, 0x00);
    _loudnessEnabled = false;
    
    BD37033_LOG("[BD37033] Full reset complete\n");
}

// ==================================================
// CORE FUNCTIONS
// ==================================================

void BD37033_setVolume(int8_t db) {
    if (!_initialized) return;
    if (db < -79) db = -79;
    if (db > 15) db = 15;
    _writeReg(BD37033_REG_MASTER_VOLUME, BD37033_volumeDbToReg(db));
}

void BD37033_setInput(uint8_t input) {
    if (!_initialized) return;
    if (input > 4) input = 4;
    
    BD37033_setMute(true, 0);
    delay(50);
    BD37033_setInputDirect(input);
    delay(50);
    _writeReg(BD37033_REG_MUTE_GAIN, 0x00);
}

void BD37033_setInputDirect(uint8_t input) {
    if (!_initialized) return;
    if (input > 4) input = 4;
    const uint8_t inputValues[] = {
        BD37033_INPUT_A, BD37033_INPUT_B, BD37033_INPUT_C, 
        BD37033_INPUT_D, BD37033_INPUT_E
    };
    _writeReg(BD37033_REG_INPUT_SELECT, inputValues[input]);
}

void BD37033_setMute(bool enable, uint8_t inputGain) {
    if (!_initialized) return;
    if (inputGain > 16) inputGain = 16;
    uint8_t reg = (enable ? BD37033_MUTE_BIT : 0x00) | (inputGain & BD37033_INPUT_GAIN_MASK);
    _writeReg(BD37033_REG_MUTE_GAIN, reg);
}

void BD37033_setBalance(int8_t bal) {
    if (!_initialized) return;
    if (bal < -10) bal = -10;
    if (bal > 10) bal = 10;
    
    int8_t leftGain = 0, rightGain = 0;
    if (bal < 0) rightGain = bal;
    else if (bal > 0) leftGain = -bal;
    
    _writeReg(BD37033_REG_FRONT_L, BD37033_volumeDbToReg(leftGain));
    _writeReg(BD37033_REG_FRONT_R, BD37033_volumeDbToReg(rightGain));
}

void BD37033_setFader(int8_t value) {
    if (!_initialized) return;
    if (value < -100) value = -100;
    if (value > 100) value = 100;
    
    uint8_t front = map(value, -100, 100, 0x00, 0x7F);
    uint8_t rear = 0x7F - front;
    
    _writeReg(BD37033_REG_FRONT_L, front);
    _writeReg(BD37033_REG_FRONT_R, front);
    _writeReg(BD37033_REG_REAR_L, rear);
    _writeReg(BD37033_REG_REAR_R, rear);
}

// ==================================================
// TONE CONTROLS
// ==================================================

void BD37033_setBass(int8_t db) {
    if (!_initialized) return;
    if (db < -15) db = -15;
    if (db > 15) db = 15;
    _writeReg(BD37033_REG_BASS_GAIN, BD37033_toneDbToReg(db));
}

void BD37033_setMiddle(int8_t db) {
    if (!_initialized) return;
    if (db < -15) db = -15;
    if (db > 15) db = 15;
    _writeReg(BD37033_REG_MID_GAIN, BD37033_toneDbToReg(db));
}

void BD37033_setTreble(int8_t db) {
    if (!_initialized) return;
    if (db < -15) db = -15;
    if (db > 15) db = 15;
    _writeReg(BD37033_REG_TREBLE_GAIN, BD37033_toneDbToReg(db));
}

// ==================================================
// ADVANCED TONE SETTINGS
// ==================================================

void BD37033_setBassF0(uint8_t f0) {
    if (!_initialized) return;
    if (f0 > 3) f0 = 3;
    _writeReg(BD37033_REG_BASS_FQ, BD37033_MAKE_FQ_REG(f0, BD37033_BASS_Q_1_5));
}

void BD37033_setBassQ(uint8_t q) {
    if (!_initialized) return;
    if (q > 2) q = 2;
    _writeReg(BD37033_REG_BASS_FQ, BD37033_MAKE_FQ_REG(BD37033_BASS_F0_100HZ, q));
}

void BD37033_setMidF0(uint8_t f0) {
    if (!_initialized) return;
    if (f0 > 3) f0 = 3;
    _writeReg(BD37033_REG_MID_FQ, BD37033_MAKE_FQ_REG(f0, BD37033_MID_Q_1_0));
}

void BD37033_setMidQ(uint8_t q) {
    if (!_initialized) return;
    if (q > 2) q = 2;
    _writeReg(BD37033_REG_MID_FQ, BD37033_MAKE_FQ_REG(BD37033_MID_F0_1KHZ, q));
}

void BD37033_setTrebleF0(uint8_t f0) {
    if (!_initialized) return;
    if (f0 > 3) f0 = 3;
    _writeReg(BD37033_REG_TREBLE_FQ, BD37033_MAKE_FQ_REG(f0, BD37033_TREBLE_Q_1_25));
}

void BD37033_setTrebleQ(uint8_t q) {
    if (!_initialized) return;
    if (q > 1) q = 1;
    _writeReg(BD37033_REG_TREBLE_FQ, BD37033_MAKE_FQ_REG(BD37033_TREBLE_F0_10KHZ, q));
}

// ==================================================
// LOUDNESS
// ==================================================

void BD37033_setLoudness(bool enable) {
    if (!_initialized) return;
    _loudnessEnabled = enable;
    if (enable) {
        _writeReg(BD37033_REG_LOUDNESS, 
                  BD37033_MAKE_LOUDNESS_REG(_loudnessGain, _loudnessHiCut));
    } else {
        _writeReg(BD37033_REG_LOUDNESS, 0x00);
    }
}

void BD37033_setLoudnessGain(uint8_t gain) {
    if (!_initialized) return;
    if (gain > 20) gain = 20;
    _loudnessGain = gain;
    if (_loudnessEnabled) {
        _writeReg(BD37033_REG_LOUDNESS, 
                  BD37033_MAKE_LOUDNESS_REG(_loudnessGain, _loudnessHiCut));
    }
}

void BD37033_setLoudnessHiCut(uint8_t cut) {
    if (!_initialized) return;
    if (cut > 2) cut = 2;
    _loudnessHiCut = cut;
    if (_loudnessEnabled) {
        _writeReg(BD37033_REG_LOUDNESS, 
                  BD37033_MAKE_LOUDNESS_REG(_loudnessGain, _loudnessHiCut));
    }
}

// ==================================================
// INPUTS
// ==================================================

void BD37033_setInputGain(uint8_t gain) {
    if (!_initialized) return;
    if (gain > 16) gain = 16;
    _writeReg(BD37033_REG_MUTE_GAIN, gain);
}

void BD37033_setInputType(bool diff, uint8_t input) {
    if (!_initialized) return;
    if (input > 4) input = 4;
    const uint8_t inputValues[] = {
        BD37033_INPUT_A, BD37033_INPUT_B, BD37033_INPUT_C, 
        BD37033_INPUT_D, BD37033_INPUT_E
    };
    uint8_t reg = inputValues[input];
    if (diff) reg |= BD37033_INPUT_DIFF_MASK;
    _writeReg(BD37033_REG_INPUT_SELECT, reg);
}

// ==================================================
// SUBWOOFER
// ==================================================

void BD37033_setSubLPF(uint8_t fc) {
    if (!_initialized) return;
    if (fc > 4) fc = 4;
    uint8_t reg = _readReg(BD37033_REG_SUBWOOFER);
    if (reg != 0xFF) {
        reg = (reg & 0xF0) | fc;
        _writeReg(BD37033_REG_SUBWOOFER, reg);
    } else {
        _writeReg(BD37033_REG_SUBWOOFER, fc);
    }
}

void BD37033_setSubPhase(bool invert) {
    if (!_initialized) return;
    uint8_t reg = _readReg(BD37033_REG_SUBWOOFER);
    if (reg != 0xFF) {
        if (invert) reg |= BD37033_SUB_PHASE_MASK;
        else reg &= ~BD37033_SUB_PHASE_MASK;
        _writeReg(BD37033_REG_SUBWOOFER, reg);
    } else {
        _writeReg(BD37033_REG_SUBWOOFER, invert ? BD37033_SUB_PHASE_MASK : 0x00);
    }
}

void BD37033_setSubInput(uint8_t sel) {
    if (!_initialized) return;
    if (sel > 1) sel = 1;
    uint8_t reg = _readReg(BD37033_REG_SUBWOOFER);
    if (reg != 0xFF) {
        if (sel) reg |= BD37033_SUB_INPUT_MASK;
        else reg &= ~BD37033_SUB_INPUT_MASK;
        _writeReg(BD37033_REG_SUBWOOFER, reg);
    } else {
        _writeReg(BD37033_REG_SUBWOOFER, sel ? BD37033_SUB_INPUT_MASK : 0x00);
    }
}

void BD37033_setSubOutput(uint8_t sel) {
    if (!_initialized) return;
    if (sel > 1) sel = 1;
    uint8_t reg = _readReg(BD37033_REG_SUBWOOFER);
    if (reg != 0xFF) {
        if (sel) reg |= BD37033_SUB_OUTPUT_MASK;
        else reg &= ~BD37033_SUB_OUTPUT_MASK;
        _writeReg(BD37033_REG_SUBWOOFER, reg);
    } else {
        _writeReg(BD37033_REG_SUBWOOFER, sel ? BD37033_SUB_OUTPUT_MASK : 0x00);
    }
}

// ==================================================
// ADDITIONAL FUNCTIONS
// ==================================================

void BD37033_setSubwooferSource(uint8_t sel) {
    if (!_initialized) return;
    if (sel > 3) sel = 3;
    _writeReg(BD37033_REG_SUBWOOFER, sel);
}

void BD37033_setMixingGain(int8_t gain) {
    if (!_initialized) return;
    if (gain < -79) gain = -79;
    if (gain > 15) gain = 15;
    _writeReg(BD37033_REG_MIX_GAIN, BD37033_volumeDbToReg(gain));
}

void BD37033_setMixingInput(uint8_t input) {
    if (!_initialized) return;
    if (input > 4) input = 4;
    _writeReg(BD37033_REG_MIXER, input << 5);
}

void BD37033_setAdvancedSwitch(uint8_t value) {
    if (!_initialized) return;
    _writeReg(BD37033_REG_ADVANCED_SWITCH, value);
}

void BD37033_setChannelVolume(uint8_t channel, int8_t db) {
    if (!_initialized) return;
    const uint8_t regs[] = {
        BD37033_REG_FRONT_L, 
        BD37033_REG_FRONT_R, 
        BD37033_REG_REAR_L, 
        BD37033_REG_REAR_R, 
        BD37033_REG_SUB_L
    };
    if (channel < 5) {
        _writeReg(regs[channel], BD37033_volumeDbToReg(db));
    }
}

// ==================================================
// READ FUNCTIONS (DIAGNOSTICS)
// ==================================================

int8_t BD37033_getVolume() {
    uint8_t reg = _readReg(BD37033_REG_MASTER_VOLUME);
    if (reg == 0xFF) return 0;
    return BD37033_volumeRegToDb(reg);
}

uint8_t BD37033_getInput() {
    uint8_t reg = _readReg(BD37033_REG_INPUT_SELECT);
    if (reg == 0xFF) return 0;
    uint8_t input = reg & BD37033_INPUT_SEL_MASK;
    switch(input) {
        case BD37033_INPUT_A: return 0;
        case BD37033_INPUT_B: return 1;
        case BD37033_INPUT_C: return 2;
        case BD37033_INPUT_D: return 3;
        case BD37033_INPUT_E: return 4;
        default: return 0;
    }
}

bool BD37033_getMute() {
    uint8_t reg = _readReg(BD37033_REG_MUTE_GAIN);
    if (reg == 0xFF) return false;
    return (reg & BD37033_MUTE_BIT) != 0;
}

int8_t BD37033_getBalance() {
    uint8_t left = _readReg(BD37033_REG_FRONT_L);
    uint8_t right = _readReg(BD37033_REG_FRONT_R);
    if (left == 0xFF || right == 0xFF) return 0;
    int8_t leftDb = BD37033_volumeRegToDb(left);
    int8_t rightDb = BD37033_volumeRegToDb(right);
    if (leftDb < 0 && rightDb == 0) return -leftDb;
    if (rightDb < 0 && leftDb == 0) return rightDb;
    return 0;
}

int8_t BD37033_getBass() {
    uint8_t reg = _readReg(BD37033_REG_BASS_GAIN);
    if (reg == 0xFF) return 0;
    return BD37033_toneRegToDb(reg);
}

int8_t BD37033_getMiddle() {
    uint8_t reg = _readReg(BD37033_REG_MID_GAIN);
    if (reg == 0xFF) return 0;
    return BD37033_toneRegToDb(reg);
}

int8_t BD37033_getTreble() {
    uint8_t reg = _readReg(BD37033_REG_TREBLE_GAIN);
    if (reg == 0xFF) return 0;
    return BD37033_toneRegToDb(reg);
}

bool BD37033_getLoudness() {
    return _loudnessEnabled;
}

uint8_t BD37033_getInputGain() {
    uint8_t reg = _readReg(BD37033_REG_MUTE_GAIN);
    if (reg == 0xFF) return 0;
    return reg & BD37033_INPUT_GAIN_MASK;
}

bool BD37033_getInputType() {
    uint8_t reg = _readReg(BD37033_REG_INPUT_SELECT);
    if (reg == 0xFF) return false;
    return (reg & BD37033_INPUT_DIFF_MASK) != 0;
}

uint8_t BD37033_getSubLPF() {
    uint8_t reg = _readReg(BD37033_REG_SUBWOOFER);
    if (reg == 0xFF) return 0;
    return reg & BD37033_SUB_LPF_MASK;
}

bool BD37033_getSubPhase() {
    uint8_t reg = _readReg(BD37033_REG_SUBWOOFER);
    if (reg == 0xFF) return false;
    return (reg & BD37033_SUB_PHASE_MASK) != 0;
}

uint8_t BD37033_getSubInput() {
    uint8_t reg = _readReg(BD37033_REG_SUBWOOFER);
    if (reg == 0xFF) return 0;
    return (reg & BD37033_SUB_INPUT_MASK) ? 1 : 0;
}

uint8_t BD37033_getSubOutput() {
    uint8_t reg = _readReg(BD37033_REG_SUBWOOFER);
    if (reg == 0xFF) return 0;
    return (reg & BD37033_SUB_OUTPUT_MASK) ? 1 : 0;
}

int8_t BD37033_getMixingGain() {
    uint8_t reg = _readReg(BD37033_REG_MIX_GAIN);
    if (reg == 0xFF) return 0;
    return BD37033_volumeRegToDb(reg);
}

uint8_t BD37033_getMixingInput() {
    uint8_t reg = _readReg(BD37033_REG_MIXER);
    if (reg == 0xFF) return 0;
    return (reg >> 5) & 0x07;
}

uint8_t BD37033_getTrebleF0() {
    uint8_t reg = _readReg(BD37033_REG_TREBLE_FQ);
    if (reg == 0xFF) return 0;
    return (reg >> 4) & 0x03;
}

uint8_t BD37033_getTrebleQ() {
    uint8_t reg = _readReg(BD37033_REG_TREBLE_FQ);
    if (reg == 0xFF) return 0;
    return reg & 0x03;
}

// ==================================================
// DIAGNOSTICS
// ==================================================

uint8_t BD37033_readRegister(uint8_t reg) {
    return _readReg(reg);
}

void BD37033_printRegisters() {
    BD37033_LOG("\n--- BD37033 REGISTERS ---\n");
    BD37033_LOG("0x01: 0x%02X\n", _readReg(BD37033_REG_ADVANCED_SWITCH));
    BD37033_LOG("0x02: 0x%02X\n", _readReg(BD37033_REG_SUBWOOFER));
    BD37033_LOG("0x03: 0x%02X\n", _readReg(BD37033_REG_MIXER));
    BD37033_LOG("0x05: 0x%02X\n", _readReg(BD37033_REG_INPUT_SELECT));
    BD37033_LOG("0x06: 0x%02X\n", _readReg(BD37033_REG_MUTE_GAIN));
    BD37033_LOG("0x20: 0x%02X\n", _readReg(BD37033_REG_MASTER_VOLUME));
    BD37033_LOG("0x28: 0x%02X\n", _readReg(BD37033_REG_FRONT_L));
    BD37033_LOG("0x29: 0x%02X\n", _readReg(BD37033_REG_FRONT_R));
    BD37033_LOG("0x2A: 0x%02X\n", _readReg(BD37033_REG_REAR_L));
    BD37033_LOG("0x2B: 0x%02X\n", _readReg(BD37033_REG_REAR_R));
    BD37033_LOG("0x2C: 0x%02X\n", _readReg(BD37033_REG_SUB_L));
    BD37033_LOG("0x30: 0x%02X\n", _readReg(BD37033_REG_MIX_GAIN));
    BD37033_LOG("0x41: 0x%02X\n", _readReg(BD37033_REG_BASS_FQ));
    BD37033_LOG("0x44: 0x%02X\n", _readReg(BD37033_REG_MID_FQ));
    BD37033_LOG("0x47: 0x%02X\n", _readReg(BD37033_REG_TREBLE_FQ));
    BD37033_LOG("0x51: 0x%02X\n", _readReg(BD37033_REG_BASS_GAIN));
    BD37033_LOG("0x54: 0x%02X\n", _readReg(BD37033_REG_MID_GAIN));
    BD37033_LOG("0x57: 0x%02X\n", _readReg(BD37033_REG_TREBLE_GAIN));
    BD37033_LOG("0x75: 0x%02X\n", _readReg(BD37033_REG_LOUDNESS));
    BD37033_LOG("--------------------------\n\n");
}