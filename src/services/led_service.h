#pragma once

#include <Arduino.h>

class Led {
public:
    explicit Led(uint8_t pin);

    void begin();

    void on();
    void off();
    void blink(unsigned long intervalMs = 500);

    void update(); // chamada contínua no loop

private:
    uint8_t _pin;
    bool _blinking = false;
    bool _state = false;
    unsigned long _interval = 500;
    unsigned long _lastToggle = 0;
};
