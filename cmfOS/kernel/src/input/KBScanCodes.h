#pragma once
#include "../std/ctypedef.h"


namespace QWERTY {

    #define LeftShift       0x2A
    #define RightShift      0x36
    #define Enter           0x1C
    #define Backspace       0x0E
    #define Spacebar        0x39
    #define Tab             0x0F
    #define Control         0x1D
    #define Alt             0x38

    #define ReleaseOffset   0x80

    #define UpArrow         0x18
    #define DownArrow       0x19
    #define RightArrow      0x1A
    #define LeftArrow       0x1B

    extern const char ASCIITable[];
    char Traslate(uint8_t scancode, bool uppercase);

}
