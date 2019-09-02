#ifndef _LEDHANDLER_h
#define _LEDHANDLER_h

#include <WS2812FX.h>
#include <ArduinoJson.h>

class LedHandler
{
private:
    WS2812FX *_ws0;
    WS2812FX *_ws1;
    WS2812FX *_ws2;
    WS2812FX *_ws3;

protected:
public:
    bool DoAction(JsonObject &ActionObj, String &strErrorMessage);
    void Init();
    void Loop();
};

extern LedHandler ledHandler;

#endif