#include "KB.h"


bool LShiftPressed;
bool RShiftPressed;


void HandleKeyboard(uint8_t scancode) {

    switch (scancode) {
        case LeftShift: {
            LShiftPressed = true;
            return;
        }
        case (LeftShift + ReleaseOffset): {
            LShiftPressed = false;
            return;
        }
        case RightShift: {
            RShiftPressed = true;
            return;
        }
        case (RightShift + ReleaseOffset): {
            RShiftPressed = false;
            return;
        }
        case Enter: {
            KernelRenderer.printString("\n\r", BLACK, {false, BLACK});
            return;
        }
        case Spacebar: {
            KernelRenderer.printChar(' ', BLACK, {false, WHITE});
            return;
        }
        case Tab: {
            KernelRenderer.printChar('\t', BLACK, {false, WHITE});
            return;
        }
        case Backspace: {
            KernelRenderer.deletePrevious();
            return;
        }
        default : {
            char ascii = QWERTY::Traslate(scancode, LShiftPressed || RShiftPressed);
            if (ascii != 0) {
                KernelRenderer.printChar(ascii, BLACK, {true, WHITE});
            }
            break;
        }
    }
}
