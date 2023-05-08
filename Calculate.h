#include <stdint.h>
#include "Operations.h"
class Calculate
{
public:
    static float meanAmbientTemp(float sample[][2], uint8_t row, uint8_t col)
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
        return Operations::mean(samplePlain, row * col);
    }

    static float meanObjectTemp(float sample[], uint8_t length)
    {
        return Operations::mean(sample, length);
    }

    static float meanHumidity(float sample[], uint8_t length)
    {
    }

    static float madAmbientTemp(float sample[][2], uint8_t row, uint8_t col) {}

    static float madObjectTemp(float sample[], uint8_t length) {
      return Operations::meanAbsoluteDeviation(sample, length);

    }

    static float madHumidity(float sample[], uint8_t length)
    {
    }

    static float absoluteDeviationTemperature(float sample[][2], uint8_t row, uint8_t col)
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
        return Operations::meanAbsoluteDeviation(samplePlain, row * col);
    }
};