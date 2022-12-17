#ifndef TASK_HPP
#define TASK_HPP

#include <TaskSchedulerDeclarations.h>
#include <config.h>
#include <fs.hpp>

void btnHandler();

void stepperLoop();

void feed(int c);

void inspect();

void listen() {
    pubsub.loop();
}

void cbk() {
    digitalWrite(INFRARED_LED,LOW);

    DynamicJsonDocument doc(1024);
    doc["id"] = DEVICE_ID;
//    doc["type"] = "manual";
    doc["amount"] = amount;
    String out;
    serializeJson(doc, out);
    pubsub.publish(PUB_TOPIC, out.c_str());

    amount = 0;
}

void reconnect() {
    if (WiFi.status() !=WL_CONNECTED) {
        WiFiAvailable = false;
        WiFiX::connect();
        if (WiFiAvailable) Mqtt::connect();
    }
}

void stepperLoop() {
    stepper.run();
}

// 按钮loop
Task listenBtn(0, TASK_FOREVER, &btnHandler, &runner, true);
// 电机loop 如果电机不转动，把下面那行解除注释
Task stepperTask(0, TASK_FOREVER, &stepperLoop, &runner, true);

Task pubsubTask(0, TASK_FOREVER, &listen, &runner, true);

//Task l(0, TASK_FOREVER, &cbk,&ts, false);
Task i(1, TASK_FOREVER, &inspect,&runner ,false);

Task reconnectTask(12000, TASK_FOREVER, &inspect,&runner, true);

Task deviceError();

void feed(int c) {
    i.enable();
}

void inspect() {
    unsigned long iter = i.getRunCounter();

    if (iter ==0 || iter % 2500 ==1) {
        digitalWrite(INFRARED_LED, HIGH);
        stepper.moveCW(2048);
    }
    if (iter > 0 && iter % 2500 ==0) {
        cbk();
        i.disable();
    }
    int infrared_value = digitalRead(INFRARED);
//    lightArray.add(lightIntensity);
    if (infrared_value ==1) amount++;
}

void btnHandler() {
    currentState = digitalRead(BTN);
    if (currentState != prevState) {
        if (currentState == LOW) counter = millis();
        else {
            unsigned long diff = millis() - counter;
            if (diff < LONG_PRESS_DURATION && diff >= PRESS_DURATION) feed(1);  // 单击按钮
            else if (diff >= LONG_PRESS_DURATION) reset(); // 长按按钮
        }
        prevState = currentState;
    }
}

#endif
