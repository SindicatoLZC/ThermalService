#include <DHT.h>
#include 'Utils/Sleep.h'
class SensorAmbient
{
private:
    short pin;
    DHT sensor;
    float temperature;
    float humidity;

    short TEMP_MIN;
    short TEMP_MAX;

public:
    SensorAmbient(short pin)
    {
        this->pin = pin;
        this->sensor = NULL;
    }

    void start()
    {
        this->sensor = DHT(this->pin, DHT22);
        this->sensor.begin();
        Sleep::passTime(300);
    }

    void read()
    {
        this->temperature = this->sensor.readTemperature();
        this->humidity = this->sensor.readHumidity();
    }

    float getTemperature()
    {
        return this->temperature;
    }

    float getHumidity()
    {
        return this->humidity;
    }
};