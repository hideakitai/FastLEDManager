#pragma once
#ifndef ARDUINO_LEDMANAGER_LEDSEQUENCER_MOOD_MACHINE_H
#define ARDUINO_LEDMANAGER_LEDSEQUENCER_MOOD_MACHINE_H

#include <TaskManager.h>
#include <FastLED.h>
#include <Tween.h>

#include "Base.h"

namespace led {
namespace sequencer {

    class MoodMachine : public Sequencer {
        Tween::Timeline timeline[3];
        float clr[3];  // should be float not CRGB
        float duration_ms[3] {1000, 900, 1300};

    public:
        virtual ~MoodMachine() {}

        MoodMachine(const String& name)
        : Sequencer(name) {}

        virtual void enter() override {
            for (size_t i = 0; i < 3; ++i) {
                clr[i] = 0;
                restart_timeline(i);
            }
        }

        virtual void update() override {
            for (size_t i = 0; i < 3; ++i) {
                if (!timeline[i].update()) {
                    restart_timeline(i);
                }
            }
            leds->fill_solid(CRGB(clr[0], clr[1], clr[2]));
        }

        virtual void exit() override {
            for (size_t i = 0; i < 3; ++i)
                timeline[i].clear();
            leds->fill_solid(CRGB::Black);
        }

    private:
        void restart_timeline(const uint8_t i) {
            timeline[i].add(clr[i], true).then(random(0, 255), duration_ms[i]);
            timeline[i].mode(Tween::Mode::ONCE);
            timeline[i].start();
        }
    };

}  // namespace sequencer
}  // namespace led

#endif  // ARDUINO_LEDMANAGER_LEDSEQUENCER_MOOD_MACHINE_H
