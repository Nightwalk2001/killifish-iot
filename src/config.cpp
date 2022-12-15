#include <ArduinoJson.h>
#include <config.h>

const char *SSID = "mqtt";
const char *PASSPHRASE = "12345678";

const char *BROKER = "120.79.40.242";
const int PORT = 1883;
const char *USER = "admin";
const char *PASSWORD = "public";
const char *SUB_TOPIC = "feeding-times";
const char *PUB_TOPIC = "feeding-res";
const char *ERROR_TOPIC = "error-occur";

const unsigned long PRESS_DURATION = 100;
const unsigned long LONG_PRESS_DURATION = 3000;

unsigned long counter = 0;
int prevState = HIGH;
int currentState = HIGH;
unsigned int amount = 0;

JsonArray feedings;

WiFiClient wifi;
WiFiUDP udp;
PubSubClient pubsub(wifi);
NTPClient timeClient(udp, "ntp.aliyun.com");
Scheduler scheduler;
CheapStepper stepper(IN1, IN2, IN3, IN4);
