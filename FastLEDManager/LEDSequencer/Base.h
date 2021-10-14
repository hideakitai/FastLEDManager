#pragma once
#ifndef ARDUINO_LEDMANAGER_LEDSEQUENCER_BASE_H
#define ARDUINO_LEDMANAGER_LEDSEQUENCER_BASE_H

#include <TaskManager.h>
#include <FastLED.h>
#include <Tween.h>

namespace led {
namespace sequencer {

    class Sequencer : public Task::Base {
    protected:
        CPixelView<CRGB>* leds {nullptr};
        CRGB* raw_leds {nullptr};
        bool b_allocated {false};

    public:
        virtual ~Sequencer() {
            if (b_allocated) {
                if (leds) delete leds;
                if (raw_leds) delete[] raw_leds;
            }
            leds = nullptr;
            raw_leds = nullptr;
        }

        Sequencer(const String& name) : Task::Base(name) {}

        void attach(CPixelView<CRGB>& led_arr) {
            raw_leds = nullptr;
            leds = &led_arr;
            b_allocated = false;
        }

        void allocate(const size_t sz) {
            raw_leds = new CRGB[sz];
            leds = new CPixelView<CRGB>(raw_leds, sz);
            b_allocated = true;
            leds->fill_solid(CRGB::Black);
        }

        CPixelView<CRGB>& colors() {
            return *leds;
        }
        const CPixelView<CRGB>& colors() const {
            return *leds;
        }
        bool attached() const {
            return (leds != nullptr) && !b_allocated;
        }
        bool allocated() const {
            return (leds != nullptr) && b_allocated;
        }
        bool leds_valid() const {
            return attached() || allocated();
        }
        size_t size() const {
            return (size_t)leds->size();
        }

        virtual void begin() override {}
        virtual void enter() override {}
        virtual void update() override = 0;
        virtual void exit() override {}
        virtual void idle() override {}
        virtual void reset() override {}
    };

}  // namespace sequencer
}  // namespace led

#endif  // ARDUINO_LEDMANAGER_LEDSEQUENCER_BASE_H
