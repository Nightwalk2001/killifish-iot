#ifndef WIFI_HPP
#define WIFI_HPP

#include <ESP8266WiFi.h>
#include <config.h>

class WiFiX {
public:
    static bool available;

    WiFiX() = default;

    static void connect() {
        available = false;
        int count = 0;
        WiFi.begin(SSID, PASSPHRASE);
        while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
            if (++count >= 60) break;
        }
        if (count < 60) available = true;
    };

    static void blink(uint8_t pin) {
        digitalWrite(pin, HIGH);
        delay(1000);
        digitalWrite(pin, LOW);
    };
};

#endif
