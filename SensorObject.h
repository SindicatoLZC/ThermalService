#include <Wire.h> // Librería para comunicación I2C
#include <Adafruit_MLX90614.h>
#include 'Utils/Sleep.h'

class SensorObject
{
private:
    uint8_t pin;
    Adafruit_MLX90614 sensor = NULL;
    short TEMPERATURE_MIN = -40;
    short TEMPERATURE_MAX = 125;

public:
    SensorObject()
    {
        this->sensor = Adafruit_MLX90614();
    }

    void start()
    {
        this->sensor.begin();
        Sleep::passTime(200); // Left pass 200 ms
    }

    float getAmbientTemp()
    {
        return this->sensor.readAmbientTempC();
    }

    float getObjectTemp()
    {
        return this->sensor.readObjectTempC();
    }
};