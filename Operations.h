#include <math.h>
#include <stdint.h>
class Operations
{
public:
    static float mean(float sample[], uint8_t length)
    {
        float sum = 0.0, mean, sd = 0.0;
        for (uint8_t i = 0; i < length; i++)
        {
            sum += sample[i];
        }

        mean = sum / length;
    }

    static float standardDeviation(float sample[], uint8_t length)
    {
        float sum = 0.0, mean, sd = 0.0;
        for (uint8_t i = 0; i < length; i++)
        {
            sum += sample[i];
        }

        mean = sum / length;

        for (uint8_t i = 0; i < length; i++)
        {
            sd += pow(sample[i] - mean, 2);
        }
        return sqrt(sd / length);
    }

    static float mode(float sample[], short length)
    {
        uint8_t frecuency[length];
        float mode;

        for (uint8_t i = 0; i < length; i++)
        {
            frecuency[(uint8_t)sample[i]]++;
        }

        short maxFrecuency = 0;
        short indexMode;

        for (uint8_t i = 0; i < length; i++)
        {
            if (frecuency[i] > maxFrecuency)
            {
                maxFrecuency = frecuency[i];
                indexMode = i;
            }
        }
        return sample[indexMode];
    }

    static float meanAbsoluteDeviation(float sample[], uint8_t length)
    {
        float media, mad;

        for (uint8_t i = 0; i < length; i++)
        {
            media += sample[i];
        }

        media = media / length;

        for (uint8_t i = 0; i < length; i++)
        {
            mad += fabs(sample[i] - media);
            Serial.printf("%f\t", sample[i]);
        }
        Serial.println();
        mad = mad / length;

        return mad;
    }
    
};