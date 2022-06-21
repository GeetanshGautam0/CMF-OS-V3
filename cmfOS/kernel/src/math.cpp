#include "math.h"

double power(double base, int exponent) {
    if (exponent == 0) return 1; 
    if (exponent < 0) return (1/power(base, exponent*-1));

    double acc = base;
    for (int i = 0; i < exponent; i++) acc *= base;
    return acc;
}

int floor(double n) { return (int)n; }

int ciel(double n) { return (int)n == n ? (int)n : (int)n + 1; }

double clamp(double min, double actual, double max) { 
    return min <= actual && actual <= max ? actual : (min > actual ? min : max);
}

