#include <Wire.h>
#include <BD37033.h>

void setup() {
    Serial.begin(115200);
    Wire.begin();
    BD_init();
    
    // Основные настройки
    BD_setVolume(-20);
    BD_setInput(0);
    BD_setMute(false);
    BD_setBalance(0);
    
    // Тембры
    BD_setBass(3);
    BD_setMiddle(0);
    BD_setTreble(-2);
    
    // Расширенные настройки тембров
    BD_setBassF0(1);      // 80 Hz
    BD_setBassQ(1);       // 1.5
    BD_setMidF0(1);       // 1 kHz
    BD_setMidQ(1);        // 1.5
    
    // Loudness
    BD_setLoudness(true);
    BD_setLoudnessGain(10);
    BD_setLoudnessHiCut(1);
    
    // Входы
    BD_setInputGain(0);
    BD_setInputType(false);
    
    // Сабвуфер
    BD_setSubLPF(2);      // 85 Hz
    BD_setSubPhase(false);
    BD_setSubInput(0);
    BD_setSubOutput(0);
    
    // Микшер
    BD_setMixingGain(0);
    BD_setMixingInput(0);
    
    // Чтение и проверка
    Serial.print("Volume: "); Serial.println(BD_getVolume());
    Serial.print("Input: "); Serial.println(BD_getInput());
    Serial.print("Bass: "); Serial.println(BD_getBass());
    Serial.print("Loudness: "); Serial.println(BD_getLoudness());
}

void loop() {}