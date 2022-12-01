#include <pubSub.cuh>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "wireless.hpp"

std::function<void(char *, uint8_t *, unsigned int)> callback() {

}

void PubSub::setup() {
    WiFiClient wifi;
    PubSubClient client(wifi);
    client.setServer(BROKER, PORT);
    client.setCallback(callback);
    while (!client.connected()) {
        Serial.printf("%s connects to mqtt broker\n", DEVICE_ID);

        if (client.connect(DEVICE_ID, USER, PASSWORD))
            Serial.println("mqtt broker connected");
        else delay(3000);
    }

    client.subscribe(SUB_TOPIC);
}

PubSub::PubSub() :
        broker(""),
        port(1883),
        user("admin"),
        password("public") {

}

void error(const char *message) {
    DynamicJsonDocument doc(1024);
    doc["message"] = message;
    doc["id"] = DEVICE_ID;
    String out;
    serializeJson(doc, out);
    pubsub.publish(ERROR_TOPIC, out.c_str());
}
