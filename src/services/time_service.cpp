#include "time_service.h"
#include "../config/ntp_config.h"
#include "ntp_service.h"

#include <time.h>

static bool synced = false;

/* ================= EXISTENTE ================= */

bool timeSyncTick() {
    if (synced) return true;

    if (sincronizarTempoNTP()) {
        synced = true;
        return true;
    }
    return false;
}

bool timeIsSynced() {
    return synced;
}

String getDateTimeNow() {
    if (!synced) return "";

    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return "";
    }

    char buffer[25];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", &timeinfo);
    return String(buffer);
}

unsigned long getEpochNow() {
    if (!synced) return 0;

    time_t now;
    time(&now);
    return (unsigned long)now;
}

/* ================= NOVO ================= */

// Converte "dd/mm/yyyy HH:MM:SS" em time_t
time_t parseDateTime(const String& datetime) {
    if (datetime.length() == 0) return 0;

    struct tm tm {};
    sscanf(
        datetime.c_str(),
        "%d/%d/%d %d:%d:%d",
        &tm.tm_mday,
        &tm.tm_mon,
        &tm.tm_year,
        &tm.tm_hour,
        &tm.tm_min,
        &tm.tm_sec
    );

    tm.tm_mon  -= 1;      // mês começa em 0
    tm.tm_year -= 1900;   // anos desde 1900

    return mktime(&tm);
}

// Quantos dias se passaram desde a data informada
uint32_t daysSince(const String& datetime) {
    if (!synced) return 0;

    time_t start = parseDateTime(datetime);
    time_t now;
    time(&now);

    if (start == 0 || now == 0) return 0;

    double diffSeconds = difftime(now, start);
    return diffSeconds / (60 * 60 * 24);
}
