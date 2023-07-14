#include "../HEADERS/pic_time.hpp"


void Time::IncrementCounter() { 
    counter_ms += 1; 
    if (counter_ms > 1000) {
        counter ++; 
        counter_ms = 0;
    }
}
unsigned long Time::GetCounter() { return counter; }
