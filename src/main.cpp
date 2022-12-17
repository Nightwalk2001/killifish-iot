#include <Arduino.h>
#include <config.h>
#include <hal.hpp>
#include <task.hpp>

void setup() {
    setupPinMode();
    WiFiX::connect();
    Mqtt::connect();
//    timeClient.begin();
    setupFs();
}

void loop() {
    runner.execute();
}
