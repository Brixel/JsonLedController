#include "LedHandler.h"
#include "ESP8266WiFi.h"
#include <ArduinoJson.h>
#include "PubSubClient.h"

class MQTTHandler
{
private:
    void _Reconnect();
    WiFiClient _nsClient;
    PubSubClient _psClient;
    const char *_mqtt_server = "192.168.20.100";

protected:
public:
    void Init();
    void Loop();
    void Callback(char *topic, byte *payload, unsigned int length);
};

extern MQTTHandler mqttHandler;