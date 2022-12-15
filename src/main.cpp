#include <Arduino.h>
#include <hal.hpp>
#include <wifi.hpp>
#include <task.hpp>

void setup() {
    setupPinMode();
    WiFiX::connect();
    Mqtt::connect();
    timeClient.begin();
    setupFs();
}

void loop() {
    scheduler.execute();
}
