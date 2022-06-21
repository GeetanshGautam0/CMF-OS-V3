#pragma once
#include "ktypedef.h"


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
    unsigned int stroke_width = 1;
};


class BasicRenderer {
    public:
        BasicRenderer(FrameBuffer* frame_buffer, PSF1_FONT* simple_font, Region bounds);

        Region RendererBounds;
        Point CursorPosition;
        FrameBuffer* frameBuffer;
        PSF1_FONT* font;

        void printString(const char* string, Color color = WHITE, Color background = BLACK);
        void putChar(char chr, unsigned int xOff, unsigned int yOff, Color color = WHITE, Color background = BLACK);
        void clearRegion(Region region, Color color = RED);
        void clearScreen(Color color = BLACK);
        void drawHShape(Geometry geometry, Color color = WHITE);
        void drawFShape(Geometry geometry, Color color = WHITE, Color fill_color = BLUE);
};


bool authorized(Region bounds, unsigned int x, unsigned int y);
