#ifndef CORE_DIGITAL_INPUT_H
#define CORE_DIGITAL_INPUT_H

#include <stdint.h>
#include "Arduino.h"

class DigitalInput {
public:
    DigitalInput(const uint32_t pin) : _pin(pin) {}

    // call this within `void setup() {...}`
    void init() {
        pinMode(_pin, INPUT);
    }

    void update() {
        _previousState = _state;
        _state = digitalRead(_pin);
    }

    const bool hasChanged() const {
        return _state != _previousState;
    }

    const bool state() const {
        return _state;
    }

private:
    uint32_t _pin;
    bool _state = false;
    bool _previousState = false;
};

#endif
