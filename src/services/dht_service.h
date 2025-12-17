#pragma once

#include <Arduino.h>

struct DhtData {
    float temperature;
    float humidity;
    bool valid;
};

class DhtService {
public:
    void begin();
    DhtData read();

private:
    DhtData lastData{};
};
