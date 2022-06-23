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
        void printString(const char* string, Color color = WHITE, Color background = BLACK);
        void putChar(char chr, unsigned int xOff, unsigned int yOff, Color color = WHITE, Color background = BLACK);
        void clearRegion(Region region, Color color = RED);
        void clearScreen(Color color = BLACK);
        void drawHShape(Geometry geometry, Color color = WHITE);
        void drawFShape(Geometry geometry, Color color = WHITE, Color fill_color = BLUE);

        void SetCursorPos(unsigned int x = 0, unsigned int y = (unsigned int)NULL); 
        Point GetCursorPos();

        bool authorized(unsigned int x, unsigned int y);

    private:
        Region RendererBounds;
        FrameBuffer* frameBuffer;
        PSF1_FONT* font;
        Point CursorPosition;
        void _putChar(char chr, unsigned int xOff, unsigned int yOff, unsigned int color, unsigned int background);
        void _clearRegion(Region region, unsigned int color);
        void _drawHLine(unsigned int x_start, unsigned int x_end, unsigned int y, unsigned int color, unsigned int stroke);
        void _drawVLine(unsigned int y_start, unsigned int y_end, unsigned int x, unsigned int color, unsigned int stroke); 
        void _drawHRect(Geometry geo, unsigned int color);
        void _drawFRect(Geometry geo, unsigned int color, unsigned int fill_color);

};

