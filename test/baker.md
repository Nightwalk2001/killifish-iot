```c++
const unsigned long PRESS_DURATION = 100; // 单击最小时间间隔100ms
const unsigned long LONG_PRESS_DURATION = 3000; // 长按最小时间间隔3000ms

unsigned long counter = 0; // 时间间隔计时器
int prevState = HIGH;
int currentState; // 按钮的前一时刻// 状态和当前时刻状态
unsigned int amount = 0; // 检测到的光强小于阈值的次数

WiFiClient wifi;
PubSubClient pubsub(wifi);
CheapStepper stepper(IN1, IN2, IN3, IN4); // 电机控制对象
Scheduler ts; // 任务调度器对象

void setupWifi() {
    WiFi.begin(SSID, PASSPHRASE);
    while (WiFi.status() != WL_CONNECTED) {
        delay(3000);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the WiFi network");
}

void button();

void stepperLoop();

void feed(int c);

void inspect();

void cbk();

void listen() {
    pubsub.loop();
}

// 按钮loop
Task listenBtn(0, TASK_FOREVER, &button, &ts, true);
// 电机loop 如果电机不转动，把下面那行解除注释
Task stepperTask(0, TASK_FOREVER, &stepperLoop, &ts, true);

Task pubsubTask(0, TASK_FOREVER, &listen, &ts, true);

//Task l(0, TASK_FOREVER, &cbk,&ts, false);
Task i(1, TASK_FOREVER, &inspect,&ts ,false);

// 重置函数，真正的代码在loop函数下方，被注释掉了
// 功能是删除保存在闪存里的饲喂时间、数量信息
void reset() {
//    LittleFS.remove("./conf.json");
    pubsub.publish(PUB_TOPIC, "恢复出厂设置");
}

// 处理按钮事件
void button() {
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

void stepperLoop() {
    stepper.run();
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

//void step() {
//
//}

void feed(int c) {
    i.enable();
}

// 监测饲料是否成功掉下
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

void callback(char *topic, byte *payload, unsigned int length) {
    DynamicJsonDocument doc(1024);

    DeserializationError error = deserializeJson(doc, payload);

    File f = LittleFS.open("./conf.json", "w");
    serializeJson(doc, f);
    f.close();

//    int count = doc["count"];
//    feed(count);
}

void setupMqtt() {
    pubsub.setServer(BROKER, PORT);
    pubsub.setCallback(callback);
    while (!pubsub.connected()) {
        Serial.printf("%s connects to mqtt broker\n", DEVICE_ID);

        if (pubsub.connect(DEVICE_ID, USER, PASSWORD))
            Serial.println("mqtt broker connected");
        else delay(3000);
    }

    pubsub.subscribe(SUB_TOPIC);
}

void error(const char *message) {
    DynamicJsonDocument doc(1024);
    doc["message"] = message;
    doc["id"] = DEVICE_ID;
    String out;
    serializeJson(doc, out);
    pubsub.publish(ERROR_TOPIC, out.c_str());
}

void setupFs() {
    bool opened = LittleFS.begin();
    if (!opened) Serial.println("An Error has occurred while mounting LittleFS");

    File file = LittleFS.open("./conf.json", "r");

    DynamicJsonDocument config(1024);

//    DynamicJsonDocument array(CAPACITY);

    if (!file || file.isDirectory()) return;
    else deserializeJson(config, file);
    file.close();

    String out;
    serializeJson(config, out);
    pubsub.publish(PUB_TOPIC, out.c_str());
//
//    auto array = config.as<JsonArray>();
//
//    for (JsonObject i: array) {
//        int count = i.getMember("count");
//        Serial.println(count);
//        const char *time = i.getMember("time");
//        pubsub.publish(PUB_TOPIC, time);
//    }

}
    
void save() {

}

void setup() {
    Serial.begin(9600);

    // 设置按钮、光强传感器、光强传感器LED、电源LED引脚IO模式
    pinMode(BTN, INPUT);
    pinMode(INFRARED, INPUT);
    pinMode(INFRARED_LED,OUTPUT);
    pinMode(POWER_LED, OUTPUT);

    setupWifi();
    setupMqtt();
//    setupFs();

    stepper.setRpm(12);
}

void loop() {
    ts.execute();
}

```
