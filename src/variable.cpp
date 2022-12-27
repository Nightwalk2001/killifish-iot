#include <config.h>
#include <variable.h>

WiFiClient wifi;
WiFiUDP udp;
PubSubClient pubsub(wifi);
NTPClient timeClient(udp, "cn.pool.ntp.org", 3600 * 8);
CheapStepper stepper(IN1, IN2, IN3, IN4);

DynamicJsonDocument configDocument(1024);
JsonArray feedings;

unsigned long btnCounter = 0;
int prevState = HIGH;
int currentState = HIGH;

uint8 turns = 0;
uint detected = 0;
String trigger;
