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

extern JsonArray feedings;

extern unsigned long btnCounter;
extern int prevState;
extern int currentState;

extern bool POWER_LED_ON;

extern uint8 turns;
extern uint detected;

extern Task tFeed;
extern Task tPubsub;

#endif
