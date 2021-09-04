#pragma once
#ifndef ARDUINO_LEDMANAGER_LEDSEQUENCER_FILL_H
#define ARDUINO_LEDMANAGER_LEDSEQUENCER_FILL_H

#include <TaskManager.h>
#include <FastLED.h>
#include <Tween.h>

#include "Base.h"

namespace led {
namespace sequencer {

    class Fill : public Sequencer {
        CRGB clr {CRGB::Black};
        size_t idx {0};
        size_t sz {0};
        uint8_t fade {0};

    public:
        virtual ~Fill() {}

        Fill(const String& name)
        : Sequencer(name) {}

        Fill* color(const CRGB& c) {
            this->clr = c;
            this->idx = 0;
            this->sz = this->leds->size();
            return this;
        }
        Fill* color(const CRGB& c, const size_t sz) {
            this->clr = c;
            this->idx = 0;
            this->sz = sz;
            return this;
        }
        Fill* color(const CRGB& c, const size_t idx, const size_t sz) {
            this->clr = c;
            this->idx = idx;
            this->sz = sz;
            return this;
        }

        Fill* fade_by(const uint8_t v) {
            this->fade = v;
            return this;
        }

        virtual void enter() override {
            fill_solid(&(*(this->leds)[idx]), sz, clr);
        }

        virtual void update() override {
            fill_solid(&(*(this->leds)[idx]), sz, clr);
            if (fade) {
                clr.fadeToBlackBy(fade);
            }
        }
    };

}  // namespace sequencer
}  // namespace led

#endif  // ARDUINO_LEDMANAGER_LEDSEQUENCER_FILL_H
