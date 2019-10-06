#include <stdint.h>
#include "Arduino.h"
#include "Ticker.h"
#include "core.h"

const uint32_t SWITCH1 = D1;
const uint32_t SWITCH2 = D2;
const uint32_t OUT1 = D5;
const uint32_t OUT2 = D6;
const uint32_t FLASH_DURATION = 200; // ms
Ticker blocker;
bool writingAllowed = true;

void allowWriting() {
    writingAllowed = true;
    blocker.detach();
}

void denyWriting() {
    writingAllowed = false;
    blocker.attach(1.0, allowWriting);
}

void printA0() {
    if (writingAllowed) {
        Serial.println((analogRead(A0) * 3.3 ) / (4095));
        denyWriting();
    }
}

void checkInput() {
    if (digitalRead(SWITCH1)) {
        digitalWrite(OUT1, HIGH);
        printA0();
    } else {
        digitalWrite(OUT1, LOW);
    }
    if (digitalRead(SWITCH2)) {
        digitalWrite(OUT2, HIGH);
    } else {
        digitalWrite(OUT2, LOW);
    }
}

void setup() {
    serial::init();
    onboard_led::init();
    pinMode(SWITCH1, INPUT);
    pinMode(SWITCH2, INPUT);
    pinMode(OUT1, OUTPUT);
    pinMode(OUT2, OUTPUT);
}

void loop() {
    checkInput();
}
