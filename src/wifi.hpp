#ifndef WIFI_HPP
#define WIFI_HPP

#include <ESP8266WiFi.h>
#include <config.h>

class WiFiX {
public:
    WiFiX() = default;

    static void connect() {
        uint8 count = 0;
        WiFi.begin(SSID, PASSPHRASE);
        while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
            if (++count >= Threshold) break;
        }
        if (count < Threshold) WiFiAvailable = true;
    };

    static void blink(uint8_t pin) {
        digitalWrite(pin, HIGH);
        delay(1000);
        digitalWrite(pin, LOW);
    };
};

#endif
