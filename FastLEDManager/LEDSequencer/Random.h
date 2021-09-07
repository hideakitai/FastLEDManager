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
        struct Config {
            size_t index {0};
            size_t sz {0};
            CRGB clr {CRGB::Black};
            size_t n_at_once {1};
            size_t n_per_frame {1};
            uint8_t fade_value {10};
        } config;

    public:
        virtual ~Random() {}

        Random(const String& name)
        : Sequencer(name) {}

        Random* range(const size_t idx, const size_t sz) {
            config.index = idx;
            config.sz = sz;
            return this;
        }

        Random* color(const CRGB& c) {
            config.clr = c;
            return this;
        }

        Random* num_at_once(const size_t n) {
            config.n_at_once = n;
            return this;
        }

        Random* every_n_frame(const size_t n) {
            config.n_per_frame = n;
            return this;
        }

        Random* fade_by(const uint8_t v) {
            config.fade_value = v;
            return this;
        }

        Random* configs(const Config& cfg) {
            config = cfg;
            return this;
        }

        const Config& configs() const {
            return config;
        }

        virtual void enter() override {
            if ((config.index == 0) && (config.sz == 0)) {
                config.sz = this->leds->size();
            }
        }

        virtual void update() override {
            static size_t cnt = 0;
            if (cnt++ % config.n_per_frame == 0) {
                for (size_t i = 0; i < config.n_at_once; ++i) {
                    randomSeed(micros());
                    size_t idx = random(config.index, config.index + config.sz);
                    (*leds)[idx] = config.clr;
                }
            }
            if (config.fade_value != 0)
                leds->fadeToBlackBy(config.fade_value);
        }

        virtual void exit() override {
            leds->fill_solid(CRGB::Black);
        }
    };

}  // namespace sequencer
}  // namespace led

#endif  // ARDUINO_LEDMANAGER_LEDSEQUENCER_RANDOM_H
