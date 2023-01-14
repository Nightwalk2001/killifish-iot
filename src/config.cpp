#include <config.h>

const char *DEVICE_ID = "01A03";

const char *SSID = "KNF_auto";
const char *PASSPHRASE = "WLreg10jq";

const char *BROKER = "192.168.235.14";
const int PORT = 1883;
const char *USER = "tank_group1";
const char *PASSWORD = "tank_group1";
const char *AUTO_TOPIC = "feeding-times";
const char *MANUAL_TOPIC = "manual-feeding";
const char *RESULT_TOPIC = "feeding-res";
const char *WILL_TOPIC = "will";

const uint8 Threshold = 15;
const char *ConfigFile = "./conf.json";

const uint16 PRESS_DURATION = 100;
const uint16 LONG_PRESS_DURATION = 3000;
