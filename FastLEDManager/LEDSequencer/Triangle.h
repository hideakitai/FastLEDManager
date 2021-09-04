#pragma once
#ifndef ARDUINO_LEDMANAGER_LEDSEQUENCER_TRIANGLE_H
#define ARDUINO_LEDMANAGER_LEDSEQUENCER_TRIANGLE_H

#include <TaskManager.h>
#include <FastLED.h>
#include <Tween.h>

#include "Base.h"

namespace led {
namespace sequencer {

    class Triangle : public Sequencer {
        CRGB target_clr {CRGB::Black};
        uint32_t duration_ms {0};
        uint32_t start_ms {0};
        bool b_up {true};

    public:
        virtual ~Triangle() {}

        Triangle(const String& name)
        : Sequencer(name) {}

        Triangle* color(const CRGB& clr) {
            target_clr = clr;
            return this;
        }

        Triangle* cycle_ms(const uint32_t ms) {
            duration_ms = ms / 2;
            return this;
        }

        Triangle* half_cycle_ms(const uint32_t ms) {
            duration_ms = ms;
            return this;
        }

        virtual void enter() override {
            start_ms = millis();
        }

        virtual void update() override {
            uint32_t elapsed = millis() - start_ms;
            uint32_t count = elapsed / duration_ms;
            bool b_up = !bool(count % 2);
            const float rate = (float)(elapsed % duration_ms) / (float)duration_ms;
            CRGB curr_clr;
            if (b_up) {
                curr_clr = target_clr.scale8((uint8_t)constrain(rate * 255, 0, 255));
            } else {
                curr_clr = target_clr.scale8((uint8_t)constrain((1.f - rate) * 255, 0, 255));
            }
            leds->fill_solid(curr_clr);
        }

        virtual void exit() override {
            leds->fill_solid(CRGB::Black);
        }
    };

}  // namespace sequencer
}  // namespace led

#endif  // ARDUINO_LEDMANAGER_LEDSEQUENCER_TRIANGLE_H
