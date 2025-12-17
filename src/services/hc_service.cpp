#include "hc_service.h"
#include "../config/app_config.h"

void HcService::begin() {
    pinMode(NIVEL_TRIG, OUTPUT);
    pinMode(NIVEL_ECHO, INPUT);
    digitalWrite(NIVEL_TRIG, LOW);
}

HcData HcService::read() {
    // Pulso de trigger (10us)
    digitalWrite(NIVEL_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(NIVEL_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(NIVEL_TRIG, LOW);

    // Leitura do echo (timeout para segurança)
    unsigned long duration = pulseIn(NIVEL_ECHO, HIGH, 30000UL); // ~5m

    if (duration == 0) {
        lastData.valid = false;
        return lastData;
    }

    // Conversão para cm
    float distance = (duration * 0.0343f) / 2.0f;

    lastData.distanceCm = distance;
    lastData.valid = true;

    return lastData;
}
