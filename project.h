
short RELAY = 1;
short DHT22 = 2;

uint8_t LED_RED;
uint8_t LED_BLUE;
uint8_t LED_GREEN;


WiFiConnection wifi = WiFiConnection();
TemperatureAnalyzer analyzer;

Task taskReadSensors;
Task taskActivateRelayIfNecessary;
Task taskPostData;


void setup() {
    Serial.begin(9600);
    wifi.start();
   
    analyzer = TemperatureAnalyzer();
    analyzer.setPinForAmbientSensor(DHT22);
    analyzer.setPinForRelay(RELAY);
    analyzer.startSensors();

    taskReadSensors(5 * 1000L, []() {
        analyzer.reading();
    }); 

    taskPostData(30 * 1000L, []() {
        analyzer.send();
    });

    taskActivateRelayIfNecessary(15 * 1000L * 60L, []() {
        analyzer.activateRelayIfNecessary();
    });
}


void loop() {
    taskReadSensors.run();
    taskPostData.run();
    taskActivateRelayIfNecessary.run();
}