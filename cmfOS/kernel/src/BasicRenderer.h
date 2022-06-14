#pragma once
#include "math.h"
#include "FrameBuffer.h"
#include "SimpleFont.h"
#include <stddef.h>


class BasicRenderer {
    public:
        BasicRenderer(FrameBuffer* frame_buffer, PSF1_FONT* simple_font);

        Point CursorPosition;
        FrameBuffer* frameBuffer;
        PSF1_FONT* font;
        void printString(const char* string, unsigned int color=0xffffffff);
        void putChar(char chr, unsigned int xOff, unsigned int yOff, unsigned int color=0xffffffff);
};
