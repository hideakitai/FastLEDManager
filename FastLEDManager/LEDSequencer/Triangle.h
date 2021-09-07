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
        struct Config {
            CRGB target_clr {CRGB::Black};
            uint32_t duration_ms {0};
            uint32_t start_ms {0};
        } config;

    public:
        virtual ~Triangle() {}

        Triangle(const String& name)
        : Sequencer(name) {}

        Triangle* color(const CRGB& clr) {
            config.target_clr = clr;
            return this;
        }

        Triangle* cycle_ms(const uint32_t ms) {
            config.duration_ms = ms / 2;
            return this;
        }

        Triangle* half_cycle_ms(const uint32_t ms) {
            config.duration_ms = ms;
            return this;
        }

        Triangle* configs(const Config& cfg) {
            config = cfg;
            return this;
        }

        const Config& configs() const {
            return config;
        }

        virtual void enter() override {
            config.start_ms = millis();
        }

        virtual void update() override {
            uint32_t elapsed = millis() - config.start_ms;
            uint32_t count = elapsed / config.duration_ms;
            bool b_up = !bool(count % 2);
            const float rate = (float)(elapsed % config.duration_ms) / (float)config.duration_ms;
            CRGB curr_clr;
            if (b_up) {
                curr_clr = config.target_clr.scale8((uint8_t)constrain(rate * 255, 0, 255));
            } else {
                curr_clr = config.target_clr.scale8((uint8_t)constrain((1.f - rate) * 255, 0, 255));
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
