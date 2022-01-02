#include <FastLEDManager.h>

static constexpr size_t N_LEDS {16};
static constexpr uint8_t PIN_WS2812_DATA {32};

void setup() {
    Serial.begin(115200);
    delay(2000);

    $LED.add<WS2812B, N_LEDS, PIN_WS2812_DATA, GRB>("WS2812")
        .startFps(120);

    // Get CPixelView<CRGB> from "WS2812" Controller
    // This is almost same as CRGBArray<N_LEDS>
    auto& leds = $LED["WS2812"].get_pixel_view();

    // One way to handle led buffer manually is to call hold()
    // hold() shows current led buffer once and suspend program
    leds.fill_solid(CRGB::Black);
    $LED.hold(1000);
    leds.fill_solid(CRGB::Red);
    $LED.hold(1000);
    leds.fill_solid(CRGB::Green);
    $LED.hold(1000);
    leds.fill_solid(CRGB::Blue);
    $LED.hold(1000);
    leds.fill_solid(CRGB::White);
    $LED.hold(1000);
    leds.fill_solid(CRGB::Black);
    $LED.show();

    // Another way to handle it is to disable layer first
    // If layer is enabled, output buffer is cleared every frame
    // to accumlate all tasks pixels into output buffer
    // spin() blocks program but led tasks continue to run in the background
    $LED["WS2812"].layer(false);
    leds.fill_solid(CRGB::Black);
    $LED.spin(1000);
    leds.fill_solid(CRGB::Red);
    $LED.spin(1000);
    leds.fill_solid(CRGB::Green);
    $LED.spin(1000);
    leds.fill_solid(CRGB::Blue);
    $LED.spin(1000);
    leds.fill_solid(CRGB::White);
    $LED.spin(1000);
    leds.fill_solid(CRGB::Black);
    $LED.show();
}

void loop() {
    Tasks.update();  // automatically fadeout
}
