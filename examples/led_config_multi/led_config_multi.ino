// Defining this macro enables debug logging for
#define FASTLED_MANAGER_DEBUGLOG_ENABLE

// Disable FastLED setCorrection & setTemperature and make the calc faster
// #define FASTLED_MANAGER_DISABLE_COLOR_CORRECTIONS

#include <FastLEDManager.h>

static constexpr size_t N_LEDS {16};
static constexpr uint8_t PIN_WS2812_DATA {32};
static constexpr uint8_t PIN_SK6812_DATA {33};
uint8_t WS2812_BRIGHTNESS {192};
uint8_t SK6812_BRIGHTNESS {52};

void setup() {
    Serial.begin(115200);
    delay(2000);

    // These are the default configs
    $LED.add<WS2812B, N_LEDS, PIN_WS2812_DATA, GRB>("WS2812")
        .brightness(WS2812_BRIGHTNESS)
        .dither(BINARY_DITHER)
        .correction(CRGB::White)
        .temperature(CRGB::White)
        .fadeout(0)
        .layer(true)
        .divide()  // TODO: TBD (currently do nothing)
#ifdef ARDUINO_ARCH_ESP32
        .multi_core(true)
        .multi_core_config(0, 1024, 2)
#endif
        .startFps(120);

    // These are the default configs
    $LED.add<SK6812, N_LEDS, PIN_SK6812_DATA, GRB>("SK6812")
        .brightness(SK6812_BRIGHTNESS)
        .dither(BINARY_DITHER)
        .correction(CRGB::White)
        .temperature(CRGB::White)
        .fadeout(0)
        .layer(true)
        .divide()  // TODO: TBD (currently do nothing)
#ifdef ARDUINO_ARCH_ESP32
        .multi_core(true)
        .multi_core_config(0, 1024, 2)
#endif
        .startFps(120);

    // These are the default configs
    // $LED.add<APA102, N_LEDS, PIN_APA102_DATA, PIN_APA102_CLK, GRB, DATA_RATE_MHZ(12)>("APA102", 120)
    //    .brightness(APA102_BRIGHTNESS)
    //    .dither(BINARY_DITHER)
    //    .correction(CRGB::White)
    //    .temperature(CRGB::White)
    //    .fadeout(0)
    //    .layer(true)
    //    .divide()  // TODO: TBD (currently do nothing)
    //    .startFps(120);

    $LED["WS2812"]
        .mood_machine()
        ->startFpsForSec(30, 20);
    $LED["SK6812"]
        .mood_machine()
        ->startFpsForSec(30, 20);
}

void loop() {
    Tasks.update();  // automatically fadeout
}
