#include <FastLEDManager.h>

static constexpr size_t N_LEDS {16};
static constexpr uint8_t PIN_WS2812_DATA {32};

void setup() {
    Serial.begin(115200);
    delay(2000);

    $LED.add<WS2812B, N_LEDS, PIN_WS2812_DATA, GRB>("WS2812")
        .startFps(120);

    $LED["WS2812"]
        .rgbw()
        ->duration(1000)
        ->repeat(false)
        ->startFpsForSec(30, 5);

    // block program but runs sequence in background
    $LED["WS2812"].spin(5000);

    $LED["WS2812"]
        .rrggbbww()
        ->duration(1000)
        ->repeat(false)
        ->startFpsForSec(30, 8);

    // block program but runs sequence in background
    $LED["WS2812"].spin(9000);
}

void loop() {
    Tasks.update();
}
