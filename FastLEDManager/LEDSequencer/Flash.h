#pragma once
#ifndef ARDUINO_LEDMANAGER_LEDSEQUENCER_FLASH_H
#define ARDUINO_LEDMANAGER_LEDSEQUENCER_FLASH_H

#include <TaskManager.h>
#include <FastLED.h>
#include <Tween.h>

#include "Base.h"

namespace led {
namespace sequencer {

    class Flash : public Sequencer {
        struct Config {
            CRGB target_clr {CRGB::Black};
            uint32_t duration_ms {0};
            uint32_t start_ms {0};
        } config;

    public:
        virtual ~Flash() {}

        Flash(const String& name) : Sequencer(name) {}

        Flash* color(const CRGB& clr) {
            config.target_clr = clr;
            return this;
        }

        // 1 cycle = high -> low -> (here) -> high ...
        Flash* cycle_ms(const uint32_t ms) {
            config.duration_ms = ms / 2;
            return this;
        }

        Flash* half_cycle_ms(const uint32_t ms) {
            config.duration_ms = ms;
            return this;
        }

        Flash* configs(const Config& cfg) {
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
            bool b_high = !bool(count % 2);
            CRGB curr_clr = b_high ? config.target_clr : CRGB::Black;
            leds->fill_solid(curr_clr);
        }

        virtual void exit() override {
            leds->fill_solid(CRGB::Black);
        }
    };

}  // namespace sequencer
}  // namespace led

#endif  // ARDUINO_LEDMANAGER_LEDSEQUENCER_FLASH_H
