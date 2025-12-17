#include "ntp_service.h"

#include <Arduino.h>
#include <time.h>

bool sincronizarTempoNTP() {
    static unsigned long lastAttempt = 0;
    static int attempts = 0;

    const unsigned long INTERVALO_TENTATIVA = 1000; // 1s
    const int MAX_TENTATIVAS = 10;

    // já excedeu tentativas
    if (attempts >= MAX_TENTATIVAS) {
        return false;
    }

    // respeita intervalo
    if (millis() - lastAttempt < INTERVALO_TENTATIVA) {
        return false;
    }

    lastAttempt = millis();

    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
        attempts = 0;
        return true;
    }

    attempts++;

#if SERIAL_OPERAND
    Serial.println("Falha ao obter tempo NTP, tentando novamente...");
#endif

    return false;
}
