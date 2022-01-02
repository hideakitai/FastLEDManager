#pragma once
#ifndef ARDUINO_LEDMANAGER_LEDSEQUENCER_H
#define ARDUINO_LEDMANAGER_LEDSEQUENCER_H

#include <TaskManager.h>
#include <FastLED.h>
#include <Tween.h>

#include "LEDSequencer/Base.h"
#include "LEDSequencer/Spin.h"
#include "LEDSequencer/Fill.h"
#include "LEDSequencer/Flash.h"
#include "LEDSequencer/Line.h"
#include "LEDSequencer/MoodMachine.h"
#include "LEDSequencer/Progress.h"
#include "LEDSequencer/Random.h"
#include "LEDSequencer/RGBW.h"
#include "LEDSequencer/Sequence.h"
#include "LEDSequencer/Triangle.h"
#include "LEDSequencer/Tween.h"

namespace LEDSequence = led::sequencer;

#endif  // ARDUINO_LEDMANAGER_LEDSEQUENCER_H
