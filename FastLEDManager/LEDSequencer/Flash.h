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
        CRGB target_clr {CRGB::Black};
        uint32_t duration_ms {0};
        uint32_t start_ms {0};

    public:
        virtual ~Flash() {}

        Flash(const String& name)
        : Sequencer(name) {}

        Flash* color(const CRGB& clr) {
            target_clr = clr;
            return this;
        }

        // 1 cycle = high -> low -> (here) -> high ...
        Flash* cycle_ms(const uint32_t ms) {
            duration_ms = ms / 2;
            return this;
        }

        Flash* half_cycle_ms(const uint32_t ms) {
            duration_ms = ms;
            return this;
        }

        virtual void enter() override {
            start_ms = millis();
        }

        virtual void update() override {
            uint32_t elapsed = millis() - start_ms;
            uint32_t count = elapsed / duration_ms;
            bool b_high = !bool(count % 2);
            CRGB curr_clr = b_high ? target_clr : CRGB::Black;
            leds->fill_solid(curr_clr);
        }

        virtual void exit() override {
            leds->fill_solid(CRGB::Black);
        }
    };

}  // namespace sequencer
}  // namespace led

#endif  //ARDUINO_LEDMANAGER_LEDSEQUENCER_FLASH_H
