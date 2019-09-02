#include "SerialHandler.h"

void SerialHandler::_SerialEvent()
{
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
void SerialHandler::_HandleSerialInput()
{
    if (bSerialStringComplete)
    {
        //try to get json data
        //const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3);
        Serial.printf("Received: %s\r\n", strSerialData.c_str());
        StaticJsonDocument<256> inputDoc;

        DeserializationError error = deserializeJson(inputDoc, strSerialData);
        JsonObject input = inputDoc.as<JsonObject>();
        // Test if parsing succeeds.
        if (!error)
        {
            StaticJsonDocument<128> output;
            if (input.containsKey("action"))
            {
                String strErrorMessage;
                if (ledHandler.DoAction(input, strErrorMessage))
                {
                    output["success"] = true;
                }
                else
                {
                    output["success"] = false;
                    output["errorMessage"] = strErrorMessage.c_str();
                }
            }
            serializeJsonPretty(output, Serial);
        }

        strSerialData = "";
        bSerialStringComplete = false;
    }
}
void SerialHandler::Loop()
{
    _SerialEvent();
    _HandleSerialInput();
}