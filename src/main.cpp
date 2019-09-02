#include "main.h"

void setup()
{
   Serial.begin(115200);
   Serial.println("Starting!");
   setup_wifi();
   mqttHandler.Init();
   ledHandler.Init();
}

void loop()
{
   mqttHandler.Loop();
   ledHandler.Loop();
   serialHandler.Loop();
}

void setup_wifi()
{

   delay(10);
   // We start by connecting to a WiFi network
   Serial.println();
   Serial.printf("Connecting to %s", ssid);

   WiFi.begin(ssid, password);

   while (WiFi.status() != WL_CONNECTED)
   {
      delay(500);
      Serial.print(".");
   }

   randomSeed(micros());

   Serial.println("");
   Serial.printf("WiFi connected, IP address: %i", WiFi.localIP().toString().c_str());
}