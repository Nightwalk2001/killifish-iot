#ifndef VARIABLE_H
#define VARIABLE_H

#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <ArduinoJson.h>
#include <CheapStepper.h>
#include <TaskSchedulerDeclarations.h>

extern WiFiClient wifi;
extern WiFiUDP udp;
extern PubSubClient pubsub;
extern NTPClient timeClient;
extern CheapStepper stepper;

extern DynamicJsonDocument configDocument;
extern JsonArray feedings;

extern unsigned long btnCounter;
extern int prevState;
extern int currentState;

extern uint8 turns;
extern uint detected;
extern String trigger;

extern Task tFeed;
extern Task tPubsub;
extern Task tLedControl;
extern Task tCountdown;

#endif
