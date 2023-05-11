#include <stdint.h>
#include "SensorAmbient.h"
#include "SensorObject.h"
#include "Relay.h"
#include "POST.h"
#include "Calculate.h"
#include "LedControl.h"

class TemperatureAnalyzer {
private:
  SensorAmbient* ambient;
  SensorObject object;

  float sampleAmbientTemperatures[20][2];
  float sampleHumidity[20];
  float sampleObjectTemperatures[20];

  Relay relay;

  uint8_t pinSensorAmbient;
  uint8_t countAmbientTemp;
  uint8_t countHumCount;
  uint8_t countObjectTemp;

  uint8_t MAX_TEMPERATURE = 38;



  void prepareSamples() {
    for (int i = 0; i < 20; i++) {
      this->sampleHumidity[i] = 0.0;
    }
    for (int i = 0; i < 20; i++) {
      this->sampleObjectTemperatures[i] = 0.0;
    }

    for (int i = 0; i < 20; i++) {
      for (int j = 0; j < 2; j++) {
        this->sampleAmbientTemperatures[i][j] = 0.0;
      }
    }
  }

  void fillAmbientTemperatureSamples() {
    if (this->countAmbientTemp > 20) {
      this->countAmbientTemp = 0;
    }
    sampleAmbientTemperatures[this->countAmbientTemp][0] = this->ambient->getTemperature();
    sampleAmbientTemperatures[this->countAmbientTemp][1] = this->object.getAmbientTemp();

    this->countAmbientTemp++;
  }

  void fillHumiditySamples() {
    if (this->countHumCount > 20) {
      this->countHumCount = 0;
    }
    sampleHumidity[this->countHumCount] = this->ambient->getHumidity();
    this->countHumCount++;
  }

  void fillObjectTemperatureSamples() {
    if (this->countObjectTemp > 20) {
      this->countObjectTemp = 0;
    }
    float tem = this->object.getObjectTemp();
    Serial.printf("TEMP object :%f\n", tem);
    sampleObjectTemperatures[this->countObjectTemp] = tem;
  }

public:
  TemperatureAnalyzer() {
    this->prepareSamples();  //set to 0.0 all spaces of arrays

    pinMode(LedControl::PIN_LED_RED, OUTPUT);
    pinMode(LedControl::PIN_LED_BLUE, OUTPUT);
    pinMode(LedControl::PIN_LED_GREEN, OUTPUT);
  }



  void startSensors() {
    this->ambient = new SensorAmbient(this->pinSensorAmbient, 22);
    this->object = SensorObject();

    this->ambient->start();
    this->object.start();

    this->relay.start();
    Serial.printf("Sensores inciados!\n");
  }

  void setPinForRelay(uint8_t pinRelay) {
    this->relay.setPin(pinRelay);
  }

  void setPinForAmbientSensor(uint8_t pin) {
    this->pinSensorAmbient = pin;
  }

  void reading() {
    this->fillAmbientTemperatureSamples();
    this->fillHumiditySamples();
    this->fillObjectTemperatureSamples();
    Serial.printf("Se realizaron las lecturas!\n");
    LedControl::parpadearLed(LedControl::PIN_LED_GREEN, 200, 2);

  }

  void send() {
    float meanAmbientTemp = Calculate::meanAmbientTemp(this->sampleAmbientTemperatures, 20, 2);
    float meanObjectTemp = Calculate::meanObjectTemp(this->sampleObjectTemperatures, 20);
    float meanHumidity = Calculate::meanHumidity(this->sampleHumidity, 20);
    float madAmbientTemp = Calculate::madAmbientTemp(this->sampleAmbientTemperatures, 20, 2);
    float madObjectTemp = Calculate::madObjectTemp(this->sampleObjectTemperatures, 20);
    float madHumidity = Calculate::madHumidity(this->sampleHumidity, 20);

    float dhtAmbientTemp = this->ambient->getTemperature();
    float dhtHumidity = this->ambient->getHumidity();
    float mlxAmbientTemp = this->object.getAmbientTemp();
    float mlxObjectTemp = this->object.getObjectTemp();
    bool relayStatus = this->relay.isOn();

    POST::send(
      POST::rawToJSON(
        meanAmbientTemp,
        meanObjectTemp,
        meanHumidity,
        madAmbientTemp,
        madObjectTemp,
        madHumidity,
        dhtAmbientTemp,
        dhtHumidity,
        mlxAmbientTemp,
        mlxObjectTemp,
        relayStatus));

    Serial.printf("Enviando datos!!\n");
  }

  void activateRelayIfNecessary() {
    float meanObjectTemp = Calculate::meanObjectTemp(this->sampleObjectTemperatures, 20);
    LedControl::parpadearLed(LedControl::PIN_LED_BLUE, 200, 4);

    if (meanObjectTemp >= MAX_TEMPERATURE) {
      this->relay.tryToTurnOn();
      Serial.printf("Se encendio el relay!\n");
      LedControl::turnOn();
    } else {
      this->relay.tryToTurnOff();
      Serial.printf("Se apago el relay!\n");
      LedControl::turnOff();
    }
  }


  float getActualObjTemp() {
    return this->object.getObjectTemp();
  }

  float getActualAmbientTemp() {
    return this->ambient->getTemperature();
  }
};