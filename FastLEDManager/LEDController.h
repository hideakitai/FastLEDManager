#pragma once
#ifndef ARDUINO_LEDMANAGER_CONTROLLER_H
#define ARDUINO_LEDMANAGER_CONTROLLER_H

#include <FastLED.h>
#include <TaskManager.h>
#include <GammaTable.h>

#include "LEDSequencer.h"

namespace led {

class Controller : public Task::Base {
protected:
    CLEDController* fastled {nullptr};
    CPixelView<CRGB>& leds;
    GammaTable<uint8_t, 256> gamma_tbl {1.f};
    bool b_assigned {false};
    bool b_layered {true};
    uint8_t fade_value {0};

    struct Config {
        uint8_t brightness {255};
        uint8_t dither {BINARY_DITHER};
        uint16_t refresh_rate {0};
        bool refresh_constrain {false};
        CRGB correction {CRGB::White};
        CRGB temperature {CRGB::White};
    } config;

#ifdef ARDUINO_ARCH_ESP32
    bool b_multi_core {true};
    uint8_t task_led_show_core {0};            // core 1: main app
    uint32_t task_led_show_stack_size {1024};  // bytes
    uint32_t task_led_show_priority {2};       // LOW 0 - 25 HIGH
    TaskHandle_t handle_task_led_show {NULL};

    // TODO: Notify back to main task from isr
    // Notify back needs static but different main task handle for each LED stripe
    // One solution is to pass different main task handle via pvParameters
    // Currently just notify give to isr task
    // https://github.com/hideakitai/sketches_arduino/blob/main/esp32_fastled_multicore_rmt/esp32_fastled_multicore_rmt.ino
    static void isr_task_led_show(void* pvParameters) {
        Controller* fastled = (Controller*)pvParameters;
        while (true) {
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);  // wait for the trigger
            fastled->show_impl();                     // do the show()
        }
    }

    void trigger_led_show() {
        xTaskNotifyGive(handle_task_led_show);  // trigger the next show() task
    }
#endif

public:
    virtual ~Controller() {}

    Controller(const String& name, CPixelView<CRGB>& leds)
    : Task::Base(name), leds(leds) {}

    // ---------- TaskManager overloads ----------

    virtual void enter() override {
        fastled->setDither(config.dither);  // explicitly enable dithering
        fastled->setCorrection(config.correction);
        fastled->setTemperature(config.temperature);

        leds.fill_solid(CRGB::Black);

#ifdef ARDUINO_ARCH_ESP32
        if (b_multi_core) {
            // create the isr task for fastled show()
            if (handle_task_led_show == NULL) {
                xTaskCreatePinnedToCore(
                    isr_task_led_show,
                    this->getName().c_str(),
                    task_led_show_stack_size,
                    this,  // pass Controller* as pvParameters
                    task_led_show_priority,
                    &handle_task_led_show,
                    task_led_show_core);
            }
        }
#endif

        // trigger the first show task (required for multi_core)
        show();
    }

    virtual void update() override {
        // accumulate all sequences if layered
        if (b_layered) {
            if (b_assigned) {
                b_assigned = false;
            } else {
                leds.fill_solid(CRGB::Black);
            }

            for (size_t i = 0; i < this->subtasks.size(); ++i) {
                auto st = this->getSubTaskByIndex<sequencer::Sequencer>(i);
                if (st->isRunning()) {
                    leds += st->colors();
                }
            }
        }

        // overwrite output
        if (this->existsSubTask("overwrite")) {
            auto ot = this->getSubTaskByName<sequencer::Sequencer>("overwrite");
            const auto& c = ot->colors();
            for (size_t i = 0; i < leds.size(); ++i) {
                if (c[i]) {
                    leds[i] = c[i];
                }
            }
        }

        // apply gamma
        if (gamma_tbl.gamma() != 1.f) {
            for (size_t p = 0; p < leds.size(); ++p) {
                for (size_t c = 0; c < 3; ++c) {
                    leds[p][c] = gamma_tbl[leds[p][c]];
                }
            }
        }
        show();
    }

    virtual void exit() override {
        leds.fill_solid(CRGB::Black);
        show();
    }

    // ---------- Configuration ----------

    Controller& brightness(const uint8_t bri) {
        config.brightness = bri;
        return *this;
    }

    Controller& dither(const uint8_t dither_mode) {
        config.dither = dither_mode;
        fastled->setDither(dither_mode);
        return *this;
    }

    Controller& correction(const CRGB& clr_correction) {
        config.correction = clr_correction;
        fastled->setCorrection(clr_correction);
        return *this;
    }

