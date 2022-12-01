#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <TaskScheduler.h>
#include <PubSubClient.h>

#define ALERT_LED D5

WiFiClient wifi;
PubSubClient pubsub;
Scheduler ts;

const char *SSID = "mqtt";
const char *PASSPHRASE = "12345678";

int retryCount = 0;
bool wifiAvailable = false;
bool pubsubAvailable = false;
bool fsAvailable = false;
bool blinkState = false;

void blinkFn();

Task listenBtn(0, TASK_FOREVER, &blinkFn, &ts, true);

void connectWifi() {
    WiFi.begin(SSID, PASSPHRASE);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        if (++retryCount >= 60) break;
    }
    if (retryCount < 60) wifiAvailable = true;
    retryCount = 0;
}

void setupMqtt() {
    if (wifiAvailable) {
//        pubsub.setServer(BROKER, PORT);
//        pubsub.setCallback(callback);
//        while (!pubsub.connected()) {
//
//            if (pubsub.connect(DEVICE_ID, USER, PASSWORD))
//                Serial.println("mqtt broker connected");
//            else delay(3000);
//        }
//
//        pubsub.subscribe(SUB_TOPIC);
    }
}

void blinkFn() {
    if (!wifiAvailable) {
        if (blinkState) {
            blinkState = true;
            digitalWrite(ALERT_LED, HIGH);
        } else {
            blinkState = false;
            digitalWrite(ALERT_LED, LOW);
        }
    }
    delay(1000);
}

#endif
