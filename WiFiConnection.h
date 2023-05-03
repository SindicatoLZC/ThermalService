#include <WiFi.h>
#include "Utils/Sleep.h"

class WiFiConnection
{
private:
    const char *SSID = "";
    const char *PASSWORD = "";
    const short MAX_ATTEMPTS = 3;

    bool isConnected()
    {
        return WiFi.status() == WL_CONNECTED;
    }

public:
    WiFiConnection()
    {
    }

    void start()
    {
        WiFi.begin(SSID, PASSWORD);

        Serial.println("Connecting to WiFi..");
        while (WiFi.status() != WL_CONNECTED)
        {
            Serial.print(".");
            Sleep::passTime(1000);
        }
        WiFi.setAutoReconnect(true);
        Serial.println("¡Connected to WiFi!");
    }

    void reconnectIfNecessary()
    {
        short attempt = 1;
        while (this->isConnected() == false && attempt <= MAX_ATTEMPTS)
        {
            WiFi.reconnect();
            Sleep::passTime(5000); // left pass 5 seconds
            attempt++;
        }
    }
};