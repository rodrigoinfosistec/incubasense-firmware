#include "led_service.h"

Led::Led(uint8_t pin)
    : _pin(pin) {}

void Led::begin() {
    pinMode(_pin, OUTPUT);
    off();
}

void Led::on() {
    _blinking = false;
    digitalWrite(_pin, LOW);
    _state = true;
}

void Led::off() {
    _blinking = false;
    digitalWrite(_pin, HIGH);
    _state = false;
}

void Led::blink(unsigned long intervalMs) {
    _blinking = true;
    _interval = intervalMs;

    // garante estado inicial consistente
    _state = false;
    digitalWrite(_pin, HIGH); // LED desligado (ativo em LOW)

    // marca o tempo inicial corretamente
    _lastToggle = millis();
}

void Led::update() {
    if (!_blinking) return;

    unsigned long now = millis();
    if (now - _lastToggle >= _interval) {
        _lastToggle = now;
        _state = !_state;
        digitalWrite(_pin, _state ? LOW : HIGH);
    }
}
