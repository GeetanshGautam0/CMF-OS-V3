#pragma once
#include "../std/ctypedef.h"

namespace QWERTY {

    extern const char ASCIITable[];
    char Traslate(uint8_t scancode, bool uppercase);

}
