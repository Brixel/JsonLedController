#include "LedHandler.h"

#define LED0_COUNT 314
#define LED0_PIN D5

#define LED1_COUNT 10
#define LED1_PIN D6

#define LED2_COUNT 2
#define LED2_PIN D7

#define LED3_COUNT 12
#define LED3_PIN D8

void LedHandler::Init()
{
    _ws0 = new WS2812FX(LED0_COUNT, LED0_PIN, NEO_GRB + NEO_KHZ800);
    _ws1 = new WS2812FX(LED1_COUNT, LED1_PIN, NEO_RGB + NEO_KHZ800);
    _ws2 = new WS2812FX(LED2_COUNT, LED2_PIN, NEO_RGB + NEO_KHZ800);
    _ws3 = new WS2812FX(LED3_COUNT, LED3_PIN, NEO_GRB + NEO_KHZ800);

    _ws0->init();
    _ws0->setBrightness(255);
    _ws0->setSpeed(200);
    _ws0->setMode(2);
    _ws0->start();

    _ws1->init();
    _ws1->setBrightness(255);
    _ws1->setSpeed(200);
    _ws1->setMode(2);
    _ws1->start();

    _ws2->init();
    _ws2->setBrightness(255);
    _ws2->setSpeed(200);
    _ws2->setMode(2);
    _ws2->start();

    _ws3->init();
    _ws3->setBrightness(255);
    _ws3->setSpeed(2000);
    _ws3->setMode(14);
    _ws3->setColor(0xFF0000);
    _ws3->start();
}

void LedHandler::Loop()
{
    _ws0->service();
    _ws1->service();
    _ws2->service();
    _ws3->service();
}

bool LedHandler::DoAction(JsonObject &ActionObj, String &strErrorMessage)
{
    bool bSuccess = true;

    if (ActionObj["action"] == "controlLedStrip")
    {
        WS2812FX *wsStrip;
        JsonObject data = ActionObj["actionData"];
        if (!data.containsKey("stripNum"))
        {
            bSuccess = false;
            strErrorMessage = "Action did not specify which strip number we should change!";
            return bSuccess;
        }
        else
        {
            uint8_t iStripNum = (uint8_t)data["stripNum"];
            switch (iStripNum)
            {
            case 0:
                wsStrip = _ws0;
                break;
            case 1:
                wsStrip = _ws1;
                break;
            case 2:
                wsStrip = _ws2;
                break;
            default:
                strErrorMessage = "Invalid strip number specified!";
                bSuccess = false;
                return bSuccess;
            }
        }
        if (data.containsKey("animation"))
            wsStrip->setMode((int)data["animation"]);
        if (data.containsKey("speed"))
            wsStrip->setSpeed((int)data["speed"]);
        if (data.containsKey("brightness"))
            wsStrip->setBrightness((int)data["brightness"]);
        if (data.containsKey("color"))
        {
            String color = data["color"];
            uint32_t c = (uint32_t)strtol(color.substring(1, color.length() - 1).c_str(), NULL, 16);
            wsStrip->setColor(c);
            Serial.printf("Color: %i (%s)\r\n", c, color.c_str());
        }

        Serial.printf("Setting leds to: A: %i, S: %i, B: %i\r\n",
                      (int)data["animation"],
                      (int)data["speed"],
                      (int)data["brightness"]);
    }
    else
    {
        bSuccess = false;
        strErrorMessage = "Invalid action received...";
    }

    return bSuccess;
}