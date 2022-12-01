#ifndef PUBSUB_CUH
#define PUBSUB_CUH


class PubSub {
public:
    const char *broker;
    int port;
    const char *user;
    const char *password;
    PubSub();
    void setup();
};


#endif
