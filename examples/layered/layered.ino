#include <FastLEDManager.h>

static constexpr size_t N_LEDS {16};
static constexpr uint8_t PIN_WS2812_DATA {MOSI};
uint8_t WS2812_BRIGHTNESS {192};

void setup() {
    Serial.begin(115200);
    delay(2000);

    LEDMNGR.add<WS2812B, N_LEDS, PIN_WS2812_DATA, GRB>("WS2812")
        .layer(true)  // default: true
        .startFps(120);

    LEDMNGR["WS2812"]
        .fill()
        ->color(CRGB(32, 0, 0))
        ->startFpsForSec(30, 10.);

    LEDMNGR["WS2812"]
        .random()
        ->range(0, N_LEDS)
        ->color(CRGB::White)
        ->num_at_once(2)
        ->every_n_frame(3)
        ->fade_by(96)
        ->startFpsForSec(30, 10.);

    LEDMNGR["WS2812"]
        .triangle()
        ->color(CRGB::Green)
        ->cycle_ms(1000)
        ->startFpsForSec(30, 10.);

    // If layer is disabled, you can only see this sequence
    // Others are overwritten by this sequence
    LEDMNGR["WS2812"]
        .line()
        ->color(CRGB::Blue)
        ->pixel_delay(100)
        ->fadeout_ms(400)
        ->reverse(false)
        ->repeat(true)
        ->startFpsForSec(30, 10.);
}

void loop() {
    Tasks.update();  // automatically fadeout
}
