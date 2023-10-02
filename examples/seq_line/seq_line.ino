#include <FastLEDManager.h>

static constexpr size_t N_LEDS {16};
static constexpr uint8_t PIN_WS2812_DATA {MOSI};

void setup() {
    Serial.begin(115200);
    delay(2000);

    LEDMNGR.add<WS2812B, N_LEDS, PIN_WS2812_DATA, GRB>("WS2812")
        .startFps(120);

    LEDMNGR["WS2812"]
        .line()
        ->color(CRGB::Blue)
        ->pixel_delay(100)
        ->fadeout_ms(400)
        ->reverse(false)
        ->repeat(true)
        ->startFpsForFrame(30, 30 * 10);
}

void loop() {
    Tasks.update();
}
