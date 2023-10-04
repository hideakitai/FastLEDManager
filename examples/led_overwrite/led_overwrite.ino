#include <FastLEDManager.h>

static constexpr size_t N_LEDS {16};
static constexpr uint8_t PIN_WS2812_DATA {MOSI};

CRGBArray<N_LEDS> rgb;

void setup() {
    Serial.begin(115200);
    delay(2000);

    LEDMNGR.add<WS2812B, N_LEDS, PIN_WS2812_DATA, GRB>("WS2812")
        .startFps(120);

    LEDMNGR["WS2812"]
        .mood_machine()
        ->startFps(30);

    LEDMNGR["WS2812"]
        .overwrite<LEDSequence::Fill>()
        ->color(CRGB::Red, 0, 5)
        ->fade_by(5)
        ->startFpsForSec(30, 5);
}

void loop() {
    Tasks.update();
}
