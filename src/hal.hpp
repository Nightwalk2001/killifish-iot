#ifndef HAL_HPP
#define HAL_HPP

#include <config.h>

void setupPinMode() {
    pinMode(BTN, INPUT_PULLUP);
    pinMode(INFRARED, INPUT);
    pinMode(INFRARED_LED,OUTPUT);
    pinMode(PILOT_LAMP, OUTPUT);
}

#endif
