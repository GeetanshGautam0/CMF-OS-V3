#include "KBScanCodes.h"


char QWERTYUppercase(uint8_t scancode, const char Table[]) {
    // Number Row
    if (0x02 <= scancode && scancode <= 0x0d) {
        switch(scancode) {
            // case (0x02 || 0x04 || 0x05 || 0x06):    return scancode + 0x1f;
            case (0x02):    return scancode + 0x1f;
            case (0x04):    return scancode + 0x1f;
            case (0x05):    return scancode + 0x1f;
            case (0x06):    return scancode + 0x1f;
            case (0x03):    return 0x40;
            case (0x07):    return 0x5e;
            case (0x08):    return 0x26;
            case (0x09):    return 0x2a;
            case (0x0A):    return 0x28;
            case (0x0B):    return 0x29;
            case (0x0C):    return 0x5f;
            case (0x0D):    return 0x2b;

            default:        return 0;
        }
    }

    // Alphabets -> Uppercase 
    const char character = Table[scancode];
    if (0x61 <= character && character <= 0x7a) { return character - 32; }
    
    // Other characters
    switch (scancode) {
        case (0x1a):    return '{';
        case (0x1b):    return '}';
        case (0x2B):    return '|';
        case (0x27):    return ':';
        case (0x28):    return '"';
        case (0x33):    return '<';
        case (0x34):    return '>';
        case (0x35):    return '?';
        case (0x29):    return '~';

        default:        return 0;
    }

    return 0;
}

char QWERTYLowercase(uint8_t scancode, const char Table[]) {
    const char character = Table[scancode];
    if (character != 0) return character;

    switch(scancode) {
        case (0x29):    return '~';
        default:        return 0;
    }
}

namespace QWERTY {
    const char ASCIITable[] = {
        0 ,  0 , '1', '2',
        '3', '4', '5', '6',
        '7', '8', '9', '0',
        '-', '=',  0 ,  0 ,
        'q', 'w', 'e', 'r',
        't', 'y', 'u', 'i',
        'o', 'p', '[', ']',
         0 ,  0 , 'a', 's',
        'd', 'f', 'g', 'h',
        'j', 'k', 'l', ';',
        '\'','`',  0 , '\\',
        'z', 'x', 'c', 'v',
        'b', 'n', 'm', ',',
        '.', '/',  0 , '*',
         0 , ' '
    };

    char Traslate(uint8_t scancode, bool uppercase) {
        if (scancode > 58) return 0;
        return uppercase ? QWERTYUppercase(scancode, ASCIITable) : QWERTYLowercase(scancode, ASCIITable);
    }

}
