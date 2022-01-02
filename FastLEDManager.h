#pragma once
#ifndef ARDUINO_LEDMANAGER_H
#define ARDUINO_LEDMANAGER_H

#ifdef FASTLED_MANAGER_DEBUGLOG_ENABLE
#include <DebugLogEnable.h>
#else
#include <DebugLogDisable.h>
#endif

// disable FastLED setCorrection & setTemperature and make the calc faster
#ifdef FASTLED_MANAGER_DISABLE_COLOR_CORRECTIONS
#define NO_CORRECTION 1
#endif

#include <Arduino.h>
#include <FastLED.h>
#include <TaskManager.h>

namespace led {
#if ARX_HAVE_LIBSTDCPLUSPLUS >= 201103L  // Have libstdc++11
template <typename T>
using Vec = std::vector<T>;
using namespace std;
#else
template <typename T>
using Vec = arx::vector<T>;
using namespace arx;
#endif
}  // namespace led

#include "FastLEDManager/LEDController.h"
#include "FastLEDManager/LEDSequencer.h"

namespace led {

class Manager {
    // singleton
    Manager() {
        // No limit
        FastLED.setBrightness(255);

        // We should run show() higher than frame rate to enable dithering
        // https://github.com/FastLED/FastLED/wiki/FastLED-Temporal-Dithering
        FastLED.setDither(BINARY_DITHER);  // explicitly enable dithering

        // disable blocking in FastLED.show()
        // https://github.com/FastLED/FastLED/blob/master/FastLED.cpp#L47
        FastLED.setMaxRefreshRate(0, false);  // disable blocking in FastLED.show()

        // FastLED color correction & temperature settings
        // these settings are disabled unless you define
        // #define FASTLED_SEQUENCER_ENABLE_COLOR_CORRECTIONS
        // this skips correction procedure and makes a bit faster
        //
        // FastLED final color is calcurated as:
        // final color = raw color * correction * temperature * brightness
        // https://github.com/FastLED/FastLED/blob/master/controller.h#L150
        // you can disable these correction if you define
        // #define NO_CORRECTION 1 // disable FastLED setCorrection & setTemperature
        //
        // for color correction / temperature reference
        // https://github.com/FastLED/FastLED/blob/master/color.h
        // FastLED.setCorrection(TypicalLEDStrip); // 255, 176, 240
        // FastLED.setTemperature(DirectSunlight); // 255, 255, 255
        FastLED.setCorrection(CRGB::White);
        FastLED.setTemperature(CRGB::White);
    }
    Manager(const Manager&) = delete;
    Manager& operator=(const Manager&) = delete;

    using LEDTaskRef = TaskRef<Controller>;
    std::vector<LEDTaskRef> tasks;

public:
    static Manager& get() {
        static Manager m;
        return m;
    }

    // ---------- Create LED Controller ----------

    template <
        ESPIChipsets CHIPSET,
        size_t N_LEDS,
        uint8_t DATA_PIN,
        uint8_t CLOCK_PIN,
        EOrder RGB_ORDER = RGB,
        uint32_t SPI_DATA_RATE = DATA_RATE_MHZ(12)>
    Controller& add(const String& name) {
        auto task = Tasks.add<SPIController<CHIPSET, N_LEDS, DATA_PIN, CLOCK_PIN, RGB_ORDER, SPI_DATA_RATE>>(name);
        tasks.emplace_back(task);
        // return task;
        return *(task.get());
    }

    template <
        template <uint8_t DATA_PIN, EOrder RGB_ORDER>
        class CHIPSET,
        size_t N_LEDS,
        uint8_t DATA_PIN,
        EOrder RGB_ORDER = RGB>
    Controller& add(const String& name) {
        auto task = Tasks.add<ClocklessController<CHIPSET, N_LEDS, DATA_PIN, RGB_ORDER>>(name);
        tasks.emplace_back(task);
        // return task;
        return *(task.get());
    }

    // ---------- Configuration ----------

    Manager& brightness(const uint8_t bri) {
        for (auto& t : tasks) t->brightness(bri);
        return *this;
    }

    Manager& dither(const uint8_t dither_mode) {
        for (auto& t : tasks) t->dither(dither_mode);
        return *this;
    }

    Manager& correction(const CRGB& clr_correction) {
        for (auto& t : tasks) t->correction(clr_correction);
        return *this;
    }

    Manager& temperature(const CRGB& clr_temperature) {
        for (auto& t : tasks) t->temperature(clr_temperature);
        return *this;
    }

    Manager& fadeout(const uint8_t v) {
        for (auto& t : tasks) t->fadeout(v);
        return *this;
    }

    Manager& gamma(const float v) {
        for (auto& t : tasks) t->gamma(v);
        return *this;
    }

    Manager& layer(const bool b) {
        for (auto& t : tasks) t->layer(b);
        return *this;
    }

    // TODO: divide one led stripe to multiple parts
    // maybe create new Controllers inside of Controller
    Manager& divide() {
        return *this;
    }

#ifdef ARDUINO_ARCH_ESP32
    Manager& multi_core(const bool b) {
        for (auto& t : tasks) t->multi_core(b);
        return *this;
    }
    Manager& multi_core_config(const uint8_t core, const uint32_t stack_size, const uint32_t priority) {
        for (auto& t : tasks) t->multi_core_config(core, stack_size, priority);
        return *this;
    }
#endif

    // ---------- LED Control ----------

    // drive leds with current led buffer immediately
    Manager& show(const uint8_t brightness = 0) {
        for (auto& t : tasks) t->show(brightness);
        return *this;
    }

    /// Run all sequence and block program
    Manager& spin(const uint32_t ms) {
        const uint64_t end_us = micros() + uint64_t(ms) * 1000;
        while (micros() < end_us) {
            for (auto& t : tasks) Tasks.update(t->getName());
        }
        return *this;
    }

    /// show current color once and hold it until timeout
    Manager& hold(const uint32_t ms) {
        show();
        const uint64_t end_us = micros() + uint64_t(ms) * 1000;
        while (micros() < end_us)
            ;
        return *this;
    }

    // set assigned flag not to clear the output buffer
    Manager& assign(const bool b) {
        for (auto& t : tasks) t->assign(b);
        return *this;
    }

    // ---------- LED Control (Immediate) ----------

    Manager& fill_solid(const CRGB& c) {
        for (auto& t : tasks) {
            t->fill_solid(c);
        }
        return *this;
    }

    // ---------- LED Sequencers (Non Blocking) ----------

    // create sequences each led stripes

    // ---------- Access to task / controller ----------

    LEDTaskRef get_controller_by_name(const String& name) const {
        return Tasks.getTaskByName<Controller>(name);
    }
    LEDTaskRef get_controller_by_index(const size_t idx) const {
        if (idx >= tasks.size()) {
            LOG_ERROR("Index overrun:", idx, "should be <", tasks.size());
            return nullptr;
        }
        return Tasks.getTaskByName<Controller>(tasks[idx]->getName());
    }

    Controller& operator[](const String& name) const {
        auto controller_ref = get_controller_by_name(name);
        ASSERTM(controller_ref != nullptr, "LED Controller NOT FOUND");
        return *(controller_ref.get());
    }
    Controller& operator[](const size_t idx) const {
        auto controller_ref = get_controller_by_index(idx);
        ASSERTM(controller_ref != nullptr, "LED Controller NOT FOUND");
        return *(controller_ref.get());
    }
};

}  // namespace led

#define $LED led::Manager::get()

#include <DebugLogRestoreState.h>

#endif  // ARDUINO_LEDMANAGER_H