    Controller& temperature(const CRGB& clr_temperature) {
        config.temperature = clr_temperature;
        fastled->setTemperature(clr_temperature);
        return *this;
    }

    Controller& fadeout(const uint8_t v) {
        fade_value = v;
        return *this;
    }

    Controller& gamma(const float v) {
        gamma_tbl.gamma(v);
        return *this;
    }

    Controller& layer(const bool b) {
        b_layered = b;
        return *this;
    }

    // TODO: divide one led stripe to multiple parts
    // maybe create new Controllers inside of Controller
    Controller& divide() {
        return *this;
    }

#ifdef ARDUINO_ARCH_ESP32
    Controller& multi_core(const bool b) {
        b_multi_core = b;
        return *this;
    }
    Controller& multi_core_config(const uint8_t core, const uint32_t stack_size, const uint32_t priority) {
        task_led_show_core = core;              // core 1: main app
        task_led_show_stack_size = stack_size;  // bytes
        task_led_show_priority = priority;      // LOW 0 - 25 HIGH
        return *this;
    }
#endif

    // ---------- LED Control ----------

    template <typename PixelType = CRGB>
    Controller& assign(const PixelType* colors, const size_t size) {
        CPixelView<PixelType> c(colors, size);
        return assign(c);
    }

    template <typename PixelType = CRGB>
    Controller& assign(CPixelView<PixelType>& colors) {
        if (colors.size() > leds.size()) {
            LOG_ERROR("Assigned pixel size is out of range:", colors.size(), "should be <", leds.size());
        } else {
            leds = colors;
            b_assigned = true;
        }
        return *this;
    }

    Controller& show(const uint8_t brightness = 0) {
#ifdef ARDUINO_ARCH_ESP32
        if (b_multi_core && (handle_task_led_show != NULL)) {
            trigger_led_show();
        } else {
            show_impl(brightness);
        }
#else
        show_impl(brightness);
#endif
        return *this;
    }

private:
    void show_impl(const uint8_t brightness = 0) {
        if (brightness == 0)
            fastled->showLeds(config.brightness);
        else
            fastled->showLeds(brightness);

        if (fade_value != 0)
            leds.fadeToBlackBy(fade_value);
    }

public:
    /// Run all sequence and block program
    Controller& delay(const uint32_t ms) {
        const uint64_t end_us = micros() + uint64_t(ms) * 1000;
        while (micros() < end_us) {
            Tasks.update(this->getName());
        }
        return *this;
    }

    /// show current color once and hold it until timeout
    Controller& hold(const uint32_t ms) {
        show();
        const uint64_t end_us = micros() + uint64_t(ms) * 1000;
        while (micros() < end_us)
            ;
        return *this;
    }

    // ---------- LED Control (Immediate) ----------

    Controller& fill_solid(const CRGB& c) {
        return this->fill_solid(c, 0, leds.size());
    }
    Controller& fill_solid(const CRGB& c, const size_t sz) {
        return this->fill_solid(c, 0, sz);
    }
    Controller& fill_solid(const CRGB& c, const size_t idx, const size_t sz) {
        ::fill_solid(&(leds[idx]), sz, c);
        b_assigned = true;
        return *this;
    }

    // ---------- LED Sequencers (Non Blocking) ----------

    TaskRef<sequencer::Fill> fill() {
        return fill("");
    }
    TaskRef<sequencer::Fill> fill(const String& name) {
        return create_sequence<sequencer::Fill>(name);
    }

    TaskRef<sequencer::Flash> flash() {
        return flash("");
    }
    TaskRef<sequencer::Flash> flash(const String& name) {
        return create_sequence<sequencer::Flash>(name);
    }

    TaskRef<sequencer::Line> line() {
        return line("");
    }
    TaskRef<sequencer::Line> line(const String& name) {
        return create_sequence<sequencer::Line>(name);
    }

    TaskRef<sequencer::MoodMachine> mood_machine() {
        return mood_machine("");
    }
    TaskRef<sequencer::MoodMachine> mood_machine(const String& name) {
        return create_sequence<sequencer::MoodMachine>(name);
    }

    TaskRef<sequencer::Progress> progress() {
        return progress("");
    }
    TaskRef<sequencer::Progress> progress(const String& name) {
        return create_sequence<sequencer::Progress>(name);
    }
    sequencer::Progress* progress_ratio(const String& name, const float rate) {
        return get_sequence<sequencer::Progress>(name)->ratio(rate);
    }
    sequencer::Progress* progress_percent(const String& name, const float percent) {
        return get_sequence<sequencer::Progress>(name)->percent(percent);
    }

