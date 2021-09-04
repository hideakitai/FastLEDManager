#pragma once
#ifndef ARDUINO_LEDMANAGER_LEDSEQUENCER_DELAY_H
#define ARDUINO_LEDMANAGER_LEDSEQUENCER_DELAY_H

#include <TaskManager.h>
#include <FastLED.h>
#include <Tween.h>

#include "Base.h"

namespace led {
namespace sequencer {

    class Delay : public Sequencer {
    public:
        virtual ~Delay() {}

        Delay(const String& name)
        : Sequencer(name) {}

        void delay(const uint32_t ms) {
            setDurationMsec(ms);
        }

        virtual void update() override {}
    };

}  // namespace sequencer
}  // namespace led

#endif  // ARDUINO_LEDMANAGER_LEDSEQUENCER_DELAY_H
