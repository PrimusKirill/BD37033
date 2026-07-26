/*
 * BD37033_registers.h
 * РЕГИСТРЫ И КОНСТАНТЫ ДЛЯ BD37033FV-M
 * 
 * Версия: 1.0.0
 * Дата: 26.07.2026
 * 
 * Полное описание всех регистров, битов и масок
 * для аудиопроцессора BD37033FV-M.
 * 
 * Ссылка на даташит:
 * https://fscdn.rohm.com/en/products/databook/datasheet/ic/audio_video/audio_processor/bd37033fv-m-e.pdf
 */

#ifndef BD37033_REGISTERS_H
#define BD37033_REGISTERS_H

#include <Arduino.h>

// ==================================================
// БАЗОВЫЙ АДРЕС I2C
// ==================================================
#define BD37033_ADDR_DEFAULT    0x40

// ==================================================
// РЕГИСТРЫ УПРАВЛЕНИЯ
// ==================================================
#define BD37033_REG_ADVANCED_SWITCH  0x01    // Advanced Switch + Anti-Alias Filter
#define BD37033_REG_SUBWOOFER        0x02    // Subwoofer Control
#define BD37033_REG_MIXER            0x03    // Mixer Control
#define BD37033_REG_INPUT_SELECT     0x05    // Input Selector
#define BD37033_REG_MUTE_GAIN        0x06    // Mute + Input Gain
#define BD37033_REG_MASTER_VOLUME    0x20    // Master Volume

// ==================================================
// РЕГИСТРЫ КАНАЛЬНЫХ ГРОМКОСТЕЙ
// ==================================================
#define BD37033_REG_FRONT_L          0x28    // Front Left
#define BD37033_REG_FRONT_R          0x29    // Front Right
#define BD37033_REG_REAR_L           0x2A    // Rear Left
#define BD37033_REG_REAR_R           0x2B    // Rear Right
#define BD37033_REG_SUB_L            0x2C    // Subwoofer Left
#define BD37033_REG_MIX_GAIN         0x30    // Mixer Gain

// ==================================================
// РЕГИСТРЫ ТЕМБРОВ
// ==================================================
#define BD37033_REG_BASS_FQ          0x41    // Bass: F0 + Q
#define BD37033_REG_MID_FQ           0x44    // Middle: F0 + Q
#define BD37033_REG_TREBLE_FQ        0x47    // Treble: F0 + Q
#define BD37033_REG_BASS_GAIN        0x51    // Bass Gain
#define BD37033_REG_MID_GAIN         0x54    // Middle Gain
#define BD37033_REG_TREBLE_GAIN      0x57    // Treble Gain

// ==================================================
// РЕГИСТР LOUDNESS
// ==================================================
#define BD37033_REG_LOUDNESS         0x75    // Loudness (Gain + HiCut)

// ==================================================
// БИТЫ И МАСКИ: ADVANCED SWITCH
// ==================================================
#define BD37033_SWITCH_ADVANCED_EN   (1 << 7)
#define BD37033_SWITCH_ANTIALIAS_EN  (1 << 6)

// ==================================================
// БИТЫ И МАСКИ: SUBWOOFER
// ==================================================
#define BD37033_SUB_PHASE_MASK       0x80
#define BD37033_SUB_OUTPUT_MASK      0x20
#define BD37033_SUB_INPUT_MASK       0x10
#define BD37033_SUB_LPF_MASK         0x0F

// ==================================================
// БИТЫ И МАСКИ: INPUT SELECT
// ==================================================
#define BD37033_INPUT_DIFF_MASK      0x80
#define BD37033_INPUT_SEL_MASK       0x0F

#define BD37033_INPUT_A              0x00
#define BD37033_INPUT_B              0x01
#define BD37033_INPUT_C              0x02
#define BD37033_INPUT_D              0x03
#define BD37033_INPUT_E              0x0A

// ==================================================
// БИТЫ И МАСКИ: MUTE + GAIN
// ==================================================
#define BD37033_MUTE_BIT             0x80
#define BD37033_INPUT_GAIN_MASK      0x1F

// ==================================================
// БИТЫ И МАСКИ: LOUDNESS
// ==================================================
#define BD37033_LOUDNESS_HICUT_MASK  0x60
#define BD37033_LOUDNESS_GAIN_MASK   0x1F

// ==================================================
// КОНСТАНТЫ ДЛЯ ЧАСТОТ (F0)
// ==================================================
#define BD37033_BASS_F0_60HZ         0
#define BD37033_BASS_F0_80HZ         1
#define BD37033_BASS_F0_100HZ        2
#define BD37033_BASS_F0_120HZ        3

#define BD37033_MID_F0_500HZ         0
#define BD37033_MID_F0_1KHZ          1
#define BD37033_MID_F0_1_5KHZ        2
#define BD37033_MID_F0_2_5KHZ        3

#define BD37033_TREBLE_F0_7_5KHZ     0
#define BD37033_TREBLE_F0_10KHZ      1
#define BD37033_TREBLE_F0_12_5KHZ    2
#define BD37033_TREBLE_F0_15KHZ      3

// ==================================================
// КОНСТАНТЫ ДЛЯ ДОБРОТНОСТИ Q
// ==================================================
#define BD37033_BASS_Q_1_0           0
#define BD37033_BASS_Q_1_5           1
#define BD37033_BASS_Q_2_0           2

#define BD37033_MID_Q_0_75           0
#define BD37033_MID_Q_1_0            1
#define BD37033_MID_Q_1_25           2

#define BD37033_TREBLE_Q_0_75        0
#define BD37033_TREBLE_Q_1_25        1

// ==================================================
// КОНСТАНТЫ ДЛЯ LOUDNESS HiCut
// ==================================================
#define BD37033_LOUDNESS_HICUT_1_2KHZ    0
#define BD37033_LOUDNESS_HICUT_2_4KHZ    1
#define BD37033_LOUDNESS_HICUT_4_0KHZ    2

// ==================================================
// КОНСТАНТЫ ДЛЯ SUBWOOFER LPF
// ==================================================
#define BD37033_SUB_LPF_OFF          0
#define BD37033_SUB_LPF_55HZ         1
#define BD37033_SUB_LPF_85HZ         2
#define BD37033_SUB_LPF_120HZ        3
#define BD37033_SUB_LPF_160HZ        4

// ==================================================
// ВСПОМОГАТЕЛЬНЫЕ МАКРОСЫ
// ==================================================

// Формирование регистра для частоты и Q
#define BD37033_MAKE_FQ_REG(f0, q)   (((f0) << 4) | (q))

// Извлечение частоты и Q из регистра
#define BD37033_GET_F0(reg)          (((reg) >> 4) & 0x03)
#define BD37033_GET_Q(reg)           ((reg) & 0x03)

// Формирование регистра для Loudness
#define BD37033_MAKE_LOUDNESS_REG(gain, hicut)   (((hicut) << 5) | (gain))

#endif // BD37033_REGISTERS_H