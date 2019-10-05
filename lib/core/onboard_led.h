#ifndef CORE_ONBOARD_LED_H
#define CORE_ONBOARD_LED_H

#include <stdint.h>
#include "Arduino.h"

namespace onboard_led {
    namespace {
        bool ready = false;
    }

    const uint32_t FLASH_DURATION = 10; // ms
    const uint32_t ONBOARD_LED = D4;

    void on() {
        digitalWrite(ONBOARD_LED, LOW);
    }

    void off() {
        digitalWrite(ONBOARD_LED, HIGH);
    }

    void flash() {
        on();
        delay(FLASH_DURATION);
        off();
    }

    void init() {
        if (ready) return;
        pinMode(ONBOARD_LED, OUTPUT);
        off();
        ready = true;
    }

    const bool isOn() {
        return !digitalRead(ONBOARD_LED);
    }
}

#endif
