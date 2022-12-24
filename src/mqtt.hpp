#ifndef MQTT_HPP
#define MQTT_HPP

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <WiFiUdp.h>
#include <wifi.hpp>
#include <config.h>
#include <variable.h>

void saveConfig(byte *raw) {
    DynamicJsonDocument doc(1024);

    deserializeJson(doc, raw);

    File f = LittleFS.open(ConfigFile, "w");
    serializeJson(doc, f);
    f.close();
}

void callback(const char *topic, byte *payload, unsigned int length) {
    if (topic == MANUAL_TOPIC) {
        DynamicJsonDocument doc(1024);

        deserializeJson(doc,payload);
        if (doc["device"] == DEVICE_ID) {
            turns = doc["count"];
            tFeed.enable();
        }
    }

    if (topic == AUTO_TOPIC)  saveConfig(payload);
}

class Mqtt {
public:
    Mqtt() = default;

    static void connect() {
//        uint8 suffix = ((DEVICE_ID[0] - 48) * 10 + (DEVICE_ID[1] - 48)) % 4;
//        uint16 port = 1883 + suffix;
        uint8 count = 0;

        pubsub.setServer(BROKER, PORT);

        while (!pubsub.connected()) {
            delay(1000);
            if (pubsub.connect(DEVICE_ID, USER, PASSWORD) || ++count > Threshold) break;
        }
        if (count < Threshold) {
            tPubsub.enableIfNot();
            pubsub.setCallback(callback);
            pubsub.subscribe(AUTO_TOPIC);
            pubsub.subscribe(MANUAL_TOPIC);
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
