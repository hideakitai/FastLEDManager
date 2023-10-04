#include <FastLEDManager.h>

static constexpr size_t N_LEDS {16};
static constexpr uint8_t PIN_WS2812_DATA {MOSI};

CRGBArray<N_LEDS> rgb;

void setup() {
    Serial.begin(115200);
    delay(2000);

    LEDMNGR.add<WS2812B, N_LEDS, PIN_WS2812_DATA, GRB>("WS2812")
        .startFps(120);

    using namespace LEDSequence;
    LEDMNGR["WS2812"]
        .sequence("sq")  // create a series of sequences
        ->then<Fill>(1, [&](TaskRef<Fill> t) { t->color(CRGB::Black); })
        ->then<Fill>(2, [&](TaskRef<Fill> t) { t->color(CRGB(64, 0, 0)); })
        ->then<Fill>(2, [&](TaskRef<Fill> t) { t->color(CRGB(0, 64, 0)); })
        ->then<Fill>(2, [&](TaskRef<Fill> t) { t->color(CRGB(0, 0, 64)); })
        ->then<Fill>(2, [&](TaskRef<Fill> t) { t->color(CRGB(64, 64, 64)); })
        ->then<Fill>(1, [&](TaskRef<Fill> t) { t->color(CRGB::Black); })
        ->startFpsForSec(30, 10., true);

    LEDMNGR["WS2812"]
        .random()
        ->range(0, N_LEDS)
        ->color(CRGB::White)
        ->num_at_once(2)
        ->every_n_frame(3)
        ->fade_by(96)
        ->startFpsForSec(30, 10, true);
}

void loop() {
    rgb.fill_rainbow(uint8_t(float(millis() % 3000) / 3000.f * 255.f));
    rgb.nscale8(48);

    // This the base layer
    // NOTE: if this framerate is less than LEDController's framerate,
    // it causes flicker because assignment is valid only on frame
    // if you want not to flicker, please use attach() instead
    LEDMNGR["WS2812"].assign(rgb);

    Tasks.update();
}