    TaskRef<sequencer::Random> random() {
        return random("");
    }
    TaskRef<sequencer::Random> random(const String& name) {
        return create_sequence<sequencer::Random>(name);
    }

    TaskRef<sequencer::RGBW> rgbw() {
        return rgbw("");
    }
    TaskRef<sequencer::RGBW> rgbw(const String& name) {
        return create_sequence<sequencer::RGBW>(name);
    }

    TaskRef<sequencer::RRGGBBWW> rrggbbww() {
        return rrggbbww("");
    }
    TaskRef<sequencer::RRGGBBWW> rrggbbww(const String& name) {
        return create_sequence<sequencer::RRGGBBWW>(name);
    }

    TaskRef<sequencer::Triangle> triangle() {
        return triangle("");
    }
    TaskRef<sequencer::Triangle> triangle(const String& name) {
        return create_sequence<sequencer::Triangle>(name);
    }

    TaskRef<sequencer::Sequence> sequence() {
        return sequence("");
    }
    TaskRef<sequencer::Sequence> sequence(const String& name) {
        return create_sequence<sequencer::Sequence>(name);
    }

    TaskRef<sequencer::TweenSolid> tween_solid() {
        return tween_solid("");
    }
    TaskRef<sequencer::TweenSolid> tween_solid(const String& name) {
        return create_sequence<sequencer::TweenSolid>(name);
    }

    TaskRef<sequencer::TweenEach> tween_each() {
        return tween_each("");
    }
    TaskRef<sequencer::TweenEach> tween_each(const String& name) {
        return create_sequence<sequencer::TweenEach>(name);
    }

    template <typename TaskType>
    TaskRef<TaskType> overwrite() {
        return create_sequence<TaskType>("overwrite");
    }

    // ---------- Accessor ----------

    CPixelView<CRGB>& get_pixel_view() { return leds; }
    const CPixelView<CRGB>& get_pixel_view() const { return leds; }

    template <typename TaskType = Base>
    TaskRef<TaskType> get_sequence(const String& name) const {
        if (this->existsSubTask(name))
            return this->getSubTaskByName<TaskType>(name);
        else
            return nullptr;
    }

    template <typename TaskType = Base>
    TaskRef<TaskType> get_sequence_by_index(const size_t idx) const {
        if (idx < this->numSubTasks())
            return this->getSubTaskByIndex<TaskType>(idx);
        else
            return nullptr;
    }

private:
    template <typename SequenceType>
    TaskRef<SequenceType> create_sequence(const String& name) {
        this->subtask<SequenceType>(name, [](TaskRef<SequenceType>) {});
        auto task = this->getSubTaskByIndex<SequenceType>(this->numSubTasks() - 1);
        assign_leds_to_task(task);
        return task;
    }

    template <typename T>
    void assign_leds_to_task(T& task) {
        if (b_layered) {
            task->allocate(leds.size());
        } else {
            task->attach(leds);
        }
        // erase sequence if stopped by default
        task->setAutoErase(true);
    }
};

// ---------- Create LEDController ----------

template <ESPIChipsets CHIPSET, size_t N_LEDS, uint8_t DATA_PIN, uint8_t CLOCK_PIN, EOrder RGB_ORDER = RGB, uint32_t SPI_DATA_RATE = DATA_RATE_MHZ(12)>
class SPIController : public Controller {
    CRGBArray<N_LEDS> leds;

public:
    virtual ~SPIController() {}

    SPIController(const String& name)
    : Controller(name, leds) {
        this->fastled = &FastLED.addLeds<CHIPSET, DATA_PIN, CLOCK_PIN, RGB_ORDER, SPI_DATA_RATE>(leds, N_LEDS);
    }
};

template <template <uint8_t DATA_PIN, EOrder RGB_ORDER> class CHIPSET, size_t N_LEDS, uint8_t DATA_PIN, EOrder RGB_ORDER = RGB>
class ClocklessController : public Controller {
    CRGBArray<N_LEDS> leds;

public:
    virtual ~ClocklessController() {}

    ClocklessController(const String& name)
    : Controller(name, leds) {
        this->fastled = &FastLED.addLeds<CHIPSET, DATA_PIN, RGB_ORDER>(leds, N_LEDS);
    }
};

}  // namespace led

using LEDController = led::Controller;

#endif  //ARDUINO_LEDMANAGER_CONTROLLER_H
