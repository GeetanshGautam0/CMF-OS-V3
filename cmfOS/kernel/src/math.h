#pragma once
#include "ctypedef.h"

struct Point {
    unsigned int x; 
    unsigned int y; 
};

double power(double base, int exponent);
int floor(double n);
int ciel(double n);
int clamp(int min, int actual, int max);
unsigned int clamp(unsigned int min, unsigned int actual, unsigned int max);
double clamp(double min, double actual, double max);
