#pragma once
#ifndef ARDUINO_LEDMANAGER_LEDSEQUENCER_RGBW_H
#define ARDUINO_LEDMANAGER_LEDSEQUENCER_RGBW_H

#include <TaskManager.h>
#include <FastLED.h>
#include <Tween.h>

#include "Base.h"

namespace led {
namespace sequencer {

    class RGBW : public Sequencer {
        struct Config {
            uint32_t duration_ms {0};
            bool b_repeat {false};
        } config;

        Tween::Timeline timeline;
        CRGB clr {CRGB::Black};

    public:
        virtual ~RGBW() {}

        RGBW(const String& name)
        : Sequencer(name) {}

        RGBW* duration(const uint32_t ms) {
            config.duration_ms = ms;
            return this;
        }

        RGBW* repeat(bool b) {
            config.b_repeat = b;
            return this;
        }

        RGBW* configs(const Config& cfg) {
            config = cfg;
            return this;
        }

        const Config& configs() const {
            return config;
        }

        virtual void enter() override {
            if (config.b_repeat) {
                timeline.mode(Tween::Mode::REPEAT_SQ);
            } else {
                timeline.mode(Tween::Mode::ONCE);
            }

            timeline.add(clr)
                .then(CRGB::Red)
                .hold(config.duration_ms)
                .then(CRGB::Green)
                .hold(config.duration_ms)
                .then(CRGB::Blue)
                .hold(config.duration_ms)
                .then(CRGB::White)
                .hold(config.duration_ms)
                .then(CRGB::Black);

            timeline.start();
        }

        virtual void update() override {
            timeline.update();
            leds->fill_solid(clr);
        }

        virtual void exit() override {
            timeline.clear();
            leds->fill_solid(CRGB::Black);
        }
    };

    class RRGGBBWW : public Sequencer {
        struct Config {
            uint32_t duration_ms {0};
            bool b_repeat {false};
        } config;

        Tween::Timeline timeline;
        CRGB clr {CRGB::Black};

    public:
        virtual ~RRGGBBWW() {}

        RRGGBBWW(const String& name)
        : Sequencer(name) {}

        RRGGBBWW* duration(const uint32_t ms) {
            config.duration_ms = ms;
            return this;
        }

        RRGGBBWW* repeat(bool b) {
            config.b_repeat = b;
            return this;
        }

        RRGGBBWW* configs(const Config& cfg) {
            config = cfg;
            return this;
        }

        const Config& configs() const {
            return config;
        }

        virtual void enter() override {
            if (config.b_repeat) {
                timeline.mode(Tween::Mode::REPEAT_SQ);
            } else {
                timeline.mode(Tween::Mode::ONCE);
            }

            timeline.add(clr)
                .then(CRGB(128, 0, 0))
                .hold(config.duration_ms)
                .then(CRGB(255, 0, 0))
                .hold(config.duration_ms)
                .then(CRGB(0, 128, 0))
                .hold(config.duration_ms)
                .then(CRGB(0, 255, 0))
                .hold(config.duration_ms)
                .then(CRGB(0, 0, 128))
                .hold(config.duration_ms)
                .then(CRGB(0, 0, 255))
                .hold(config.duration_ms)
                .then(CRGB(128, 128, 128))
                .hold(config.duration_ms)
                .then(CRGB(255, 255, 255))
                .hold(config.duration_ms)
                .then(CRGB::Black);

            timeline.start();
        }

        virtual void update() override {
            timeline.update();
            leds->fill_solid(clr);
        }

        virtual void exit() override {
            timeline.clear();
            leds->fill_solid(CRGB::Black);
        }
    };

}  // namespace sequencer
}  // namespace led

#endif  // ARDUINO_LEDMANAGER_LEDSEQUENCER_RGBW_H
