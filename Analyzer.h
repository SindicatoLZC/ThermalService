#include "SensorAmbient.h"
#include "SensorObject.h"

class TemperatureAnalyzer {
    private:
        SensorAmbient ambient;
        SensorObject object;
        short pinAmbient;
        short pinObject; 
    public:
        TemperatureAnalyzer(short pinAmbient) {
            this->pinAmbient = pinAmbient;
        }

        void startSensors() {
            this->ambient = SensorAmbient(this->pinAmbient);
            this->object = SensorObject();

            this->ambient.start();
            this->object.start();
        }

        void reading() {
            this->ambient.read();
            this->object.read();
        }
};