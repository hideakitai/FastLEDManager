#pragma once
#ifndef ARDUINO_LEDMANAGER_LEDSEQUENCER_SEQUENCE_H
#define ARDUINO_LEDMANAGER_LEDSEQUENCER_SEQUENCE_H

#include <TaskManager.h>
#include <FastLED.h>
#include <Tween.h>

#include "Base.h"

namespace led {
namespace sequencer {

    class Sequence : public Sequencer {
    public:
        virtual ~Sequence() {}

        Sequence(const String& name) : Sequencer(name) {}

        virtual void enter() override {
            leds->fill_solid(CRGB::Black);
        }

        virtual void update() override {
            switch (this->getSubTaskMode()) {
                case SubTaskMode::SYNC: {
                    leds->fill_solid(CRGB::Black);
                    for (size_t i = 0; i < numSubTasks(); ++i) {
                        auto st = getSubTaskByIndex<Sequencer>(i);
                        *leds += st->colors();
                    }
                    break;
                }
                case SubTaskMode::SEQUENCE: {
                    for (size_t i = 0; i < numSubTasks(); ++i) {
                        auto st = getSubTaskByIndex<Sequencer>(i);
                        if (st->isRunning()) {
                            if (st->leds_valid()) {
                                *leds = st->colors();
                            }
                        }
                    }
                    break;
                }
                default: {
                    break;
                }
            }
        }

        virtual void exit() override {
            leds->fill_solid(CRGB::Black);
        }

        template <typename TaskType>
        Sequence* sync(const std::function<void(TaskRef<TaskType>)>& setup) {
            return sync<TaskType>("", setup);
        }
        template <typename TaskType>
        Sequence* sync(const String& name, const std::function<void(TaskRef<TaskType>)>& setup) {
            this->Sequencer::sync<TaskType>(name, [&](TaskRef<TaskType> t) {
                t->allocate(leds->size());
                setup(t);
            });
            return this;
        }

        template <typename TaskType>
        Sequence* then(const std::function<void(TaskRef<TaskType>)>& setup) {
            return then<TaskType>("", 0, setup);
        }
        template <typename TaskType>
        Sequence* then(const double sec, const std::function<void(TaskRef<TaskType>)>& setup) {
            return then<TaskType>("", sec, setup);
        }
        template <typename TaskType>
        Sequence* then(const String& name, const double sec, const std::function<void(TaskRef<TaskType>)>& setup) {
            this->Sequencer::then<TaskType>(name, sec, [&](TaskRef<TaskType> t) {
                t->attach(*leds);
                setup(t);
            });
            return this;
        }

        Sequence* hold(const double sec) {
            this->Sequencer::then<Delay>("", sec, [&](TaskRef<Delay> t) { t->delay(sec * 1000.); });
            return this;
        }
    };

}  // namespace sequencer
}  // namespace led

#endif  // ARDUINO_LEDMANAGER_LEDSEQUENCER_SEQUENCE_H
