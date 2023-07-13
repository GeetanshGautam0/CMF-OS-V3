#include "KB.h"

KB_DEBUG_MODE KeyboardDebugMode = KB_DEBUG_MODE::NONE;
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
            // KernelRenderer.print("\n\r", KernelRenderer.GetBackgroundColor() == Color::BLACK ? Color::WHITE : Color ::BLACK);
            return;
        }
        case Spacebar: {
            // KernelRenderer.print(' ', KernelRenderer.GetBackgroundColor() == Color::BLACK ? Color::WHITE : Color ::BLACK);
            return;
        }
        case Tab: {
            // KernelRenderer.print('\t', KernelRenderer.GetBackgroundColor() == Color::BLACK ? Color::WHITE : Color ::BLACK);
            return;
        }
        case Backspace: {
            // KernelRenderer.deletePrevious();
            return;
        }
        case UpArrow: {
            // CMath::Point cursorPos = KernelRenderer.GetCursorPos();
            // Font::FontSize fontSize = KernelRenderer.GetFontSize();
            // unsigned int xCoord = cursorPos.x / fontSize.w;
            // unsigned int yCoord = cursorPos.y / fontSize.h - 1;
            // KernelRenderer.SetCursorPos(xCoord, yCoord);
            return;
        }
        case DownArrow: {

            return;
        }
        case RightArrow: {

            return;
        }
        case LeftArrow: {

            return;
        }
        default : {
            char ascii = QWERTY::Traslate(scancode, LShiftPressed || RShiftPressed);
            
            if (KeyboardDebugMode == KB_DEBUG_MODE::ALL) {
                // KernelRenderer.print(to_hstring(scancode), Color::BLUE, {false, Color::YELLOW});
            }

            if (ascii != 0) {
                // KernelRenderer.print(ascii, KernelRenderer.GetBackgroundColor() == Color::BLACK ? Color::WHITE : Color ::BLACK);
            } else if (KeyboardDebugMode == KB_DEBUG_MODE::UNKNOWN) {
                // KernelRenderer.print(to_hstring(scancode), Color::BLUE, {false, Color::YELLOW});
            }

            break;
        }
    }
}
