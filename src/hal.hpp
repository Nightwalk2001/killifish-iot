#ifndef HAL_HPP
#define HAL_HPP

#include <config.h>

void setupPinMode() {
    pinMode(BTN, INPUT);
    pinMode(POWER_LED, OUTPUT);
}

void blinkAlarm() {
}

#endif
