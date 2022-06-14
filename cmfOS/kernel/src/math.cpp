#include "math.h"

double power(double base, int exponent) {
    if (exponent == 0) return 1; 
    if (exponent < 0) return (1/power(base, exponent*-1));

    double acc = base;
    for (int i = 0; i < exponent; i++) acc *= base;
    return acc;
}