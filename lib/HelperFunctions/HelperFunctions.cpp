#include "HelperFunctions.h"

void HelperFunctions::sendPutRequest(String address, String value)
{
    HTTPClient client;
    client.begin(address);
    client.addHeader("Content-Type", "text/plain");

    int resCode = client.PUT(value);

    if (resCode <= 0)
    {
        Serial.println(
            String("Could send PUT to address '") + address + String("'. The following error occured: '") +
            String(resCode) +
            String("' Please refer to the following address to get further information: ") +
            String("https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266HTTPClient/src/ESP8266HTTPClient.h"));
    }
    else if (resCode < 200 || resCode >= 300)
    {
        Serial.println(String("Server returned an error status code sending to '") + address + String("': '") + String(resCode) + String("'!"));
    }
}

String HelperFunctions::stripLastSlash(String value)
{
    if (value.length() == 0)
        return value;

    while(value.charAt(value.length() -1) == '/')
    {
        value.remove(value.length() -1, 1);
    }

    return value;
}

String HelperFunctions::stripFirstSlash(String value)
{
    if (value.length() == 0)
        return value;

    while(value.charAt(0) == '/')
    {
        value.remove(0, 1);
    }
    
    return value;
}