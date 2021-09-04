#include <FastLEDManager.h>

static constexpr size_t N_LEDS {16};
static constexpr uint8_t PIN_WS2812_DATA {32};

void setup() {
    Serial.begin(115200);
    delay(2000);

    $LED.add<WS2812B, N_LEDS, PIN_WS2812_DATA, GRB>("WS2812")
        .startFps(120);

    $LED["WS2812"]
        .random()
        ->range(0, N_LEDS)
        ->color(CRGB::White)
        ->num_at_once(2)
        ->every_n_frame(3)
        ->fade_by(96)
        ->startFpsForSec(30, 10);
}

void loop() {
    Tasks.update();
}
