
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "LedControl.h"

class POST {
public:
  static String rawToJSON(float meanAmbientTemp,
                          float meanObjectTemp,
                          float meanHumidity,
                          float madAmbientTemp,
                          float madObjectTemp,
                          float madHumidity,
                          float dhtAmbientTemp,
                          float dhtHumidity,
                          float mlxAmbientTemp,
                          float mlxObjectTemp,
                          bool relayStatus) {
    DynamicJsonDocument json(2048);
    json["token"] = "TOKEN";

    json["meanAmbientTemp"] = meanAmbientTemp;
    json["meanObjectTemp"] = meanObjectTemp;
    json["meanHumidity"] = meanHumidity;

    json["madAmbientTemp"] = madAmbientTemp;
    json["madObjectTemp"] = madObjectTemp;
    json["madHumidity"] = madHumidity;

    json["dhtAmbientTemp"] = dhtAmbientTemp;
    json["dhtHumidity"] = dhtHumidity;
    json["mlxAmbientTemp"] = mlxAmbientTemp;
    json["mlxObjectTemp"] = mlxObjectTemp;

    json["relayStatus"] = relayStatus;

    String json_string;
    serializeJson(json, json_string);
    return json_string;
  }
  static void send(String data) {
    Serial.println(data);
    short attempt = 1;
    while (attempt <= 3) {
      HTTPClient http;
      http.begin("WEB_SERVER");
      Sleep::passTime(200);  // left pass 200 ms
      http.addHeader("Content-Type", "application/json");
      int httpCode = http.POST(data);

      if (httpCode > 0) {
        Serial.printf("Código de estado de la respuesta: %d\n", httpCode);
        if (httpCode == HTTP_CODE_OK) {
          String response = http.getString();
          Serial.println(response);
          attempt = 3;
          LedControl::blink(LedControl::PIN_LED_GREEN);
        }
        attempt++;
        ///LedControl::blinkError();
        continue;
      } else {
        Serial.println("Error en la solicitud. Intentando de nuevo..");
        LedControl::parpadearLed(LedControl::PIN_LED_RED, 200, 6);
        attempt++;
      }
    }
  }
};