#include "ktypedef.h"


namespace Color {

    unsigned long createRGBA(Color color) {   
        #ifndef KERNEL__COLOR_MODE
            // add interrupt for panic
        #endif
        
        #ifdef __COLOR_MODE_BRGA
            return ((color.B & 0xff) << 24) + ((color.R & 0xff) << 16) + ((color.G & 0xff) << 8) + (color.A & 0xff);
        
        #else
            
            #ifdef __COLOR_MODE_RGBA
                return ((color.R & 0xff) << 24) + ((color.G & 0xff) << 16) + ((color.B & 0xff) << 8) + (color.A & 0xff);
            #endif
        
        #endif
            
    }

    unsigned long createRGBA(unsigned long color) { return color; }

    unsigned int createRGBA(unsigned int color) { return color; }

    bool operator ==(Color one, Color two) {
        return AreEqual(one, two);
    }

    bool AreEqual(Color one, Color two) {
        return ((one.R == two.R) && (one.G == two.G) && (one.B == two.B) && (one.A == two.A));
    }

}