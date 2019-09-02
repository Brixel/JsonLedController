#include "Arduino.h"
#include <ArduinoJson.h>
#include <MQTTHandler.h>
#include <LedHandler.h>
#include <SerialHandler.h>

//WiFi params
const char *ssid = "Brixel";
const char *password = "CLUBMATE2015";

bool DoAction(JsonObject &ActionObj, String &strErrorMessage);
void setup_wifi();

MQTTHandler mqttHandler;
LedHandler ledHandler;
SerialHandler serialHandler;