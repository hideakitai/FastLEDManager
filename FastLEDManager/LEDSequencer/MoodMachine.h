#pragma once
#ifndef ARDUINO_LEDMANAGER_LEDSEQUENCER_MOOD_MACHINE_H
#define ARDUINO_LEDMANAGER_LEDSEQUENCER_MOOD_MACHINE_H

#include <TaskManager.h>
#include <FastLED.h>
#include <Tween.h>

#include "Base.h"

namespace led {
namespace sequencer {

    class MoodMachineTimeline {
        // singleton
        MoodMachineTimeline() {}
        MoodMachineTimeline(const MoodMachineTimeline&) = delete;
        MoodMachineTimeline& operator=(const MoodMachineTimeline&) = delete;

        size_t ref_count {0};

        Tween::Timeline timeline[3];
        float clr[3];  // should be float not CRGB
        float duration_ms[3] {1000, 900, 1300};

    public:
        static MoodMachineTimeline& get() {
            static MoodMachineTimeline m;
            return m;
        }

        void add_count() {
            ++ref_count;
        }
        void sub_count() {
            --ref_count;
        }
        size_t use_count() const {
            return ref_count;
        }
        CRGB color() const {
            return CRGB(clr[0], clr[1], clr[2]);
        }

        void enter() {
            for (size_t i = 0; i < 3; ++i) {
                clr[i] = 0;
                restart_timeline(i);
            }
        }

        void update() {
            for (size_t i = 0; i < 3; ++i) {
                if (!timeline[i].update()) {
                    restart_timeline(i);
                }
            }
        }

        void exit() {
            for (size_t i = 0; i < 3; ++i) timeline[i].clear();
        }

    private:
        void restart_timeline(const uint8_t i) {
            timeline[i].add(clr[i], true).then(random(0, 255), duration_ms[i]);
            timeline[i].mode(Tween::Mode::ONCE);
            timeline[i].start();
        }
    };

    class MoodMachine : public Sequencer {
    public:
        MoodMachine(const String& name) : Sequencer(name) {
            MoodMachineTimeline::get().add_count();
        }

        virtual ~MoodMachine() {
            MoodMachineTimeline::get().sub_count();
        }

        virtual void enter() override {
            if (MoodMachineTimeline::get().use_count() == 1) {
                MoodMachineTimeline::get().enter();
            }
        }

        virtual void update() override {
            MoodMachineTimeline::get().update();
            leds->fill_solid(MoodMachineTimeline::get().color());
        }

        virtual void exit() override {
            if (MoodMachineTimeline::get().use_count() == 1) {
                MoodMachineTimeline::get().exit();
                leds->fill_solid(CRGB::Black);
            }
        }
    };

}  // namespace sequencer
}  // namespace led

#endif  // ARDUINO_LEDMANAGER_LEDSEQUENCER_MOOD_MACHINE_H
