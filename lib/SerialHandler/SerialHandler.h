#include "LedHandler.h"

class SerialHandler
{
private:
    //Serial stuff
    bool bSerialStringComplete = false;
    String strSerialData;

    void _SerialEvent();
    void _HandleSerialInput();

protected:
public:
    void Loop();
};

extern SerialHandler serialHandler;