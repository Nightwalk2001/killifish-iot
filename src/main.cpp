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

Task tButton(0, TASK_FOREVER, &button, &hRunner, true);

Task tStepper(0, TASK_FOREVER, &stepperLoop, &runner, true);

Task tPubSub(0, TASK_FOREVER, &pubsubLoop, &runner, true);

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
        digitalWrite(PILOT_LAMP, HIGH);
        if (pubsub.connected()) {
            if (tLed.isEnabled()) tLed.disable();
        } else {
            tLed.setInterval(1000);
            if (!tLed.isEnabled()) tLed.enable();
        }
    } else {
        digitalWrite(PILOT_LAMP, LOW);
        if (!tLed.isEnabled()) {
            tLed.setInterval(350);
            tLed.enable();
        }
    }
}


void setup() {
    Serial.begin(9600);

    setupPinMode();
    WiFiX::connect();
    Mqtt::connect();
//timeClient.begin();
//    setupFs();
//
    stepper.setRpm(12);
    runner.setHighPriorityScheduler(&hRunner);
    runner.enableAll(true);
}

void loop() {
    runner.execute();
}
