#pragma once
#include "../std/ktypedef.h"


enum Shape 
    { RECT, HORIZ_LINE, VERT_LINE };

enum Overlay
    { MOUSE_CURSOR };

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
        void printString(const char* string, Color color, AutoColor background, bool setBackground = true);
        void printChar(const char character, Color color, AutoColor background, bool setBackground = true);
        void putChar(char chr, unsigned int xOff, unsigned int yOff, Color color, AutoColor background, bool setBackground = true);
        void clearRegion(Region region, AutoColor color);
        void clearScreen(Color color, bool resetCursorPos = true);
        void drawHShape(Geometry geometry, Color color);
        void drawFShape(Geometry geometry, Color color, Color fill_color);
        bool authorized(unsigned int x, unsigned int y);
        void deletePrevious();
        void drawMouseCursor(uint8_t* MouseCursor, Point position);
        void clearMouseCursor(uint8_t* MouseCursor, Point pos);

        void SetCursorPos(unsigned int x, unsigned int y); 
        bool IsReady();
        Point GetCursorPos();
        FontSize GetFontSize();
        Region GetBounds();
        Color GetBackgroundColor();
        void Lock();

    private:
        uint32_t MouseCursorBuffer[16 * 16];
        uint32_t MouseCursorBufferAfter[16 * 16];
        Region RendererBounds;
        bool locked = false;
        FrameBuffer* frameBuffer;
        PSF1_FONT* font;
        Point CursorPosition;
        Color RegionBackgroundColor = BLACK;
        bool ready = false;
        bool MouseDrawn = false;
        void _putChar(char chr, unsigned int xOff, unsigned int yOff, unsigned int color, unsigned int background, bool setBackground);
        void _clearRegion(Region region, unsigned int color);
        void _drawHLine(unsigned int x_start, unsigned int x_end, unsigned int y, unsigned int color, unsigned int stroke);
        void _drawVLine(unsigned int y_start, unsigned int y_end, unsigned int x, unsigned int color, unsigned int stroke); 
        void _drawHRect(Geometry geo, unsigned int color);
        void _drawFRect(Geometry geo, unsigned int color, unsigned int fill_color);
        void putPix(uint32_t x, uint32_t y, uint32_t color);
        uint32_t getPix(uint32_t x, uint32_t y);
        uint32_t findOppColor();
        void trapCursor(bool rollover = true);
        bool a2r();
        void Unlock();  // Must not be accessed externally
};
