#include "dht_service.h"
#include "../config/app_config.h"

#include <DHT.h>

#if SIMULATION
  #define DHT_TYPE DHT22
#else
  #define DHT_TYPE DHT11
#endif

static DHT dht(TEMPUMID, DHT_TYPE);

void DhtService::begin() {
    dht.begin();
}

DhtData DhtService::read() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (!isnan(h) && !isnan(t)) {
        lastData.temperature = t;
        lastData.humidity = h;
        lastData.valid = true;
    } else {
        lastData.valid = false;
    }

    return lastData;
}
