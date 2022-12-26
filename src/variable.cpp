#include <config.h>
#include <variable.h>

WiFiClient wifi;
WiFiUDP udp;
PubSubClient pubsub(wifi);
NTPClient timeClient(udp, "cn.ntp.org.cn", 3600 * 8);
CheapStepper stepper(IN1, IN2, IN3, IN4);

JsonArray feedings;

unsigned long btnCounter = 0;
int prevState = HIGH;
int currentState = HIGH;

volatile bool POWER_LED_ON = false;

uint8 turns = 0;
uint detected = 0;
