#pragma once
#include "std.hpp"
#include "math.hpp"


const char* to_string(uint64_t value);
const char* to_string(int64_t value);

const char* to_string(float value, int decPlaces);
const char* to_string(float value);

const char* to_string(double value, int decPlaces);
const char* to_string(double value);

const char* to_string(bool value);
const char* to_string(Math::Point coordinate);

const char* to_hstring(uint64_t value);
const char* to_hstring(uint32_t value);
const char* to_hstring(uint16_t value);
const char* to_hstring(uint8_t value);