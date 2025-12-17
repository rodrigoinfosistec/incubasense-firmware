#pragma once

#include <time.h>
#include <Arduino.h>

/*
 Estados internos:
 - retorna false enquanto ainda está tentando
 - retorna true quando sincronizar
 - retorna false definitivo quando exceder tentativas
*/

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
        // sucesso → reseta estado interno
        attempts = 0;
        return true;
    }

    // falha
    attempts++;
    Serial.println("Falha ao obter tempo NTP, tentando novamente...");

    return false;
}

