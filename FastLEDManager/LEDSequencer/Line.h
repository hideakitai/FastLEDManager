#pragma once
#ifndef ARDUINO_LEDMANAGER_LEDSEQUENCER_LINE_H
#define ARDUINO_LEDMANAGER_LEDSEQUENCER_LINE_H

#include <TaskManager.h>
#include <FastLED.h>
#include <Tween.h>

#include "Base.h"

namespace led {
namespace sequencer {

    class Line : public Sequencer {
        struct Config {
            CRGB clr_from {CRGB::Black};
            CRGB clr_to {CRGB::Black};
            uint32_t pixel_delay_ms {0};
            uint32_t total_delay_ms {0};
            uint32_t duration_ms {0};
            bool b_reverse {false};
            bool b_repeat {false};
        } config;

        Tween::Timeline timeline;

    public:
        virtual ~Line() {}

        Line(const String& name) : Sequencer(name) {}

        Line* color(const CRGB& clr) {
            config.clr_from = clr;
            config.clr_to = CRGB::Black;
            return this;
        }

        Line* pixel_delay(const uint32_t delay_ms) {
            config.pixel_delay_ms = delay_ms;
            config.total_delay_ms = delay_ms * leds->size();
            return this;
        }

        Line* fadeout_ms(const uint32_t ms) {
            config.duration_ms = ms;
            return this;
        }

        Line* reverse(bool b) {
            config.b_reverse = b;
            return this;
        }

        Line* repeat(bool b) {
            config.b_repeat = b;
            return this;
        }

        Line* configs(const Config& cfg) {
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
            add_to_timeline();
            timeline.start();
        }

        virtual void update() override {
            timeline.update();
        }

        virtual void exit() override {
            timeline.clear();
            leds->fill_solid(CRGB::Black);
        }

    private:
        void add_to_timeline() {
            for (size_t i = 0; i < this->size(); ++i) {
                float delay_ms_before;
                if (config.b_reverse) {
                    delay_ms_before = config.pixel_delay_ms * (leds->size() - 1 - i);
                } else {
                    delay_ms_before = config.pixel_delay_ms * i;
                }
                float delay_ms_after = config.total_delay_ms - config.duration_ms;

                timeline.add((*leds)[i])
                    .offset(delay_ms_before)
                    .init(CRGB::Black)
                    .then(config.clr_from)
                    .then(config.clr_to, config.duration_ms)
                    .hold(delay_ms_after);
            }
        }
    };

}  // namespace sequencer
}  // namespace led

#endif  // ARDUINO_LEDMANAGER_LEDSEQUENCER_LINE_H
