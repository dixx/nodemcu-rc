#include <stdint.h>
#include "Arduino.h"
#include "core.h"

const uint32_t SWITCH = D1;
const uint32_t FLASH_DURATION = 200; // ms

void setup() {
    serial::init();
    onboard_led::init();
    pinMode(SWITCH, INPUT);
}

void loop() {
    if (digitalRead(SWITCH) && !onboard_led::isOn()) {
        onboard_led::on();
        delay(FLASH_DURATION);
        onboard_led::off();
        delay(FLASH_DURATION);
    }
}
