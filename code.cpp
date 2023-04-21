#define  MAX_TEMP  37
class Task {
  unsigned long _interval;     // interval between task executions
  unsigned long _lastRunTime;  // last time the task was executed
  void (*_function)();         // function pointer to the task

public:
  Task(unsigned long interval, void (*function)()) {
    _interval = interval;
    _lastRunTime = 0;
    _function = function;
  }

  void run() {
    unsigned long currentTime = millis();
    if (currentTime - _lastRunTime >= _interval) {
      _lastRunTime = currentTime;
      _function();
    }
  }
};
class Relay {
private:
  short pin;
public:
  bool on() {
    return true;
  }
  bool off() {
    return true;
  }
  bool getStatus() {
    return true;
  }
};
class SensorTemperature {
private:
  short pin;
  float value;
public:
  SensorTemperature() {}
  short read() {
    return (short)random(15, 40);
  }
  void start() {
    Serial.println("Sensor iniciado");
  }
};

class AnalyzerTemperature {
private:
  SensorTemperature sensors[4];
  short temperatures[10][4];
  short readingCount = 0;
public:
  int calculate() {
    for (short i = 0; i < 10; i++) {
      for (short j = 0; j < 4; j++) {
        Serial.print(temperatures[i][j]);
        Serial.print("c\t");
      }
      Serial.println();
    }
    Serial.println("\n\n");
    return random(14, 40);
  }
  void initSensors() {
    for (short i = 0; i < 4; i++) {
      sensors[i].start();
    }
  }

  void reading() {
    if (this->readingCount > 9) {
      this->readingCount = 0;
    }
    for (short col = 0; col < 4; col++) {
      temperatures[this->readingCount][col] = sensors[col].read();
    }
    this->readingCount++;
  }

  void getLastTemperatures() {
    Serial.println("Ultimas temperaturas jejeje");
  }
};

AnalyzerTemperature analyzer;
Relay relay;

Task readSensors(1 * 1000L, []() {
  analyzer.reading();
});

Task activeRelay(1 * 1000L * 60L, []() {
  if (analyzer.calculate() > MAX_TEMP) {
    relay.on();
    Serial.println("RELAY ACTIVADO");
  } else {
    relay.off();
    Serial.println("RELÉ DESACTIVADO!");
  }
});

Task sendDataToWebServer(5 * 1000L * 60L, []() {
  Serial.print("Estado del relé: ");
  Serial.println(relay.getStatus());
  analyzer.getLastTemperatures();
});
void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  analyzer.initSensors();
}

void loop() {
  readSensors.run();          //Leer cada seg
  activeRelay.run();          //Encender o apagar el rele cada 15min segun la temperatura
  sendDataToWebServer.run();  //Enviar el ultimo valor de los sensores y estado del rele cada 5 min
  // put your main code here, to run repeatedly:
}
