#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const int MAX_TEMP = 37;
const char* TOKEN_APP = "kaltt0gx9=iDcmU=d7IR-o62F/piC/OJ4i9gH72fgn2A9HthB-vc/O0Zt?gQYI0m01w=9vBUx6qtCw!JbBQCR9!ORj?jS?7PeknKDHoi3fs!uC!1EumLO8QFiusKRgUFeCAiBoZ/nQdPh=Sn7QdxG=jnMvDKuDiUWG4aCWx69!5k9!UM6qxpQwj0kmH7qkZ-7/bXj?yIUcCkRDGEpc/jjndn951V7riYZgiod/WUm7smpZk/H9YFjPDBQjPatreq";

const char* SSID = "INFINITUM9430_2.4";
const char* PASSWORD = "aFKWKKE93C";


//const char* WEB_SERVER = "https://sindicatolzc.com/php/test_post.php";
const char* WEB_SERVER = "https://estedominionoexiste.com/php/test_post.php";

const char* WEB_SERVER_PORT = "";

const short MAX_ATTEMPTS = 3;

class Task {
private:
  unsigned long interval;     // interval between task executions
  unsigned long lastRunTime;  // last time the task was executed
  void (*function)();         // function pointer to the task

public:
  Task(unsigned long interval, void (*function)()) {
    this->interval = interval;
    this->lastRunTime = 0;
    this->function = function;
  }

  void run() {
    unsigned long currentTime = millis();
    if (currentTime - this->lastRunTime >= this->interval) {
      this->lastRunTime = currentTime;
      this->function();
    }
  }
};
class Relay {
private:
  short pin;
  void on() {
    digitalWrite(this->pin, HIGH);
    delay(300);
  }
  void off() {
    digitalWrite(this->pin, LOW);
    delay(300);
  }
public:

  Relay(short pin) {
    this->pin = pin;
    pinMode(this->pin, OUTPUT);
  }

  bool isOn() {
    return digitalRead(this->pin) == HIGH ? true: false;
  }

  void tryToTurnOn() {
    Serial.printf("Intentando encender el relay\n");
    short attempt = 0;

    while (attempt < MAX_ATTEMPTS) {
      this->on();

      if (this->isOn() == false) {
        attempt++;
        continue;
      } else {
        attempt = MAX_ATTEMPTS;
        Serial.println("Se activo el relay...");
      }
    }
  }
  void tryToTurnOff() {
    Serial.printf("Intentando apagar el relay\n");
    short attempt = 0;
    while (attempt < MAX_ATTEMPTS) {
      this->off();
      Serial.println(this->isOn());
      if (this->isOn() == true) {
        attempt++;
        continue;
      } else {
        attempt = MAX_ATTEMPTS;
        Serial.println("Se desactivo el relay...");
      }
    }
  }
};

/*Esta clase inicia el sensor y lee la informacion del sensor, hay que definir el PIN que se estará usando 
Si vas a usar el sensor infrarojo habra que cambiar la clase.*/
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


/*Esta clase llama los metodos de la clase sensor para obtener los valores,
Realiza un muestreo de lecturas del sensor y calcula varios metodos estadisticos paera obtener un valor mas exacto

Ademas intenta enviar por HTTP POST un json con los valores del TOKEN, la temperatura actual y la calculada, asi como el estado del rele
*/
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
    digitalWrite(LED_BUILTIN, HIGH);

    if (this->readingCount > 9) {
      this->readingCount = 0;
    }
    for (short col = 0; col < 4; col++) {
      temperatures[this->readingCount][col] = sensors[col].read();
    }
    this->readingCount++;
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }

  int getLastTemperatures() {
    Serial.println("Ultimas temperaturas jejeje");
    return sensors[0].read();
  }
  void tryToSendPostData(bool releStatus, int actual_temperature, int temp_calculate) {
    //Serial.println("Status: %d\tActual temp: %dc\tCalculate temp:%dc", releStatus, actual_temperature, temp_calculate);


    DynamicJsonDocument json(1024);
    json["token"] = TOKEN_APP;
    json["sensor"] = "Este es el nombre del sensor!";
    json["actual_temperature"] = actual_temperature;
    json["calculate_temperature"] = temp_calculate;
    json["relay_status"] = releStatus;

    // Convierte el objeto JSON a una cadena
    String json_string;
    serializeJson(json, json_string);

    short attempt = 0;

    while (attempt < MAX_ATTEMPTS) {
      Serial.printf("Intento #%d...\n", attempt);
      // Establece la URL del servidor al que se enviará la solicitud
      HTTPClient http;
      http.begin(WEB_SERVER);
      // Establece la cabecera Content-Type como application/json
      http.addHeader("Content-Type", "application/json");
      // Envía la solicitud POST con la cadena JSON como datos
      int httpCode = http.POST(json_string);
      // Verifica el código de estado de la respuesta
      if (httpCode > 0) {
        Serial.printf("Código de estado de la respuesta: %d\n", httpCode);
        if (httpCode == HTTP_CODE_OK) {
          String response = http.getString();
          Serial.println(response);
          attempt = MAX_ATTEMPTS;
        }
        attempt++;
        continue;
      } else {
        Serial.println("Error en la solicitud. Intentando de nuevo..");
        attempt++;
      }
    }
  }
};




AnalyzerTemperature analyzer;
Relay relay(23);

/*
* Read Temperature from sensors
* Every 30 seconds
*/

Task readSensors(5 * 1000L, []() {
  analyzer.reading();
  Serial.println("Leyendo temperaturas");
});

/*
* Check the temperature and try to turn on the RELE
* Every 15 minutes
*/
Task activeRelay(30 * 1000L, []() {
  if (analyzer.calculate() >= MAX_TEMP) {
    relay.tryToTurnOn();  //Try to On relay, do three attempts
  } else {
    relay.tryToTurnOff();
  }
});

/*
* Try to send data to web server via POST
* Every 5 minutes
*/
Task sendDataToWebServer(40 * 1000L, []() {
  Serial.println("Intentando enviar datos POST...");
  int actual_temp = analyzer.getLastTemperatures();
  int calc_temp = analyzer.calculate();
  bool relay_status = relay.isOn();
  analyzer.tryToSendPostData(relay_status, actual_temp, calc_temp);
});


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  WiFi.begin(SSID, PASSWORD);

  Serial.println("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("¡Connected to WiFi!");
  analyzer.initSensors();

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  readSensors.run();          //Leer cada seg
  activeRelay.run();          //Encender o apagar el rele cada 15min segun la temperatura
  sendDataToWebServer.run();  //Enviar el ultimo valor de los sensores y estado del rele cada 5 min
  // put your main code here, to run repeatedly:
}
