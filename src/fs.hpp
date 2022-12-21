#ifndef FS_HPP
#define FS_HPP

#include <ArduinoJson.h>
#include <LittleFS.h>
#include <config.h>
#include <mqtt.hpp>

void setupFs() {
    if (LittleFS.begin()) {
        File file = LittleFS.open(ConfigFile, "r");

        DynamicJsonDocument config(1024);

        if (!file || file.isDirectory()) return;
        else deserializeJson(config, file);

        feedings = config.as<JsonArray>();

        file.close();
    }
}

void reset() {
    LittleFS.remove(ConfigFile);
    if (pubsub.connected()) {
        pubsub.publish(AUTO_TOPIC, "恢复出厂设置");
    }
}

#endif
