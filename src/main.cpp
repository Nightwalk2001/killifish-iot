#include <Arduino.h>
#include <wifi.cuh>

Wifi wifi;

void setup() {
    Serial.begin(9600);
}

void loop() {
    Serial.println("hello");
    delay(1000);
}
