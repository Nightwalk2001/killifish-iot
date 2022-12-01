#ifndef WIFI_CUH
#define WIFI_CUH

void hello();

class Wifi {
private:
    const char *ssid;
    const char *password;
public:
   Wifi();

    bool available;

    void connect();

    static void reconnect();

    static void blink(uint8_t pin);
};

#endif
