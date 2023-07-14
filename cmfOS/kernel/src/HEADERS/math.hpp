#pragma once
#include "std.hpp"


namespace Math {

    struct Point {
        unsigned long x;
        unsigned long y;
    };

    bool operator !=(Point a, Point b);
    bool operator ==(Point a, Point b);

    double i_power(double base, int exponent);
    int floor(double n);
    int ciel(double n);

    int clamp(int min, int actual, int max);
    unsigned int clamp(unsigned int min, unsigned int actual, unsigned int max);
    float clamp(float min, float actual, float max);
    double clamp(double min, double actual, double max);

}
