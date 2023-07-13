#pragma once

class Time {
    public:
        void IncrementCounter();
        unsigned long GetCounter();

    private:
        unsigned long counter = 0;
        unsigned long counter_ms = 0;

};


extern Time GlobalTimeCounter;
