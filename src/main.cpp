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

Scheduler runner, hRunner;

void button();

void stepperLoop();

void inspect();

void ledControl();

void pubsubLoop() {
    pubsub.loop();
}

void report() {
    digitalWrite(INFRARED_LED, LOW);

    if (pubsub.connected()) {
        DynamicJsonDocument doc(1024);
        doc["id"] = DEVICE_ID;
        doc["trigger"] = "manual";
        doc["amount"] = detected;
        String out;
        serializeJson(doc, out);
        pubsub.publish(RESULT_TOPIC, out.c_str());
    }

    detected = 0;
}

void reconnect() {
    if (WiFi.status() != WL_CONNECTED) {
        WiFiX::connect();
        if (WiFi.status() == WL_CONNECTED) Mqtt::connect();
    } else {
        if (!pubsub.connected()) Mqtt::connect();
    }
}

void stepperLoop() {
    stepper.run();
}

void led() {
    if (POWER_LED_ON) {
        digitalWrite(PILOT_LAMP, LOW);
        POWER_LED_ON = false;
    } else {
        digitalWrite(PILOT_LAMP, HIGH);
        POWER_LED_ON = true;
    }
}

void countdown() {
    timeClient.update();
    String now = timeClient.getFormattedTime().substring(0, 5);
    for (const auto &item: feedings) {
        const char *time = item["time"];
        long count = item["count"];

        if (strcmp(now.c_str(), time) == 0) {
            turns = count;
            tFeed.enableIfNot();
        }
    }
}

Task tButton(0, TASK_FOREVER, &button, &hRunner, true);

Task tStepper(0, TASK_FOREVER, &stepperLoop, &runner, true);

Task tCountdown(1000 * 60, TASK_FOREVER, &countdown, &hRunner, false);

Task tPubsub(0, TASK_FOREVER, &pubsubLoop, &runner, false);

Task tFeed(1, TASK_FOREVER, &inspect, &runner, false);

Task tLed(350, TASK_FOREVER, &led, &runner, false);

Task tReconnect(1000 * 120, TASK_FOREVER, &reconnect, &runner, true);

Task tLedControl(1000 * 10, TASK_FOREVER, &ledControl, &runner, true);

void inspect() {
    unsigned long iter = tFeed.getRunCounter();

    if (iter == 0 || iter % 2500 == 1) {
        digitalWrite(INFRARED_LED, HIGH);
        stepper.moveCW(2048 * turns);
    }
    if (iter > 0 && iter % 2500 == 0) {
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
                tFeed.enable();
            };
            if (diff >= LONG_PRESS_DURATION) reset();
        }
        prevState = currentState;
    }
}

void ledControl() {
    if (WiFi.status() == WL_CONNECTED) {
        tCountdown.enableIfNot();
        if (pubsub.connected()) {
            tPubsub.enableIfNot();
            if (tLed.isEnabled()) tLed.disable();
        } else {
            if (tPubsub.isEnabled()) tPubsub.disable();
            tLed.setInterval(900);
            tLed.enableIfNot();
        }
    } else {
        if (tCountdown.isEnabled()) tCountdown.disable();
        if (tPubsub.isEnabled()) tPubsub.disable();
        tLed.setInterval(350);
        tLed.enableIfNot();
    }
}

void setup() {
    setupPinMode();

    stepper.setRpm(12);

    WiFiX::connect();
    Mqtt::connect();

    setupFs();

    runner.setHighPriorityScheduler(&hRunner);
    runner.enableAll(true);
}

void loop() {
    runner.execute();
}
