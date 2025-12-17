#pragma once
#include <Arduino.h>

class BuzzerService {
private:
    uint8_t _pin;
    bool _blinking = false;
    bool _state = false;
    unsigned long _interval = 1000;
    unsigned long _lastToggle = 0;

public:
    void begin(uint8_t pin) {
        _pin = pin;
        pinMode(_pin, OUTPUT);
        off();
    }

    void on() {
        _blinking = false;
        digitalWrite(_pin, HIGH); // buzzer ativo
        _state = true;
    }

    void off() {
        _blinking = false;
        digitalWrite(_pin, LOW);
        _state = false;
    }

    void blink(unsigned long intervalMs = 1000) {
        _blinking = true;
        _interval = intervalMs;
    }

    void update() {
        if (!_blinking) return;

        unsigned long now = millis();
        if (now - _lastToggle >= _interval) {
            _lastToggle = now;
            _state = !_state;
            digitalWrite(_pin, _state ? HIGH : LOW);
        }
    }
};
