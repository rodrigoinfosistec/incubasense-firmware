#include "hc_service.h"
#include "../config/app_config.h"

void HcService::begin() {
    pinMode(HC_TRIG, OUTPUT);
    pinMode(HC_ECHO, INPUT);
    digitalWrite(HC_TRIG, LOW);
}

HcData HcService::read() {
    // Pulso de trigger (10us)
    digitalWrite(HC_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(HC_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(HC_TRIG, LOW);

    // Leitura do echo (timeout para segurança)
    unsigned long duration = pulseIn(HC_ECHO, HIGH, 30000UL); // ~5m

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
