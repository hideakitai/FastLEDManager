#include <FastLEDManager.h>

static constexpr size_t N_LEDS {16};
static constexpr uint8_t PIN_WS2812_DATA {MOSI};

void setup() {
    Serial.begin(115200);
    delay(2000);

    LEDMNGR.add<WS2812B, N_LEDS, PIN_WS2812_DATA, GRB>("WS2812")
        .startFps(120);

    // Blocking LED control
    LEDMNGR.fill_solid(CRGB::Black)  // fill_solid to all led stripes
        .delay(500)                // show current buffer and suspend
        .fill_solid(CRGB::Red)
        .delay(500)
        .fill_solid(CRGB::Green)
        .delay(500)
        .fill_solid(CRGB::Blue)
        .delay(500)
        .fill_solid(CRGB::White)
        .delay(500)
        .fill_solid(CRGB::Black)
        .show();

    Serial.println("Finish");
}

void loop() {
    Tasks.update();
}
