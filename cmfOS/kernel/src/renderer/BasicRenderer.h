#pragma once
#include "../std/ktypedef.h"


enum Shape 
    { RECT, HORIZ_LINE, VERT_LINE };

struct Region 
    {
        unsigned int x_start;
        unsigned int x_end;
        unsigned int y_start;
        unsigned int y_end;
    };

struct Geometry 
    {
        Shape shape;
        Region region;
        unsigned int stroke_width = 1;
    };

// union AutoColor 
struct AutoColor
    {
        bool UseDefault = true;
        Color color = BLACK;
    };

class BasicRenderer {
    public:
        void setup(FrameBuffer* frame_buffer, PSF1_FONT* simple_font, Region bounds);
        void printString(const char* string, Color color, AutoColor background);
        void printChar(const char character, Color color, AutoColor background);
        void putChar(char chr, unsigned int xOff, unsigned int yOff, Color color, Color background);
        void clearRegion(Region region, AutoColor color);
        void clearScreen(Color color, bool resetCursorPos = true);
        void drawHShape(Geometry geometry, Color color);
        void drawFShape(Geometry geometry, Color color, Color fill_color);
        void SetCursorPos(unsigned int x, unsigned int y); 
        bool IsReady();
        Point GetCursorPos();
        FontSize GetFontSize();
        Region GetBounds();
        Color GetBackgroundColor();
        bool authorized(unsigned int x, unsigned int y);
        void Lock();

    private:
        Region RendererBounds;
        bool locked = false;
        FrameBuffer* frameBuffer;
        PSF1_FONT* font;
        Point CursorPosition;
        bool ready = false;
        void _putChar(char chr, unsigned int xOff, unsigned int yOff, unsigned int color, unsigned int background);
        void _clearRegion(Region region, unsigned int color);
        void _drawHLine(unsigned int x_start, unsigned int x_end, unsigned int y, unsigned int color, unsigned int stroke);
        void _drawVLine(unsigned int y_start, unsigned int y_end, unsigned int x, unsigned int color, unsigned int stroke); 
        void _drawHRect(Geometry geo, unsigned int color);
        void _drawFRect(Geometry geo, unsigned int color, unsigned int fill_color);
        Color RegionBackgroundColor = BLACK;
        void trapCursor(bool rollover = true);
        bool a2r();
        void Unlock();  // Must not be accessed externally
};
