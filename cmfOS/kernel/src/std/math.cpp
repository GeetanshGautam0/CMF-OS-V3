#include "math.h"

namespace CMath {

    double power(double base, int exponent) {
        if (exponent == 0) return 1;
        if (exponent < 0) return (1 / power(base, exponent*-1));

        double acc = base;
        for (int i = 0; i < exponent; i++) acc *= base;
        return acc;
    }

    int floor(double n) { return (int)n; }

    int ciel(double n) { return (int)n == n ? (int)n : (int)n + 1; }

    double clamp(double min, double actual, double max) {
        return min <= actual && actual <= max ? actual : (min > actual ? min : max);
    }

    int clamp(int min, int actual, int max) {
        return min <= actual && actual <= max ? actual : (min > actual ? min : max);
    }

    unsigned int clamp(unsigned int min, unsigned int actual, unsigned int max) {
        return min <= actual && actual <= max ? actual : (min > actual ? min : max);
    }

    bool operator !=(Point a, Point b) {
        return (a.x != b.x) || (a.y != b.y);
    }

    bool operator ==(Point a, Point b) {
        return (a.x == b.x) && (a.y == b.y);
    }

}
