#ifndef LARP_LEDS
#define LARP_LEDS

#include <FastLED.h>
#include <Arduino.h>
#define BRIGHTNESS 64
#define COLOR_ORDER GRB
#define CHIPSET     WS2811


#define LED_PIN  5
const uint8_t kMatrixWidth = 16;
const uint8_t kMatrixHeight = 16;
const bool    kMatrixSerpentineLayout = false;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
class Led {
public:
    void led_setup();
    void led_loop(int mode);
    void setColorRGB(int r, int g, int b);
    int led_intensity;
    int current_scroll;
private:
    CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
    CRGB current_color;
    CRGB *led_leds = leds_plus_safety_pixel + 1;
    uint16_t XY( uint8_t x, uint8_t y);
    uint16_t XYsafe( uint8_t x, uint8_t y);
    void DrawOneFrame( byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8);
    void DrawNoise();
    void DrawScroll();
    void DrawIntensity();
    void DrawColor();
};
#endif
