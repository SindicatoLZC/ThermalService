#include <math.h>
#include <stdint.h>
class Calculate
{
private:
    float standardDeviation(float sample[], uint8_t length)
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

    float mode(float sample[], short length)
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

    float meanAbsoluteDeviation(float sample[], uint8_t length) {
        float media, mad;
        
        for(uint8_t i = 0; i < length; i++) {
            media += sample[i];
        }

        media = media / length;

        for(uint8_t i = 0; i < length; i++) {
            mad += fabs(sample[i] - media);
        }

        mad = mad / length;

        return mad;
    }

public:
    static float temperature(float sample[], uint8_t row, uint8_t col)
    {
        float samplePlain[row * col];
        uint8_t k = 0;

        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                samplePlain[k] = sample[i][j];
                k++;
            }
        }
        return this->mode(samplePlain, row * col);
    }

    static float humidity(float sample[15])
    {

    }
};