#include "ktypedef.h"


unsigned long createRGBA(Color color) {   
    return ((color.B & 0xff) << 24) + ((color.R & 0xff) << 16) + ((color.G & 0xff) << 8) + (color.A & 0xff);
}

unsigned long createRGBA(unsigned long color) { return color; }

unsigned int createRGBA(unsigned int color) { return color; }

bool Colors_AreEqual(Color one, Color two) {
    return ((one.R == two.R) && (one.G == two.G) && (one.B == two.B) && (one.A == two.A));
}
