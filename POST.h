#include <HTTPClient.h>
class POST
{
private:
    const char *WEB_SERVER = "";
    const char *TOKEN = "";
    const short MAX_ATTEMPT = 3;

public:
    static void send(String data)
    {
        short attempt = 1;
        while (attempt <= MAX_ATTEMPT)
        {
            HTTPClient http = NULL;
            http.begin(WEB_SERVER);
            Sleep::passTime(200);//left pass 200 ms
            http.addHeader("Content-Type", "application/json");
            int httpCode = http.POST(data);

            if (httpCode > 0)
            {
                Serial.printf("Código de estado de la respuesta: %d\n", httpCode);
                if (httpCode == HTTP_CODE_OK)
                {
                    String response = http.getString();
                    Serial.println(response);
                    attempt = MAX_ATTEMPTS;
                }
                attempt++;
                continue;
            }
            else
            {
                Serial.println("Error en la solicitud. Intentando de nuevo..");
                attempt++;
            }
        }
    }
};