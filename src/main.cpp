#define _TASK_SLEEP_ON_IDLE_RUN
#define _TASK_PRIORITY

#include <Arduino.h>
#include <TaskScheduler.h>
#include <config.h>
#include <variable.h>
#include <hal.hpp>
#include <wifi.hpp>
#include <mqtt.hpp>
#include <fs.hpp>

Scheduler runner;

void button();

void stepperLoop();

void inspect();

//void ledControl();

void pubsubLoop() {
    pubsub.loop();
}

void report() {
    digitalWrite(INFRARED_LED, LOW);

    if (pubsub.connected()) {
        DynamicJsonDocument doc(128);
        doc["id"] = DEVICE_ID;
        doc["trigger"] = trigger;
        doc["times"] = turns;
        doc["amount"] = detected;
        String out;
        serializeJson(doc, out);
        pubsub.publish(RESULT_TOPIC, out.c_str());
    }

    detected = 0;
}

void reconnect() {
    if (WiFi.status() != WL_CONNECTED) {
//        tLedControl.disable();
        WiFiX::connect();
        if (WiFi.status() == WL_CONNECTED) Mqtt::connect();
    } else {
        if (!pubsub.connected()) Mqtt::connect();
    }
}

void stepperLoop() {
    stepper.run();
}

//void led() {
//    if (pilotLampOn) {
//        digitalWrite(PILOT_LAMP, LOW);
//        pilotLampOn = false;
//    } else {
//        digitalWrite(PILOT_LAMP, HIGH);
//        pilotLampOn = true;
//    }
//}

void countdown() {
    timeClient.update();
    String now = timeClient.getFormattedTime().substring(0, 5);

    for (const auto &item: feedings) {
        const char *time = item["time"];
        long count = item["count"];

        if (strcmp(now.c_str(), time) == 0) {
            turns = count;
            trigger = "auto";
//            pubsub.publish("countdown", now.c_str());
            tFeed.enableIfNot();
        }
    }
}

Task tButton(0, TASK_FOREVER, &button, &runner, true);

Task tStepper(0, TASK_FOREVER, &stepperLoop, &runner, true);

Task tCountdown(1000 * 60, TASK_FOREVER, &countdown, &runner, false);

Task tPubsub(0, TASK_FOREVER, &pubsubLoop, &runner, true);

Task tFeed(1, TASK_FOREVER, &inspect, &runner, false);

//Task tLed(500, TASK_FOREVER, &led, &runner, false);

Task tReconnect(1000 * 120, TASK_FOREVER, &reconnect, &runner, true);

//Task tLedControl(1000 * 10, TASK_FOREVER, &ledControl, &runner, true);

void inspect() {
    unsigned long iter = tFeed.getRunCounter();

    if (iter == 0 || iter % 2100 == 1) {
        digitalWrite(INFRARED_LED, HIGH);
        stepper.moveCW(1024 * turns);
    }
    if (iter > 0 && iter % 2100 == 0) {
        report();
        tFeed.disable();
    }
    int infrared_value = digitalRead(INFRARED);
    if (infrared_value == 1) detected++;
}

void button() {
    currentState = digitalRead(BTN);
    if (currentState != prevState) {
        if (currentState == LOW) btnCounter = millis();
        else {
            unsigned long diff = millis() - btnCounter;
            if (diff < LONG_PRESS_DURATION && diff >= PRESS_DURATION) {
                turns = 1;
                trigger = "physical";
                tFeed.enable();
            };
            if (diff >= LONG_PRESS_DURATION) reset();
        }
        prevState = currentState;
    }
}

//void ledControl() {
//    if (WiFi.status() == WL_CONNECTED) {
//        tCountdown.enableIfNot();
//        if (tLed.isEnabled()) {
//            tLed.disable();
//            digitalWrite(PILOT_LAMP, HIGH);
//        }
//    } else {
//        if (tCountdown.isEnabled()) tCountdown.disable();
//        tLed.enableIfNot();
//    }
//}

void setup() {
    setupPinMode();

    stepper.setRpm(12);

    WiFiX::connect();
    if (WiFi.status() == WL_CONNECTED) Mqtt::connect();

    setupFs();
}

void loop() {
    runner.execute();
}
