#include <FastLEDManager.h>

static constexpr size_t N_LEDS {16};
static constexpr uint8_t PIN_WS2812_DATA {32};

void setup() {
    Serial.begin(115200);
    delay(2000);

    $LED.add<WS2812B, N_LEDS, PIN_WS2812_DATA, GRB>("WS2812")
        .startFps(120);

    using namespace LEDSequence;
    $LED["WS2812"]
        .sequence()
        ->sync<Fill>([&](TaskRef<Fill> t) {
            t->color(CRGB(32, 0, 0));
        })
        ->sync<Random>([&](TaskRef<Random> t) {
            t->range(0, N_LEDS)
                ->color(CRGB(128, 128, 128))
                ->num_at_once(2)
                ->every_n_frame(3)
                ->fade_by(96);
        })
        ->sync<Triangle>([&](TaskRef<Triangle> t) {
            t->color(CRGB(0, 128, 0))
                ->cycle_ms(1000);
        })
        ->sync<Line>([&](TaskRef<Line> t) {
            t->color(CRGB(0, 0, 128))
                ->pixel_delay(100)
                ->fadeout_ms(400)
                ->reverse(false)
                ->repeat(true);
        })
        ->startFpsForSec(30, 10.);
}

void loop() {
    Tasks.update();  // automatically fadeout
}
