#include <config.h>
#include <variable.h>

WiFiClient wifi;
WiFiUDP udp;
PubSubClient pubsub(wifi);
NTPClient timeClient(udp, "ntp.aliyun.com");
CheapStepper stepper(IN1, IN2, IN3, IN4);

JsonArray feedings;

unsigned long btnCounter = 0;
int prevState = HIGH;
int currentState = HIGH;

bool POWER_LED_ON = false;

uint8 turns = 0;
uint detected = 0;
