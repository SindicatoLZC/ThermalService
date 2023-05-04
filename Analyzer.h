#include "SensorAmbient.h"
#include "SensorObject.h"

class TemperatureAnalyzer
{
private:
    SensorAmbient ambient;
    SensorObject object;

    uint8_t pinSensorAmbient;

    float sampleTemperatures[15][2];
    float sampleHumidity[15];

    Relay relay;

    uint8_t readingTempCount;
    uint8_t readingHumCount;

    uint8_t MAX_TEMPERATURE = 38;

    void fillTemperatureSamples()
    {
        if (this->readingTempCount > 15)
        {
            this->readingTempCount = 0;
        }
        sampleTemperatures[this->readingTempCount][0] = this->ambient.getTemperature();
        sampleTemperatures[this->readingTempCount][1] = this->objet.getAmbientTemp();
    }

    void fillHumiditySamples()
    {
        if (this->readingHumCount > 15)
        {
            this->readingHumCount = 0;
        }
        sampleHumidity[this->readingHumCount] = this->ambient.getHumidity();
        this->readingHumCount++;
    }

public:
    TemperatureAnalyzer()
    {
    }

    void startSensors()
    {
        this->ambient = SensorAmbient(this->pinSensorAmbient);
        this->object = SensorObject();

        this->ambient.start();
        this->object.start();

        this->relay.start();
    }
    void setPinForAmbientSensor(uint8_t pinSensorAmbient)
    {
        this->pinSensorAmbient = pinSensorAmbient;
    }

    void setPinForRelay(uint8_t pinRelay)
    {
        relay = Relay(pinRelay);
    }

    void reading()
    {
        this->ambient.read();
        this->object.read();

        this->fillTemperatureSamples();
        this->fillHumiditySamples();
    }

    void send(uint8_t dht_ambient_temp, uint8_t dht_humidity, uint8_t mlx_ambient_temp, uint8_t mlx_object_temp, String token, bool relay_status,
              uint8_t mlx_last_object_temp, uint8_t mlx_last_ambient_temp, uint8_t dht_last_ambient_temp, uint8_t dht_last_ambient_humidity)
    {
        DynamicJsonDocument json(2048);
        json["token"] = token;
        json["dht_ambient_temp"] = dht_ambient_temp;
        json["dht_ambient_humidity"] = dht_humidity;
        json["mlx_ambient_temp"] = mlx_ambient_temp;
        json["mlx_object_temp"] = mlx_object_temp;
        json["relay_status"] = relay_status;
        json["mlx_last_object_temp"] = mlx_last_object_temp;
        json["mlx_last_ambient_temp"] = mlx_last_ambient_temp;
        json["dht_last_ambient_temp"] = dht_last_ambient_temp;
        json["dht_last_ambient_humidity"] = dht_last_ambient_humidity;

        // Convierte el objeto JSON a una cadena
        String json_string;
        serializeJson(json, json_string);
        POST.send(json_string);
    }

    void activateRelayIfNecessary()
    {
        short ambientTempMode = Calculate::ambientTemperatureMode();
        short ambientHumMode = Calculate::ambientHumidityMode();

        short objectTempMode = Calculate::objectTemperatureMode();

        if (Calculate::modeTemperature() >= MAX_TEMPERATURE)
        {
            this->relay.tryToTurnOn();
        }
        else
        {
            this->relay.tryToTurnOff();
        }
    }

    float getActualObjTemp()
    {
        return this->object.getObjectTemp();
    }
    float getActualAmbientTemp()
    {
        return this->ambient.getTemperature();
    }
};