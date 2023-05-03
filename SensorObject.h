#include <Wire.h> // Librería para comunicación I2C
#include <Adafruit_MLX90614.h>
#include 'Utils/Sleep.h'

class SensorObject
{
private:
    short pin
        Adafruit_MLX90614 sensor = NULL;
    float ambientTemp;
    float objectTemp;

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

    void read()
    {
        this->ambientTemp = sensor.readAmbientTempC();
        this->objectTemp = sensor.readObjectTempC();
    }

    float getAmbientTemp()
    {
        return this->ambientTemp;
    }

    float getObjectTemp()
    {
        return this->objectTemp;
    }
};