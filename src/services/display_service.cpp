#include "display_service.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C

static Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void DisplayService::begin() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println("Falha ao iniciar OLED");
        return;
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("IncubaSense");
    display.display();
}

void DisplayService::clear() {
    display.clearDisplay();
    display.setCursor(0, 0);
}

void DisplayService::show(
    const String& l1,
    const String& l2,
    const String& l3,
    const String& l4,
    const String& l5,
    const String& l6,
    const String& l7,
    const String& l8
) {
    clear();

    if (l1.length()) display.println(l1);
    if (l2.length()) display.println(l2);
    if (l3.length()) display.println(l3);
    if (l4.length()) display.println(l4);
    if (l5.length()) display.println(l5);
    if (l6.length()) display.println(l6);
    if (l7.length()) display.println(l7);
    if (l8.length()) display.println(l8);

    display.display();
}
