#include "WiFiConnection.h"

short RELAY = 1;

short DHT22_1 = 2;
short DHT22_1 = 3;


WiFiConnection wifi = WiFiConnection();
Relay relay = Relay(RELAY);
TemperatureAnalyzer analyzer;

Task taskReadSensors;
Task taskActivateRelayIfNecessary;
Task taskPostData;



void setup() {
    Serial.begin(9600);
    wifi.start();

   
    analyzer = Analyzer(DHT22_1);

    analyzer.start();

    taskReadSensors(5 * 1000L, []() {
        analyzer.reading();
    }); 

    taskPostData(30 * 1000L, []() {
        analyzer.send();
    });

}


void loop() {
    taskReadSensors.run();
    taskPostData.run();
    taskActivateRelayIfNecessary.run();

}