class Task
{
private:
    unsigned long interval;    // interval between task executions
    unsigned long lastRunTime; // last time the task was executed
    void (*function)();        // function pointer to the task

public:
    Task() {

    }
    void defineTask(unsigned long interval, void (*function)())
    {
        this->interval = interval;
        this->lastRunTime = 0;
        this->function = function;
    }

    void run()
    {
        unsigned long currentTime = millis();
        if (currentTime - this->lastRunTime >= this->interval)
        {
            this->lastRunTime = currentTime;
            this->function();
        }
    }
};