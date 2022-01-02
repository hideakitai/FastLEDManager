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
        .sequence("Sequence")  // create a series of sequences
        ->then<Fill>("Black", 1, [&](TaskRef<Fill> t) { t->color(CRGB::Black); })
        ->then<Fill>("Red", 1, [&](TaskRef<Fill> t) { t->color(CRGB::Red); })
        ->spin(1)
        ->then<Fill>("Green", 1, [&](TaskRef<Fill> t) { t->color(CRGB::Green); })
        ->spin(1)
        ->then<Fill>("Blue", 1, [&](TaskRef<Fill> t) { t->color(CRGB::Blue); })
        ->then<Fill>("White", 1, [&](TaskRef<Fill> t) { t->color(CRGB::White); })
        ->then<Fill>("Black2", 1, [&](TaskRef<Fill> t) { t->color(CRGB::Black); })
        ->startFpsForSec(30, 10.);

    // Run sequences and block program
    // $LED["WS2812"].spin(10000);

    Serial.println("setup() end");
}

void loop() {
    // If you comment out $LED["WS2812"]->spin(), sequence is run in the loop()
    Tasks.update();
}
