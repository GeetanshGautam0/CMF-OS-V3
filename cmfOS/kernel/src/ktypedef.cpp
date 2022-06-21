#include "ktypedef.h"


unsigned long createRGBA(Color color) {   
    return ((color.B & 0xff) << 24) + ((color.R & 0xff) << 16) + ((color.G & 0xff) << 8) + (color.A & 0xff);
}
