#include <FastLEDManager.h>

static constexpr size_t N_LEDS {16};
static constexpr uint8_t PIN_WS2812_DATA {MOSI};

void setup() {
    Serial.begin(115200);
    delay(2000);

    LEDMNGR.add<WS2812B, N_LEDS, PIN_WS2812_DATA, GRB>("WS2812")
        .startFps(120);

    LEDMNGR["WS2812"]
        .tween_solid()
        ->tween([&](Tween::Sequence<CRGB>& seq) {
            seq.then(CRGB::Red, 2000)
                .then(CRGB::Green, 2000)
                .then(CRGB::Blue, 2000)
                .then(CRGB::White, 2000)
                .then(CRGB::Black, 2000);
        })
        ->auto_erase(false)
        ->mode(Tween::Mode::ONCE)
        ->startFpsForSec(30, 10);
}

void loop() {
    Tasks.update();
}
