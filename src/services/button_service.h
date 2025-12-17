#pragma once
#include <Arduino.h>

enum class ButtonEvent {
    None,
    SingleClick,
    DoubleClick,
    LongPress
};

class ButtonService {
private:
    uint8_t _pin;

    bool _lastState = HIGH;
    bool _pressed = false;
    bool _ignoreUntilRelease = false;

    unsigned long _pressTime   = 0;
    unsigned long _lastRelease = 0;
    unsigned long _lastChange  = 0;

    uint8_t _clickCount = 0;

    const unsigned long DEBOUNCE_MS     = 40;
    const unsigned long DOUBLE_CLICK_MS = 400;
    const unsigned long LONG_PRESS_MS   = 5000;

public:
    void begin(uint8_t pin) {
        _pin = pin;
        pinMode(_pin, INPUT_PULLUP);
    }

    ButtonEvent update() {
        bool state = digitalRead(_pin);
        unsigned long now = millis();

        // --- debounce ---
        if (state != _lastState) {
            _lastChange = now;
            _lastState = state;
            return ButtonEvent::None;
        }

        if (now - _lastChange < DEBOUNCE_MS) {
            return ButtonEvent::None;
        }

        // --- bloqueio pós-long-press ---
        if (_ignoreUntilRelease) {
            if (state == HIGH) {
                // liberou completamente
                _ignoreUntilRelease = false;
            }
            return ButtonEvent::None;
        }

        // --- pressionou ---
        if (!_pressed && state == LOW) {
            _pressed = true;
            _pressTime = now;
            return ButtonEvent::None;
        }

        // --- long press ---
        if (_pressed && state == LOW && (now - _pressTime >= LONG_PRESS_MS)) {
            _pressed = false;
            _clickCount = 0;
            _ignoreUntilRelease = true; // 🔒 trava tudo até soltar
            return ButtonEvent::LongPress;
        }

        // --- soltou ---
        if (_pressed && state == HIGH) {
            _pressed = false;
            _clickCount++;
            _lastRelease = now;
            return ButtonEvent::None;
        }

        // --- decisão single / double ---
        if (_clickCount > 0 && (now - _lastRelease > DOUBLE_CLICK_MS)) {
            ButtonEvent ev =
                (_clickCount == 1)
                    ? ButtonEvent::SingleClick
                    : ButtonEvent::DoubleClick;

            _clickCount = 0;
            return ev;
        }

        return ButtonEvent::None;
    }
};
