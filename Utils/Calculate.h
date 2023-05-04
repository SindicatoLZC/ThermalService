#include <math.h>
class Calculate
{
private:
    float standardDeviation(float sample[], short length)
    {
        float sum = 0.0, mean, sd = 0.0;
        for (short i = 0; i < length; i++)
        {
            sum += sample[i];
        }

        mean = sum / length;

        for (short i = 0; i < length; i++)
        {
            sd += pow(sample[i] - mean, 2);
        }
        return sqrt(sd / length);
    }

    float media(float sample[], short length)
    {
        short frecuency[length];
        float mode;

        for(short i = 0; i < length; i++) {
            frecuency[ (short) sample[i] ]++;
        }

        short maxFrecuency = 0;
        short indexMode;

        for(short i = 0; i < length; i++) {
            if(frecuency[i] > maxFrecuency) {
                maxFrecuency = frecuency[i];
                indexMode = i;
            }
        }
        return sample[indexMode];
    }

public:
    static float temperature(float sample[15][2])
    {
        float
    }
    static float humidity(float sample[15])
    {
    }
};