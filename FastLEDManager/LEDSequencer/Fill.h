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
        struct Config {
            CRGB clr {CRGB::Black};
            size_t idx {0};
            size_t sz {0};
            uint8_t fade {0};
        } config;

    public:
        virtual ~Fill() {}

        Fill(const String& name) : Sequencer(name) {}

        Fill* color(const CRGB& c) {
            config.clr = c;
            config.idx = 0;
            config.sz = this->leds->size();
            return this;
        }
        Fill* color(const CRGB& c, const size_t sz) {
            config.clr = c;
            config.idx = 0;
            config.sz = sz;
            return this;
        }
        Fill* color(const CRGB& c, const size_t idx, const size_t sz) {
            config.clr = c;
            config.idx = idx;
            config.sz = sz;
            return this;
        }

        Fill* fade_by(const uint8_t v) {
            config.fade = v;
            return this;
        }

        Fill* configs(const Config& cfg) {
            config = cfg;
            return this;
        }

        const Config& configs() const {
            return config;
        }

        virtual void enter() override {
            fill_solid(&(*(this->leds)[config.idx]), config.sz, config.clr);
        }

        virtual void update() override {
            fill_solid(&(*(this->leds)[config.idx]), config.sz, config.clr);
            if (config.fade) {
                config.clr.fadeToBlackBy(config.fade);
            }
        }
    };

}  // namespace sequencer
}  // namespace led

#endif  // ARDUINO_LEDMANAGER_LEDSEQUENCER_FILL_H
