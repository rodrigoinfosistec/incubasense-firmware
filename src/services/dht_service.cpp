#include "dht_service.h"
#include "../config/app_config.h"

#include <DHT.h>

#define DHT_TYPE DHT22

static DHT dht(DHT_22, DHT_TYPE);

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
