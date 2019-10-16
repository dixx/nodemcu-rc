#ifndef CORE_ANALOG_INPUT_H
#define CORE_ANALOG_INPUT_H

#include <stdint.h>
#include "Arduino.h"

class AnalogInput {
public:
    AnalogInput(const uint32_t pin) : _pin(pin) {}

    // call this within `void setup() {...}`
    void init() {}

    void update() {
        _previousValue = _value;
        _value = analogRead(_pin);
    }

    const bool hasChanged() const {
        // volatile due to temperature changes and voltage noise, use with caution!
        // TODO: use trend detection instead
        return _value != _previousValue;
    }

    const int16_t value() const {
        return _value;
    }

private:
    uint32_t _pin;
    int16_t _value = 0;
    int16_t _previousValue = 0;
};

#endif
