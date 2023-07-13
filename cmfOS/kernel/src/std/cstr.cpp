#include "cstr.h"


char __UIntToStringOutput[128];
const char* to_string(uint64_t value) {
    uint8_t size;
    uint64_t sizeTest = value;
    while (sizeTest / 10 > 0) {
        sizeTest /= 10;
        size ++;
    }

    uint8_t index = 0;
    while (value / 10 > 0) {
        uint8_t remainder = value % 10; 
        value /= 10;
        __UIntToStringOutput[size - index] = remainder + '0';
        ++index;
    }
    uint8_t remainder = value % 10;
    __UIntToStringOutput[size - index] = remainder + '0';
    __UIntToStringOutput[size + 1] = 0;
    return __UIntToStringOutput;
}

char __IntToStringOutput[128];
const char* to_string(int64_t value) {
    uint8_t isNegative = value < 0 ? true : false;
    if (isNegative) {
        value *= -1;
        __IntToStringOutput[0] = '-';
    }

    uint8_t size;
    uint64_t sizeTest = value;
    while (sizeTest / 10 > 0) {
        sizeTest /= 10;
        size ++;
    }

    uint8_t index = 0;
    while (value / 10 > 0) {
        uint8_t remainder = value % 10; 
        value /= 10;
        __IntToStringOutput[isNegative + size - index] = remainder + '0';
        ++index;
    }
    uint8_t remainder = value % 10;
    __IntToStringOutput[isNegative + size - index] = remainder + '0';
    __IntToStringOutput[isNegative + size + 1] = 0;
    return __IntToStringOutput;
}

char __DoubleToStringOutput[128];
const char* to_string(double value, int decPlaces) {

    if (decPlaces > 20) decPlaces = 20;

    char* intPtr = (char*)to_string((int64_t)value);
    char* doublePtr = __DoubleToStringOutput;

    if (value < 0) value *= -1;

    while (*intPtr != 0) 
    {
        *doublePtr = *intPtr;
        intPtr++;
        doublePtr++;
    }

    *doublePtr = '.';
    doublePtr++;

    double newVal = value - (int)value;

    for (uint8_t i = 0; i < decPlaces; i++) {
        newVal *= 10;
        *doublePtr = (int)newVal + '0';
        newVal -= (int)newVal;
        doublePtr++;
    }

    *doublePtr = 0;
    return __DoubleToStringOutput;
}


char __FloatToStringOutput[128];
const char* to_string(float value, int decPlaces) {

    if (decPlaces > 20) decPlaces = 20;

    char* intPtr = (char*)to_string((int64_t)value);
    char* floatPtr = __FloatToStringOutput;

    if (value < 0) value *= -1;

    while (*intPtr != 0) 
    {
        *floatPtr = *intPtr;
        intPtr++;
        floatPtr++;
    }

    *floatPtr = '.';
    floatPtr++;

    float newVal = value - (int)value;

    for (uint8_t i = 0; i < decPlaces; i++) {
        newVal *= 10;
        *floatPtr = (int)newVal + '0';
        newVal -= (int)newVal;
        floatPtr++;
    }

    *floatPtr = 0;
    return __FloatToStringOutput;
}

const char* to_string(double value) { return to_string(value, 5); }
const char* to_string(float value)  { return to_string(value, 5); }


const char* to_string(bool value) {return value ? "true" : "false"; }


char hexToStringOutput[128];
const char* to_hstring(uint64_t value) {
    uint64_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 8 * 2 - 1;
    for (uint8_t i = 0; i < size; i++) {
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xf0) >> 4);
        hexToStringOutput[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = (*ptr & 0x0f);
        hexToStringOutput[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }
    hexToStringOutput[size+1] = 0;
    return hexToStringOutput;
}

char hexToStringOutput32[128];
const char* to_hstring(uint32_t value) {
    uint32_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 4 * 2 - 1;
    for (uint8_t i = 0; i < size; i++) {
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xf0) >> 4);
        hexToStringOutput32[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = (*ptr & 0x0f);
        hexToStringOutput32[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }
    hexToStringOutput32[size+1] = 0;
    return hexToStringOutput32;
}

char hexToStringOutput16[128];
const char* to_hstring(uint16_t value) {
    uint16_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 2 * 2 - 1;
    for (uint8_t i = 0; i < size; i++) {
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xf0) >> 4);
        hexToStringOutput16[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = (*ptr & 0x0f);
        hexToStringOutput16[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }
    hexToStringOutput16[size+1] = 0;
    return hexToStringOutput16;
}

char hexToStringOutput8[128];
const char* to_hstring(uint8_t value) {
    uint8_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size =  2 - 1;
    for (uint8_t i = 0; i < size; i++) {
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xf0) >> 4);
        hexToStringOutput8[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = (*ptr & 0x0f);
        hexToStringOutput8[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }
    hexToStringOutput8[size+1] = 0;
    return hexToStringOutput8;
}

char PointToStringOutput[128];
const char* to_string(CMath::Point coordinate) {
    // TODO: implement
    return "Point(<function-undefined>, <function-undefined>)";
}


namespace cstr {

    void _clear_all_buffers() {
        for (int i = 0; i < 128; i++) {
            PointToStringOutput[i] = 0;
            hexToStringOutput8[i] = 0;
            hexToStringOutput16[i] = 0;
            hexToStringOutput32[i] = 0;
            hexToStringOutput[i] = 0;
            __FloatToStringOutput[i] = 0;
            __DoubleToStringOutput[i] = 0;
            __IntToStringOutput[i] = 0;
            __UIntToStringOutput[i] = 0;
        }
    }

}
