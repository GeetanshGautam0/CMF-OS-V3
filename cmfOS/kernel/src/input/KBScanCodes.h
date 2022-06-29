#pragma once
#include "../std/ctypedef.h"


namespace QWERTY {

    #define LeftShift       0x2A
    #define RightShift      0x36
    #define Enter           0x1C
    #define Backspace       0x0E
    #define Spacebar        0x39
    #define Tab             0x0f

    #define ReleaseOffset   0x80

    extern const char ASCIITable[];
    char Traslate(uint8_t scancode, bool uppercase);

}
