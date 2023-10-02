#include <FastLEDManager.h>

static constexpr size_t N_LEDS {16};
static constexpr uint8_t PIN_WS2812_DATA {MOSI};

void setup() {
    Serial.begin(115200);
    delay(2000);

    LEDMNGR.add<WS2812B, N_LEDS, PIN_WS2812_DATA, GRB>("WS2812")
        .startFps(120);

    // Get CPixelView<CRGB> from "WS2812" Controller
    // This is almost same as CRGBArray<N_LEDS>
    auto& leds = LEDMNGR["WS2812"].get_pixel_view();

    // One way to handle led buffer manually is to call delay()
    // delay() shows current led buffer once and suspend program
    leds.fill_solid(CRGB::Black);
    LEDMNGR.delay(1000);
    leds.fill_solid(CRGB::Red);
    LEDMNGR.delay(1000);
    leds.fill_solid(CRGB::Green);
    LEDMNGR.delay(1000);
    leds.fill_solid(CRGB::Blue);
    LEDMNGR.delay(1000);
    leds.fill_solid(CRGB::White);
    LEDMNGR.delay(1000);
    leds.fill_solid(CRGB::Black);
    LEDMNGR.show();

    // Another way to handle it is to disable layer first
    // If layer is enabled, output buffer is cleared every frame
    // to accumlate all tasks pixels into output buffer
    // spin() blocks program but led tasks continue to run in the background
    LEDMNGR["WS2812"].layer(false);
    leds.fill_solid(CRGB::Black);
    LEDMNGR.spin(1000);
    leds.fill_solid(CRGB::Red);
    LEDMNGR.spin(1000);
    leds.fill_solid(CRGB::Green);
    LEDMNGR.spin(1000);
    leds.fill_solid(CRGB::Blue);
    LEDMNGR.spin(1000);
    leds.fill_solid(CRGB::White);
    LEDMNGR.spin(1000);
    leds.fill_solid(CRGB::Black);
    LEDMNGR.show();
}

void loop() {
    Tasks.update();  // automatically fadeout
}
