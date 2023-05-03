class Sleep
{
public:
    static int passTime(unsigned long timems)
    {
        unsigned long totalTime = 0;

        while (totalTime <= timems)
        {
            totalTime += millis();
        }
    }
};