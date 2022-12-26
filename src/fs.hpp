#ifndef FS_HPP
#define FS_HPP

#include <ArduinoJson.h>
#include <LittleFS.h>
#include <config.h>
#include <mqtt.hpp>

void setupFs() {
    if (LittleFS.begin()) {
        File file = LittleFS.open(ConfigFile, "r");

        if (!file || file.isDirectory()) return;

        if (file.size() > 0) {
            DynamicJsonDocument config(1024);
            deserializeJson(config, file);
            feedings = config.as<JsonArray>();
//            for (JsonObject item: feedings) {
//                const char *time = item["time"];
//                long count = item["count"];
//                Serial.println(time);
//                Serial.println(count);
//            }

            tCountdown.enableIfNot();
        }

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
