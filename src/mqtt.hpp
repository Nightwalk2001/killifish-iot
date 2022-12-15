#ifndef MQTT_HPP
#define MQTT_HPP

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <WiFiUdp.h>
#include <wifi.hpp>
#include <config.h>

void callback(char *topic, byte *payload, unsigned int length) {
    DynamicJsonDocument doc(1024);

    DeserializationError error = deserializeJson(doc, payload);

    File f = LittleFS.open("./conf.json", "w");
    serializeJson(doc, f);
    f.close();
}

class Mqtt {
public:
    static bool available;

    Mqtt() = default;

    static void connect() {
        available = false;
        int count = 0;

        pubsub.setServer(BROKER, PORT);

        while (!pubsub.connected())
            if (pubsub.connect(DEVICE_ID, USER, PASSWORD) || ++count > 60) break;

        if (count < 60) {
            available = true;
            pubsub.setCallback(callback);
            pubsub.subscribe(SUB_TOPIC);
        }
    };
};

void error(const char *message) {
    DynamicJsonDocument doc(1024);
    doc["message"] = message;
    doc["id"] = DEVICE_ID;
    String out;
    serializeJson(doc, out);
    pubsub.publish(ERROR_TOPIC, out.c_str());
}

#endif
