#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <wifi.cuh>

void hello() {

}

Wifi::Wifi() :
        ssid(""),
        password("123"),
        available(false) {
    ssid = "";
    password = "";
}

void Wifi::connect() {
    int count = 0;
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        if (++count >= 60) break;
    }
    if (count < 60) available = true;
}

void Wifi::reconnect() {

}

void Wifi::blink(uint8_t pin) {
    digitalWrite(pin, HIGH);
    delay(1000);
    digitalWrite(pin, LOW);
}
