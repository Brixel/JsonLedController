#include <WS2812FX.h>
#include <ArduinoJson.h>

//Lights stuff
#define LED0_COUNT 314
#define LED0_PIN D5

#define LED1_COUNT 10
#define LED1_PIN D6

#define LED2_COUNT 2
#define LED2_PIN D7

#define LED3_COUNT 12
#define LED3_PIN D8

WS2812FX ws0 = WS2812FX(LED0_COUNT, LED0_PIN, NEO_GRB + NEO_KHZ800);
WS2812FX ws1 = WS2812FX(LED1_COUNT, LED1_PIN, NEO_RGB + NEO_KHZ800);
WS2812FX ws2 = WS2812FX(LED2_COUNT, LED2_PIN, NEO_RGB + NEO_KHZ800);
WS2812FX ws3 = WS2812FX(LED3_COUNT, LED3_PIN, NEO_GRB + NEO_KHZ800);

//Serial stuff
bool bSerialStringComplete = false;
String strSerialData;

void serialEvent();
bool DoAction(JsonObject& ActionObj, String &strErrorMessage);
void HandleSerialInput();

void setup() {
   Serial.begin(115200);
   Serial.println("Starting!");
   ws0.init();
   ws0.setBrightness(64);
   ws0.setSpeed(200);
   ws0.setMode(2);
   ws0.start();

   ws1.init();
   ws1.setBrightness(64);
   ws1.setSpeed(200);
   ws1.setMode(2);
   ws1.start();

   ws2.init();
   ws2.setBrightness(64);
   ws2.setSpeed(200);
   ws2.setMode(2);
   ws2.start();

   ws3.init();
   ws3.setBrightness(255);
   ws3.setSpeed(2000);
   ws3.setMode(14);
   ws3.setColor(0xFF0000);
   ws3.start();
}

void loop() {
   ws0.service();
   ws1.service();
   ws2.service();
   ws3.service();
   serialEvent();
   HandleSerialInput();
}

void HandleSerialInput() {
   if (bSerialStringComplete) {
      //try to get json data
      //const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3);
      DynamicJsonBuffer jsonBuffer(256);
      Serial.printf("Received: %s\r\n", strSerialData.c_str());
      JsonObject& input = jsonBuffer.parse(strSerialData);
      if (input.success()) {
         DynamicJsonBuffer outputBuffer(128);
         JsonObject& output = outputBuffer.createObject();
         if (input.containsKey("action")) {
            String strErrorMessage;
            if (DoAction(input, strErrorMessage)) {
               output["success"] = true;
            }
            else {
               output["success"] = false;
               output["errorMessage"] = strErrorMessage.c_str();
            }
         }

         output.printTo(Serial);
      }

      strSerialData = "";
      bSerialStringComplete = false;
   }
}

bool DoAction(JsonObject& ActionObj, String &strErrorMessage) {
   bool bSuccess = true;

   if (ActionObj["action"] == "controlLedStrip") {
      WS2812FX * wsStrip;
      JsonObject& data = ActionObj["actionData"];
      if(!data.containsKey("stripNum")){
         bSuccess = false;
         strErrorMessage = "Action did not specify which strip number we should change!";
         return bSuccess;
      } else {
         uint8_t iStripNum = (uint8_t)data["stripNum"];
         switch(iStripNum){
            case 0:
               wsStrip = &ws0;
               break;
            case 1:
               wsStrip = &ws1;
               break;
            case 2:
               wsStrip = &ws2;
               break;
            default:
               strErrorMessage = "Invalid strip number specified!";
               bSuccess = false;
               return bSuccess;
         }
      }
      if(data.containsKey("animation"))
         wsStrip->setMode((int)data["animation"]);
      if (data.containsKey("speed"))
         wsStrip->setSpeed((int)data["speed"]);
      if (data.containsKey("brightness"))
         wsStrip->setBrightness((int)data["brightness"]);
      if (data.containsKey("color")) {
         String color;
         data["color"].printTo(color);
         uint32_t c = (uint32_t)strtol(color.substring(1, color.length() - 1).c_str(), NULL, 16);
         wsStrip->setColor(c);
         Serial.printf("Color: %i (%s)\r\n", c, color.c_str());
      }

         Serial.printf("Setting leds to: A: %i, S: %i, B: %i\r\n",
            (int)data["animation"],
            (int)data["speed"],
            (int)data["brightness"]);
   }
   else {
      bSuccess = false;
      strErrorMessage = "Invalid action received...";
   }

   return bSuccess;
}

void serialEvent() {
   //Listen on serial port

   while (Serial.available() > 0)
   {
      char cInChar = Serial.read(); // Read a character
                                    //Check if buffer contains complete serial message, terminated by newline (\n)
      if (cInChar == '\n')
      {
         //Serial message in buffer is complete, null terminate it and store it for further handling
         bSerialStringComplete = true;
         strSerialData += '\0'; // Null terminate the string
         break;
      }
      strSerialData += cInChar; // Store it
   }
}