#include <FastLEDManager.h>

static constexpr size_t N_LEDS {16};
static constexpr uint8_t PIN_WS2812_DATA {32};

void setup() {
    Serial.begin(115200);
    delay(2000);

    $LED.add<WS2812B, N_LEDS, PIN_WS2812_DATA, GRB>("WS2812")
        .startFps(120);

    $LED["WS2812"]
        .flash()
        ->color(CRGB::Red)
        ->cycle_ms(1000)
        ->half_cycle_ms(500)  // same as above
        ->startFpsForFrame(30, 30 * 10);
}

void loop() {
    Tasks.update();
}
