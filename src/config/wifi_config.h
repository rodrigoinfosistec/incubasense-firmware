#pragma once

#include "app_config.h"

#if SIMULATION
    // Wokwi - Simulação
    const char* WIFI_SSID     = "Wokwi-GUEST";
    const char* WIFI_PASSWORD = "";
#else
    // Produção - ESP físico
    const char* WIFI_SSID     = "Redmi 12";
    const char* WIFI_PASSWORD = "JesusSenhor";
#endif
