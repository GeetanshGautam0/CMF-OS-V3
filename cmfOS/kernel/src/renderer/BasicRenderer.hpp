#pragma once
#include "regions.hpp"
#include "../std/ktypedef.h"
#include "../std/math.h"
#include "../std/ctypedef.h"


#ifndef __BasicRenderer
    #define __BasicRenderer
#endif

static DefinedRegions dr;

enum Shape {RECT, HLINE, VLINE};

enum Overlay {MOUSE_CURSOR};

struct Geometry {
    Shape shape;
    Region region;
};

struct AutoColor {
    bool UseDefault = true;
    Color::Color color = Color::BLACK;
};

namespace BasicRenderer {

    class Renderer {
        public:
            Renderer(FrameBuffer::FrameBuffer* frameBuffer, Font::PSF1_FONT* psf1_font, Region region, Color::Color RegionBackgroundColor, const char* drKey = "", bool use_CRO = false);
            bool IsReady();
            bool IsLocked();

            bool is_authorized(uint32_t x, uint32_t y, bool cr = true);
            void putChar(char chr, uint32_t x, uint32_t y, Color::Color fg, AutoColor bg = {true, Color::WHITE}, bool setBG = true);
            void printString(const char* string, Color::Color fg, AutoColor bg = {true, Color::WHITE}, bool setBG = true);

            void lock();
            void clearRegion(Region region, AutoColor color = {true, Color::WHITE});

            ReturnCode define_new_overlay_region(Region r, const char* region_identifier_key);
            Region RendererBounds();

            CMath::Point GetCursorPosition();
            ReturnCode SetCursorPosition(CMath::Point position);
            ReturnCode SetCursorPosition(uint32_t x, uint32_t y);

            bool check_region_o(bool customCheck = false, Region r = {0, 0, 0, 0}, const char* _K = ""); 
            
        private:
            FrameBuffer::FrameBuffer* fb;
            Font::PSF1_FONT* font;
            bool setup = false;
            bool locked = false;
            bool locked_extern = false; 
            char* RGN_K;
            Color::Color RegionBG;
            CMath::Point CursorPosition; 
            Region rBounds;

            void _putChar(char c, uint32_t x, uint32_t y, uint32_t fg, uint32_t bg, bool setBG);
            void _unlock();
            void _clearRegion(Region r, uint32_t rgba);
            void trapCursor(bool roll_over = true);
            Region _limitRegion(Region r);

            bool a2r();
    };

}


extern BasicRenderer::Renderer br; 
