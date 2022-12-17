#ifndef MQTT_HPP
#define MQTT_HPP

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <WiFiUdp.h>
#include <wifi.hpp>
#include <config.h>

void saveConfig(byte *raw) {
    DynamicJsonDocument doc(1024);

    deserializeJson(doc, raw);

    File f = LittleFS.open(ConfigFile, "w");
    serializeJson(doc, f);
    f.close();
}

void callback(char *topic, byte *payload, unsigned int length) {
    saveConfig(payload);
}

class Mqtt {
public:
    Mqtt() = default;

    static void connect() {
        uint8 count = 0;

        pubsub.setServer(BROKER, PORT);

        while (!pubsub.connected()) {
            delay(1000);
            if (pubsub.connect(DEVICE_ID, USER, PASSWORD) || ++count > Threshold) break;
        }
        if (count < Threshold) {
            MqttAvailable = true;
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
