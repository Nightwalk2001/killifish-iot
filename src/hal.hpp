#ifndef HAL_HPP
#define HAL_HPP

#include <config.h>

void setupPinMode() {
    pinMode(BTN, INPUT);
    pinMode(PILOT_LAMP, OUTPUT);
}

#endif
