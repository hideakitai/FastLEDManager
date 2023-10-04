#include <FastLEDManager.h>

static constexpr size_t N_LEDS {16};
static constexpr uint8_t PIN_WS2812_DATA {MOSI};

CRGBArray<N_LEDS> r;
CRGBArray<N_LEDS> g;
CRGBArray<N_LEDS> b;
CRGBArray<N_LEDS> w;

void setup() {
    Serial.begin(115200);
    delay(2000);

    for (size_t i = 0; i < N_LEDS; ++i) {
        r[i] = CRGB(random(255), 0, 0);
        g[i] = CRGB(0, random(255), 0);
        b[i] = CRGB(0, 0, random(255));
        uint8_t v = random(255);
        w[i] = CRGB(v, v, v);
    }

    LEDMNGR.add<WS2812B, N_LEDS, PIN_WS2812_DATA, GRB>("WS2812")
        .startFps(120);

    LEDMNGR["WS2812"]
        .tween_each()
        ->tween([&](Tween::Sequence<CRGB>& seq, const size_t i) {
            seq.then(r[i], 2000)
                .then(g[i], 2000)
                .then(b[i], 2000)
                .then(w[i], 2000)
                .then(CRGB::Black, 2000);
        })
        ->auto_erase(false)
        ->mode(Tween::Mode::ONCE)
        ->startFpsForSec(30, 10);
}

void loop() {
    Tasks.update();
}
