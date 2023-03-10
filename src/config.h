#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <Arduino.h>

#define BTN D5
#define INFRARED D6
#define INFRARED_LED D2
#define PILOT_LAMP D0
#define IN1 D7
#define IN2 D8
#define IN3 D2
#define IN4 D1

extern const char *DEVICE_ID;

extern const char *SSID;
extern const char *PASSPHRASE;

extern const char *BROKER;
extern const int PORT;
extern const char *USER;
extern const char *PASSWORD;
extern const char *AUTO_TOPIC;
extern const char *MANUAL_TOPIC;
extern const char *RESULT_TOPIC;
extern const char *WILL_TOPIC;

extern const uint8 Threshold;
extern const char *ConfigFile;

extern const uint16 PRESS_DURATION;
extern const uint16 LONG_PRESS_DURATION;

#endif
