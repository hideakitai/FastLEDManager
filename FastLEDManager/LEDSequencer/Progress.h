#pragma once
#ifndef ARDUINO_LEDMANAGER_LEDSEQUENCER_PROGRESS_H
#define ARDUINO_LEDMANAGER_LEDSEQUENCER_PROGRESS_H

#include <TaskManager.h>
#include <FastLED.h>
#include <Tween.h>

#include "Base.h"

namespace led {
namespace sequencer {

    class Progress : public Sequencer {
        struct Config {
            CRGB clr {CRGB::Black};
            float rate {0.};
        } config;

    public:
        virtual ~Progress() {}

        Progress(const String& name)
        : Sequencer(name) {}

        Progress* color(const CRGB& c) {
            config.clr = c;
            return this;
        }

        Progress* ratio(const float r) {
            config.rate = r;
            return this;
        }

        Progress* percent(const float p) {
            config.rate = p / 100.f;
            return this;
        }

        Progress* configs(const Config& cfg) {
            config = cfg;
            return this;
        }

        const Config& configs() const {
            return config;
        }

        virtual void update() override {
            size_t sz = floor((float)leds->size() * config.rate);
            if (config.rate < 0.f) sz = 0;
            if (config.rate >= 1.f) sz = leds->size();
            ::fill_solid(*leds, sz, config.clr);
        }

        virtual void exit() override {
            leds->fill_solid(CRGB::Black);
        }
    };

}  // namespace sequencer
}  // namespace led

#endif  // ARDUINO_LEDMANAGER_LEDSEQUENCER_PROGRESS_H
