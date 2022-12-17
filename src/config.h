#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <Arduino.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <ArduinoJson.h>
#include <CheapStepper.h>
#include <TaskSchedulerDeclarations.h>

#define DEVICE_ID "01A03"
#define BTN D5
#define INFRARED A0
#define INFRARED_LED D6
#define POWER_LED D4
#define IN1 D7
#define IN2 D2
#define IN3 D8
#define IN4 D1

extern const char *SSID;
extern const char *PASSPHRASE;

extern const char *BROKER;
extern const int PORT;
extern const char *USER;
extern const char *PASSWORD;
extern const char *SUB_TOPIC;
extern const char *PUB_TOPIC;
extern const char *ERROR_TOPIC;

extern const uint8 Threshold;
extern const char *ConfigFile;

extern const uint16 PRESS_DURATION;
extern const uint16 LONG_PRESS_DURATION;

extern JsonArray feedings;

extern unsigned long counter;
extern int prevState;
extern int currentState;
extern unsigned int amount;

extern WiFiClient wifi;
extern WiFiUDP udp;
extern PubSubClient pubsub;
extern NTPClient timeClient;
extern CheapStepper stepper;

extern bool WiFiAvailable;
extern bool MqttAvailable;
extern bool FsAvailable;

#endif
