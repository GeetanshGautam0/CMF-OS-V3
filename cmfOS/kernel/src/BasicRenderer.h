#pragma once
#include "math.h"
#include "FrameBuffer.h"
#include "SimpleFont.h"
#include <stddef.h>


enum Shape { RECT, HORIZ_LINE, VERT_LINE };


struct Region {
    unsigned int x_start;
    unsigned int x_end;
    unsigned int y_start;
    unsigned int y_end;
};


struct Geometry {
    Shape shape;
    Region region;
    unsigned int stroke_width = 0;
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
        void drawHShape(Geometry geometry, unsigned int color = 0xffffffff);
        void drawFShape(Geometry geometry, unsigned int color = 0xffffffff, unsigned int fill_color = 0xff00ff00);
};


bool authorized(Region bounds, unsigned int x, unsigned int y);
