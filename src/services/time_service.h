#pragma once

#include <Arduino.h>
#include <time.h>

// --- já existentes ---
bool timeSyncTick();
bool timeIsSynced();

String getDateTimeNow();
unsigned long getEpochNow();

// --- NOVAS FUNÇÕES ---
time_t parseDateTime(const String& datetime);
uint32_t daysSince(const String& datetime);
