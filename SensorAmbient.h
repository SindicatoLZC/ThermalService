#include <DHT.h>
#include "Sleep.h"
#include <stdint.h>


class SensorAmbient : public DHT{
private:
  uint8_t pin;
  short TEMP_MIN = -42;   //real -40
  uint8_t TEMP_MAX = 82;  //real 80

  uint8_t HUMIDITY_MIN = 0;
  uint8_t HUMIDITY_MAX = 100;


public:

  SensorAmbient(uint8_t pin, uint8_t type, uint8_t count = 6) : DHT(pin, type, count) {
  }


  void start() {
    this->begin();
    Sleep::passTime(300);  //left pass 300ms
  }


  float getTemperature() {
    float temperature = this->readTemperature();
    //temp < 15 && temp > 120
    if (temperature < this->TEMP_MIN || temperature > this->TEMP_MAX) {
      return 0.0;  //Esto va a afectar la desviacion estandar, una muestra mas grande daria un mejor acercamiento a un valor mas acertado
    }
    return temperature;
  }

  float getHumidity() {
    float humidity = this->readHumidity();

    if (humidity < this->HUMIDITY_MIN || humidity > this->HUMIDITY_MAX) {
      return 0.0;
    }
    return humidity;
  }
};