#include <FastLEDManager.h>

static constexpr size_t N_LEDS {16};
static constexpr uint8_t PIN_WS2812_DATA {MOSI};

void setup() {
    Serial.begin(115200);
    delay(2000);

    LEDMNGR.add<WS2812B, N_LEDS, PIN_WS2812_DATA, GRB>("WS2812")
        .startFps(120);

    LEDMNGR["WS2812"]
        .progress("Progress")
        ->color(CRGB::Blue)
        ->ratio(0.)
        ->startFpsForSec(30., 15.);
}

void loop() {
    Tasks.update();

    static uint32_t start_ms = millis();
    float percent = (float)(millis() - start_ms) / 10000.f * 100.f;

    if (LEDMNGR["WS2812"]["Progress"])
        LEDMNGR["WS2812"].progress_percent("Progress", percent);
}
