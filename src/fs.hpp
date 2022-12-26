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

        if (file.available() > 0) {
            deserializeJson(configDocument, file);
            feedings = configDocument.as<JsonArray>();
            if (WiFi.status()==WL_CONNECTED) tCountdown.enableIfNot();
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
