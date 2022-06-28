#include "KB.h"


void HandleKeyboard(uint8_t scancode) {
    char ascii = QWERTY::Traslate(scancode, false);
    if (ascii != 0) {
        KernelRenderer.printChar(ascii, BLACK, {false, MAGENTA});
    }
}
