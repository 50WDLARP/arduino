#include "led.h"
#include <Arduino.h>


uint16_t Led::XY( uint8_t x, uint8_t y)
{
    uint16_t i;

    if ( kMatrixSerpentineLayout == false) {
        i = (y * kMatrixWidth) + x;
    }

    if ( kMatrixSerpentineLayout == true) {
        if ( y & 0x01) {
            // Odd rows run backwards
            uint8_t reverseX = (kMatrixWidth - 1) - x;
            i = (y * kMatrixWidth) + reverseX;
        } else {
            // Even rows run forwards
            i = (y * kMatrixWidth) + x;
        }
    }

    return i;
}
uint16_t Led::XYsafe( uint8_t x, uint8_t y)
{
    if ( x >= kMatrixWidth) return -1;
    if ( y >= kMatrixHeight) return -1;
    return XY(x, y);
}

void Led::DrawOneFrame( byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8)
{
    byte lineStartHue = startHue8;
    for ( byte y = 0; y < kMatrixHeight; y++) {
        lineStartHue += yHueDelta8;
        byte pixelHue = lineStartHue;
        for ( byte x = 0; x < kMatrixWidth; x++) {
            pixelHue += xHueDelta8;
            led_leds[ XY(x, y)]  = CHSV( pixelHue, 255, 255);
        }
    }
}

void Led::DrawNoise() {
    for (byte y = 0; y < kMatrixHeight; y++) {
        for (byte x = 0; x < kMatrixWidth; x++) {
            led_leds [ XY ( x, y)] = CHSV(random(0, 255), 255, 255);

        }
    }
}

void Led::DrawScroll(int current) {
    int i = 0;
    for (byte y = 0; y < kMatrixHeight; y++) {
        for (byte x = 0; x < kMatrixWidth; x++) {
            led_leds[ XY(x, y)] = CHSV(0, 0, 0);
            if (i == current) {
                led_leds [ XY ( x, y)] = CHSV(255, 255, 255);
            }
            i++;
        }
    }
}

void Led::DrawIntensity() {
    int i = 0;
    for (int y = 0; y < kMatrixHeight; y++) {
        for (int x = 0; x < kMatrixWidth; x++) {
            if (i < led_intensity) {
                led_leds [ XY (x, y)] = CHSV(i * 5, 255, 255);
            } else {
                led_leds [ XY (x, y)] = CHSV(0, 0, 0);
            }
            i++;
        }
    }

}

void Led::led_setup() {

    FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(led_leds, NUM_LEDS).setCorrection(TypicalSMD5050);
    FastLED.setBrightness( BRIGHTNESS );
}

void Led::led_loop(int mode) {

    uint32_t ms = millis();
    int32_t yHueDelta32 = ((int32_t)cos16( ms * (27 / 1) ) * (350 / kMatrixWidth));
    int32_t xHueDelta32 = ((int32_t)cos16( ms * (39 / 1) ) * (310 / kMatrixHeight));
    switch (mode) {
        case 0:
            DrawIntensity();
            break;
        case 1:
            DrawNoise();
            break;
        default:
            break;
    }

    FastLED.setBrightness(BRIGHTNESS);

    FastLED.show();
}
