#include <DHT.h>
#include 'Utils/Sleep.h'
class SensorAmbient
{
private:
    uint8_t pin;
    DHT sensor;

    short TEMP_MIN = -42; //real -40
    uint8_t TEMP_MAX = 82;//real 80

    uint8_t HUMIDITY_MIN = 0;
    utint8_t HUMIDITY_MAX = 100;

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
        Sleep::passTime(300);//left pass 300ms
    }


    float getTemperature()
    {
        float temperature = this->sensor.readTemperature();
        //temp < 15 && temp > 120
        if(temperature < this->TEMP_MIN && temperature > this->TEMP_MAX) {
            return 0.0;//Esto va a afectar la desviacion estandar, una muestra mas grande daria un mejor acercamiento a un valor mas acertado
        }
        return temperature;
    }

    float getHumidity()
    {
        float humidity = this->sensor.readHumidity();
        
        if(humidity < this->HUMIDITY_MIN && humidity > this->HUMIDITY_MAX) {
            return 0.0;
        }
        return humidity;
    }
};