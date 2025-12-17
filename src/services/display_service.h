#pragma once

#include <Arduino.h>

class DisplayService {
public:
    void begin();

    // aceita até 6 linhas
    void show(
        const String& l1 = "",
        const String& l2 = "",
        const String& l3 = "",
        const String& l4 = "",
        const String& l5 = "",
        const String& l6 = "",
        const String& l7 = "",
        const String& l8 = ""
    );

private:
    void clear();
};
