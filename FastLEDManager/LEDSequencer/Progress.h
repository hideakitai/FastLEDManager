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
        CRGB clr {CRGB::Black};
        float rate {0.};

    public:
        virtual ~Progress() {}

        Progress(const String& name)
        : Sequencer(name) {}

        Progress* color(const CRGB& c) {
            clr = c;
            return this;
        }

        Progress* ratio(const float r) {
            this->rate = r;
            return this;
        }

        Progress* percent(const float p) {
            this->rate = p / 100.f;
            return this;
        }

        virtual void update() override {
            size_t sz = floor((float)leds->size() * rate);
            if (rate < 0.f) sz = 0;
            if (rate >= 1.f) sz = leds->size();
            ::fill_solid(*leds, sz, clr);
        }

        virtual void exit() override {
            leds->fill_solid(CRGB::Black);
        }
    };

}  // namespace sequencer
}  // namespace led

#endif  // ARDUINO_LEDMANAGER_LEDSEQUENCER_PROGRESS_H
