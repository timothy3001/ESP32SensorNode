#ifndef HelperFunctions_h_
#define HelperFunctions_h_

#include <Arduino.h>
#include <HTTPClient.h>

class HelperFunctions
{
    public:
        static void sendPutRequest(String address, String value);
        static void stripLastSlash(String value);
        static void stripFirstSlash(String value);
};

#endif