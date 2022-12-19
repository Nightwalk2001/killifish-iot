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

void ledControl();

void pubsubLoop() {
    pubsub.loop();
}

void report() {
    digitalWrite(INFRARED_LED, LOW);

    if (MqttAvailable) {
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
        WiFiAvailable = false;
        MqttAvailable = false;
        WiFiX::connect();
        if (WiFiAvailable) Mqtt::connect();
    }
}

void stepperLoop() {
    stepper.run();
}

void led() {
    if (POWER_LED_ON) {
        analogWrite(POWER_LED, 0);
        POWER_LED_ON = false;
    } else {
        analogWrite(POWER_LED, 50);
        POWER_LED_ON = true;
    }
}

Task tButton(0, TASK_FOREVER, &button, &runner, true);

Task tStepper(0, TASK_FOREVER, &stepperLoop, &runner, true);

Task tPubSub(0, TASK_FOREVER, &pubsubLoop, &runner, true);

Task tFeed(1, TASK_FOREVER, &inspect, &runner, false);

Task tLedControl(60000, TASK_FOREVER, &ledControl, &runner, true);

Task tWifiLed(200, TASK_FOREVER, &led, &runner, true);

Task tMqttLed(1000, TASK_FOREVER, &led, &runner, true);

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
            };  // 单击按钮
            if (diff >= LONG_PRESS_DURATION) reset(); // 长按按钮
        }
        prevState = currentState;
    }
}

void ledControl() {
    if (WiFiAvailable) {
        analogWrite(POWER_LED,10);
        if (tWifiLed.isEnabled()) tWifiLed.disable();
        if (MqttAvailable) {
            if (tMqttLed.isEnabled()) tMqttLed.disable();
        } else {
            if (!tMqttLed.isEnabled()) tMqttLed.enable();
        }
    } else {
        if (!tWifiLed.isEnabled()) tWifiLed.enable();
        if (tMqttLed.isEnabled()) tMqttLed.disable();
    }
}


void setup() {
    Serial.begin(9600);
//
//    setupPinMode();
    WiFiX::connect();
////    Mqtt::connect();
////    timeClient.begin();
//    setupFs();
//
    stepper.setRpm(12);
}

void loop() {
    runner.execute();
}

