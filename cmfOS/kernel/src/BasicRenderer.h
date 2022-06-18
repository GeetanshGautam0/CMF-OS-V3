#pragma once
#include "math.h"
#include "FrameBuffer.h"
#include "SimpleFont.h"
#include <stddef.h>


struct Region {
    unsigned int x_start;
    unsigned int x_end;
    unsigned int y_start;
    unsigned int y_end;
};


class BasicRenderer {
    public:
        BasicRenderer(FrameBuffer* frame_buffer, PSF1_FONT* simple_font, Region bounds);

        Region RendererBounds;
        Point CursorPosition;
        FrameBuffer* frameBuffer;
        PSF1_FONT* font;

        void printString(const char* string, unsigned int color=0xffffffff, unsigned int background=0x00000000);
        void putChar(char chr, unsigned int xOff, unsigned int yOff, unsigned int color=0xffffffff, unsigned int background=0x00000000);
        void clearRegion(Region region, unsigned int color = 0xff000000);
        void clearScreen(unsigned int color = 0xff000000);
        void drawHRect(Region region, unsigned int color = 0xffffffff);
        void drawFRect(Region region, unsigned int color = 0xffffffff, unsigned int fill_color = 0xff00ff00);
};
