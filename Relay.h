#include "Utils/Sleep.h";
const short MAX_ATTEMPTS = 3;
class Relay
{
private:
    short pin;
    bool is_on;

    void on()
    {
        digitalWrite(this->pin, HIGH);
        Sleep::passTime(250); // left pass time for 200ms
    }
    void off()
    {
        digitalWrite(this->pin, LOW);
        Sleep::passTime(250);
    }

public:
    Relay(short pin)
    {
        this->pin = pin;
    }
    void start()
    {
        pinMode(this->pin, OUTPUT);
    }
    void tryToTurnOn()
    {
        short attempt = 1;
        while (attempt <= MAX_ATTEMPTS)
        {
            if (this->isOn() == false)
            {
                this->on();
            }
            attempt++;
        }
    }

    void tryToTurnOff()
    {
        short attempt = 1;
        while (attempt <= MAX_ATTEMPTS)
        {
            if (this->isOn())
            {
                this->off();
            }
            attempt++;
        }
    }

    bool isOn()
    {
        return digitalRead(this->pin) == HIGH ? true : false;
    }
};