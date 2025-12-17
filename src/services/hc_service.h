#pragma once

#include <Arduino.h>

struct HcData {
    float distanceCm;
    bool valid;
};

class HcService {
public:
    void begin();
    HcData read();

private:
    HcData lastData{};
};
