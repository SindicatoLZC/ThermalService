#include "Task.h"
#include <stdint.h>
#include "WiFiConnection.h"
#include "TemperatureAnalyzer.h"

uint8_t PIN_RELAY = 1;
uint8_t PIN_DHT22 = 2;



WiFiConnection wifi = WiFiConnection();
TemperatureAnalyzer analyzer;

Task taskReadSensors;
Task taskActivateRelayIfNecessary;
Task taskPostData;

void setup() {

  Serial.begin(9600);
  wifi.start();

  analyzer = TemperatureAnalyzer();

  analyzer.setPinForRelay(PIN_RELAY);
  analyzer.setPinForAmbientSensor(PIN_DHT22);
  analyzer.startSensors();

  taskReadSensors.defineTask(30 * 1000L, []() {
    analyzer.reading();
  });

  taskPostData.defineTask(30 * 1000L, []() {
    analyzer.send();
  });

  taskActivateRelayIfNecessary.defineTask(1 * 1000L * 60L, []() {
    analyzer.activateRelayIfNecessary();
  });
}


void loop() {
  taskReadSensors.run();
  taskPostData.run();
  taskActivateRelayIfNecessary.run();
}