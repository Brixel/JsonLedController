#include "MQTTHandler.h"

#define BASE_TOPIC "/brixel/fri3dlogo"
#define STATUS_TOPIC "/brixel/fri3dlogo/status"
#define SETTINGS_TOPIC "/brixel/fri3dlogo/settings"

void MQTTHandler::Init()
{
   _psClient = PubSubClient(_nsClient);
   _psClient.setServer(_mqtt_server, 1883);
   _psClient.setCallback(std::bind(&MQTTHandler::Callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void MQTTHandler::Loop()
{
   if (!_psClient.connected())
   {
      _Reconnect();
   }
   _psClient.loop();
}

void MQTTHandler::Callback(char *topic, byte *payload, unsigned int length)
{
   Serial.printf("Message arrived [%s]: %s", topic, payload);

   if (strcasecmp(topic, SETTINGS_TOPIC) == 0)
   {
      StaticJsonDocument<200> settings;

      DeserializationError error = deserializeJson(settings, payload);
      JsonObject settingsOjb = settings.as<JsonObject>();
      StaticJsonDocument<128> output;
      if (settingsOjb.containsKey("action"))
      {
         String strErrorMessage;
         if (ledHandler.DoAction(settingsOjb, strErrorMessage))
         {
            output["success"] = true;
         }
         else
         {
            output["success"] = false;
            output["errorMessage"] = strErrorMessage.c_str();
         }
      }
      String strOutput;
      serializeJsonPretty(output, strOutput);
      Serial.print(strOutput);
      _psClient.publish(STATUS_TOPIC, strOutput.c_str());
      // Test if parsing succeeds.
      if (error)
      {
         Serial.printf("deserializeJson() failed: %s", error.c_str());
         return;
      }
   }
}

void MQTTHandler::_Reconnect()
{
   // Loop until we're reconnected
   while (!_psClient.connected())
   {
      Serial.printf("Attempting MQTT connection...");
      // Create a random _psClient ID
      String clientId = "ESPClient-";
      clientId += String(random(0xffff), HEX);
      // Attempt to connect
      if (_psClient.connect(clientId.c_str(), STATUS_TOPIC, 1, false, "0"))
      {
         Serial.printf("MQTT connected");
         // Once connected, publish an announcement...
         _psClient.publish(STATUS_TOPIC, "1");
         // ... and resubscribe
         _psClient.subscribe(SETTINGS_TOPIC);
      }
      else
      {
         Serial.printf("failed, rc=%i,  try again in 5 seconds", _psClient.state());
         delay(5000);
      }
   }
}
