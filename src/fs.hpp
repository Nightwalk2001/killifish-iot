#ifndef FS_HPP
#define FS_HPP

#include <ArduinoJson.h>
#include <LittleFS.h>
#include <config.h>
#include <mqtt.hpp>

void setupFs() {
    bool opened = LittleFS.begin();
    if (!opened) Serial.println("An Error has occurred while mounting LittleFS");

    File file = LittleFS.open("./conf.json", "r");

    DynamicJsonDocument config(1024);

    if (!file || file.isDirectory()) return;
    else deserializeJson(config, file);
    file.close();
//
//    String out;
//    serializeJson(config, out);
//    pubsub.publish(PUB_TOPIC, out.c_str());

}

void save() {

}

void reset() {
    LittleFS.remove("./conf.json");
    if (Mqtt::available) {
        pubsub.publish(PUB_TOPIC, "恢复出厂设置");
    }
}

#endif
