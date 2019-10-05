#ifndef CORE_SERIAL_H
#define CORE_SERIAL_H

#include "Arduino.h"

namespace serial {
    namespace {
        bool ready = false;
    }

    void init()
    {
        if (ready) return;
        Serial.begin(115200);
        delay(5000);
        Serial.println("");
        ready = true;
    }
}

#endif
