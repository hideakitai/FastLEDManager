#pragma once
#ifndef ARDUINO_LEDMANAGER_LEDSEQUENCER_RANDOM_H
#define ARDUINO_LEDMANAGER_LEDSEQUENCER_RANDOM_H

#include <TaskManager.h>
#include <FastLED.h>
#include <Tween.h>

#include "Base.h"

namespace led {
namespace sequencer {

    class Random : public Sequencer {
        CRGB clr;
        size_t index {0};
        size_t sz {0};
        size_t n_at_once {1};
        size_t n_per_frame {1};
        uint8_t fade_value {10};

    public:
        virtual ~Random() {}

        Random(const String& name)
        : Sequencer(name) {}

        Random* range(const size_t idx, const size_t sz) {
            index = idx;
            this->sz = sz;
            return this;
        }

        Random* color(const CRGB& c) {
            clr = c;
            return this;
        }

        Random* num_at_once(const size_t n) {
            n_at_once = n;
            return this;
        }

        Random* every_n_frame(const size_t n) {
            n_per_frame = n;
            return this;
        }

        Random* fade_by(const uint8_t v) {
            fade_value = v;
            return this;
        }

        virtual void enter() override {
            if ((index == 0) && (sz == 0)) sz = this->leds->size();
        }

        virtual void update() override {
            static size_t cnt = 0;
            if (cnt++ % n_per_frame == 0) {
                for (size_t i = 0; i < n_at_once; ++i) {
                    randomSeed(micros());
                    size_t idx = random(index, index + sz);
                    (*leds)[idx] = clr;
                }
            }
            if (fade_value != 0)
                leds->fadeToBlackBy(fade_value);
        }

        virtual void exit() override {
            leds->fill_solid(CRGB::Black);
        }
    };

}  // namespace sequencer
}  // namespace led

#endif  // ARDUINO_LEDMANAGER_LEDSEQUENCER_RANDOM_H
