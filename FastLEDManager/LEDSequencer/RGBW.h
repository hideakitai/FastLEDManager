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
        Tween::Timeline timeline;
        CRGB clr {CRGB::Black};
        uint32_t duration_ms {0};
        bool b_repeat {false};

    public:
        virtual ~RGBW() {}

        RGBW(const String& name)
        : Sequencer(name) {}

        RGBW* duration(const uint32_t ms) {
            duration_ms = ms;
            return this;
        }

        RGBW* repeat(bool b) {
            b_repeat = b;
            return this;
        }

        virtual void enter() override {
            if (b_repeat) {
                timeline.mode(Tween::Mode::REPEAT_SQ);
            } else {
                timeline.mode(Tween::Mode::ONCE);
            }

            timeline.add(clr)
                .then(CRGB::Red)
                .hold(duration_ms)
                .then(CRGB::Green)
                .hold(duration_ms)
                .then(CRGB::Blue)
                .hold(duration_ms)
                .then(CRGB::White)
                .hold(duration_ms)
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
        Tween::Timeline timeline;
        CRGB clr {CRGB::Black};
        uint32_t duration_ms {0};
        bool b_repeat {false};

    public:
        virtual ~RRGGBBWW() {}

        RRGGBBWW(const String& name)
        : Sequencer(name) {}

        RRGGBBWW* duration(const uint32_t ms) {
            duration_ms = ms;
            return this;
        }

        RRGGBBWW* repeat(bool b) {
            b_repeat = b;
            return this;
        }

        virtual void enter() override {
            if (b_repeat) {
                timeline.mode(Tween::Mode::REPEAT_SQ);
            } else {
                timeline.mode(Tween::Mode::ONCE);
            }

            timeline.add(clr)
                .then(CRGB(128, 0, 0))
                .hold(duration_ms)
                .then(CRGB(255, 0, 0))
                .hold(duration_ms)
                .then(CRGB(0, 128, 0))
                .hold(duration_ms)
                .then(CRGB(0, 255, 0))
                .hold(duration_ms)
                .then(CRGB(0, 0, 128))
                .hold(duration_ms)
                .then(CRGB(0, 0, 255))
                .hold(duration_ms)
                .then(CRGB(128, 128, 128))
                .hold(duration_ms)
                .then(CRGB(255, 255, 255))
                .hold(duration_ms)
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
