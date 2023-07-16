#pragma once 
#include "sh_regions.hpp"
#include "../HEADERS/std.hpp"
#include "../HEADERS/str.hpp"
#include "../HEADERS/pit.hpp"
#include "../HEADERS/math.hpp"
#include "../HEADERS/code.hpp"
#include "../HEADERS/ktypedef.hpp"
#include "../HEADERS/pic_time.hpp"


#ifndef _SH_SRPOL
    #define _SH_SRPOL

    // Control variables 
    // 1) Snap cursor to grid (comment out if unwanted)
    #define _SH_RPOL_CURSOR_SNAP_TO_GRID            

#endif


#ifndef _SH_SIMPLE_RENDERER
    #define _SH_SIMPLE_RENDERER

#endif

#ifndef _SH_DEFINED_REGIONS
    #define _SH_DEFINED_REGIONS
    static DefinedRegions dr;

#endif

#ifndef _SH_R_CONSTS
    #define _SH_R_CONSTS

    enum Shape {RECT, HLINE, VLINE};
    enum SystemOverlay {MouseCursor};
    
    struct Geometry {
        Shape shape;
        Region region;
    };

    struct AutoColor {
        bool use_default = true;
        COLOR::Color color = COLOR::VGA::BLACK;
    };

#endif


class SimpleRenderer{

    public:
        SimpleRenderer(
            KERNEL::FrameBuffer* frame_buffer_ptr, 
            KERNEL::SimpleFont::PSF1_FONT* psf1_font_ptr,
            Region renderer_region,
            COLOR::Color renderer_background,
            bool create_renderer_overlay = false,
            const char* overlay_renderer_id = "no---------------------------key"
        );

        void Lock(const char* auth_key);
        bool IsReady();
        bool IsLocked();
        ReturnCode PanicMode(const char* auth_key, const char* panic_key);
        void DisablePanicMode(const char* auth_key, const char* panic_key);
        bool IsAuthorized(uint32_t x, uint32_t y, bool cr = true);
        
        void PutChar(
            char chr, 
            uint32_t x, 
            uint32_t y,
            COLOR::Color foreground,
            AutoColor background = {true, COLOR::VGA::WHITE},
            bool set_background = true 
        );
        
        void PrintString(
            const char* str,
            COLOR::Color foreground,
            AutoColor background = {true, COLOR::VGA::WHITE},
            bool set_background = true
        );

        void ClearRegion(
            Region region,
            AutoColor color = {true, COLOR::VGA::WHITE}
        );
        
        void SetBackground(
            COLOR::Color region_background
        );

        ReturnCode DefineNewOverlay(
            Region region,
            const char* region_id
        );

        Region GetBounds();
        Math::Point GetCursorPosition();
        ReturnCode SetCursorPosition(uint32_t x, uint32_t y);
        ReturnCode SetCursorPosition(Math::Point new_position);

        bool CheckRegionO(bool custom_check = false, Region r = {0, 0, 0, 0}, const char* region_id = "");

    private:
        KERNEL::FrameBuffer* fb_ptr;
        KERNEL::SimpleFont::PSF1_FONT* font_ptr;
        bool setup = false;
        bool locked = false;
        bool in_panic_mode = false;
        char* region_id;  
        char* panic_key = "uoaydfaheipmw3i47w9joiH^*&(o@il)";
        COLOR::Color region_background;
        Math::Point CursorPosition;
        Region r_bounds;

        void _putChar(char c, uint32_t x, uint32_t y, uint32_t fg, uint32_t bg, bool set_bg);
        void _clearRegion(Region r, uint32_t rgba);
        void _trapCursor(bool roll_over = true);
        Region _limitRegion(Region r);
        void _unlock();
        bool a2r();

};

extern SimpleRenderer SH_SR;
