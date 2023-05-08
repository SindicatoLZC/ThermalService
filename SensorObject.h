#include <Wire.h> // Librería para comunicación I2C
#include <Adafruit_MLX90614.h>
#include "Sleep.h"
#include <stdint.h>

class SensorObject
{
private:
    uint8_t pin;
    Adafruit_MLX90614 sensor;
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
        float temperature = this->sensor.readAmbientTempC();
        if(temperature < this->TEMPERATURE_MIN || temperature > this->TEMPERATURE_MAX) {
            return 0.0;
        }
        return temperature;
    }

    float getObjectTemp()
    {
        float temperature = this->sensor.readObjectTempC();
        if(isinf(temperature) || isnan(temperature) || temperature < this->TEMPERATURE_MIN || temperature > this->TEMPERATURE_MAX) {
            return 0.0;
        }
        return temperature;
    }
};