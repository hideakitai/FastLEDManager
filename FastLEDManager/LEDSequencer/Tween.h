#pragma once
#ifndef ARDUINO_LEDMANAGER_LEDSEQUENCER_TWEEN_H
#define ARDUINO_LEDMANAGER_LEDSEQUENCER_TWEEN_H

#include <TaskManager.h>
#include <FastLED.h>
#include <Tween.h>

#include "Base.h"

namespace led {
namespace sequencer {

    class TweenSolid : public Sequencer {
        Tween::Timeline timeline;
        CRGB clr {CRGB::Black};

    public:
        virtual ~TweenSolid() {}

        TweenSolid(const String& name) : Sequencer(name) {}

        TweenSolid* tween(const std::function<void(Tween::Sequence<CRGB>&)> setup) {
            auto& seq = timeline.add(clr);
            setup(seq);
            return this;
        }

        TweenSolid* auto_erase(const bool b) {
            timeline.auto_erase(b);
            return this;
        }

        TweenSolid* mode(const Tween::Mode m) {
            timeline.mode(m);
            return this;
        }

        virtual void enter() override {
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

    class TweenEach : public Sequencer {
        Vec<Tween::Timeline> timelines;

    public:
        virtual ~TweenEach() {}

        TweenEach(const String& name) : Sequencer(name) {}

        TweenEach* tween(const std::function<void(Tween::Sequence<CRGB>&, const size_t idx)> setup) {
            if (timelines.empty()) {
                for (size_t i = 0; i < this->size(); ++i) {
                    timelines.emplace_back(Tween::Timeline());
                }
            }
            ASSERTM(this->size() == timelines.size(), "No matching size");
            for (size_t i = 0; i < timelines.size(); ++i) {
                auto& seq = timelines[i].add((*leds)[i]);
                setup(seq, i);
            }
            return this;
        }

        TweenEach* auto_erase(const bool b) {
            for (auto& tl : timelines) tl.auto_erase(b);
            return this;
        }

        TweenEach* mode(const Tween::Mode m) {
            for (auto& tl : timelines) tl.mode(m);
            return this;
        }

        virtual void enter() override {
            for (auto& tl : timelines) tl.start();
        }

        virtual void update() override {
            for (auto& tl : timelines) tl.update();
        }

        virtual void exit() override {
            for (auto& tl : timelines) tl.clear();
            timelines.clear();
            leds->fill_solid(CRGB::Black);
        }
    };

}  // namespace sequencer
}  // namespace led

#endif  // ARDUINO_LEDMANAGER_LEDSEQUENCER_TWEEN_H
