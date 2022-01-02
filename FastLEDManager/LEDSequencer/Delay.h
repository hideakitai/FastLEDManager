#pragma once
#ifndef ARDUINO_LEDMANAGER_LEDSEQUENCER_DELAY_H
#define ARDUINO_LEDMANAGER_LEDSEQUENCER_DELAY_H

#include <TaskManager.h>
#include <FastLED.h>
#include <Tween.h>

#include "Base.h"

namespace led {
namespace sequencer {

    class Spin : public Sequencer {
        struct Config {
            uint32_t delay_ms {0};
        } config;

    public:
        virtual ~Spin() {}

        Spin(const String& name) : Sequencer(name) {}

        Spin* spin(const uint32_t ms) {
            config.delay_ms = ms;
            setDurationMsec(config.delay_ms);
            return this;
        }

        Spin* configs(const Config& cfg) {
            config = cfg;
            return this;
        }

        const Config& configs() const {
            return config;
        }

        virtual void update() override {}
    };

}  // namespace sequencer
}  // namespace led

#endif  // ARDUINO_LEDMANAGER_LEDSEQUENCER_DELAY_H
