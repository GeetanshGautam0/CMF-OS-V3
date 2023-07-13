#include "BasicRenderer.hpp"
#include "../std/cstr.h"


namespace BasicRenderer {
    
    /*
        BasicRenderer::Renderer class constructor
    */

    Renderer::Renderer(
        FrameBuffer::FrameBuffer* frameBuffer, 
        Font::PSF1_FONT* f, 
        Region bounds, 
        Color::Color rBG,
        const char* _k,
        bool CRO
    ) {
        #ifndef __BasicRenderer
            return;
        #endif
        
        if (setup || locked) return;

        if (!CRO) 
        {
            RGN_K = (char*)dr.gen_key(
                (uint32_t)((int)(PIT::TimeSinceBoot * PIT::GetDivisor()) % 29639)
            );

            if (dr.define_new_region(bounds, true, (const char*)RGN_K) != ReturnCode::SUCCESS) return;
        }
        
        else if (check_region_o(true, bounds, _k)) RGN_K = (char*)_k;
        
        else return;

        rBounds = bounds;
        RegionBG = rBG;
        fb = frameBuffer;
        font = f;
        setup = true;
        locked = false;
        locked_extern = false;

        // Needs to happen after the setup, for some reason?
        if (
            !((bounds.x0 >= 0) 
         && (bounds.y0 >= 0)
         && (bounds.xf <= fb->Width)
         && (bounds.yf <= fb->Height)
         && (bounds.x0 < bounds.xf)
         && (bounds.y0 < bounds.yf))

         && false
        ) {
            _clearRegion(bounds, Color::createRGBA(Color::BLUE));
            SetCursorPosition(16, 16);
            printString("__BasicRenderer::Renderer::Renderer() <Error> failed to setup construct RGN: Invalid region parameters provided <API use error>.", Color::YELLOW, {false, Color::BLUE});
            
            dr.remove_region_definition(bounds, RGN_K);
            setup = false;
            locked = true; 
            locked_extern = false;
            
            return;
        }

        _clearRegion(rBounds, Color::createRGBA(rBG));
        SetCursorPosition(0, 0);

        return;
    }

    /*
        BasicRenderer::Renderer state getters
    */

    bool Renderer::IsReady()    { return setup; }
    bool Renderer::IsLocked()   { return locked; }
    CMath::Point Renderer::GetCursorPosition() { return CursorPosition; }
    
    /*
        BasicRenderer::Renderer - Private function setup
        Functions:

        void _putChar           UNSAFE <Does not check authority>       Prints a char at the specified (x, y) pixel coordinate
        void trapCursor                                                 Limits the cursor's point value to one within rBounds
        void _clearRegion       SAFE <Checks pixel ownership>           Clears the specified region and fills it with the provided color value (in uint32_t form)
    */

    void Renderer::_putChar(char c, uint32_t x, uint32_t y, uint32_t fg, uint32_t bg, bool setBG) {
        uint32_t* pixPtr = (uint32_t*)fb->BaseAddress;
        char* fontPtr = (char*)font->glyphBuffer + (c * font->psf1_Header->charsize);

        for (unsigned long Y = y; Y < y + font->psf1_Header->charsize; Y++) {
            for (unsigned long X = x; X < x + font->font_size.w; X++) {
                if ( (*fontPtr & (0b10000000 >> (X - x))) > 0 ) {
                    *(uint32_t*)(pixPtr + X + (Y * fb->PixelsPerScanline)) = fg;
                } else if (setBG) {
                    *(uint32_t*)(pixPtr + X + (Y * fb->PixelsPerScanline)) = bg;
                }
            }
            fontPtr ++;
        }
    }   

    void Renderer::trapCursor(bool roll_over) {
        if (!a2r()) return;
        if (font == NULL || fb == NULL) return;  // not ready to print yet.

        if (CursorPosition.x + font->font_size.w > rBounds.xf) {
            if (roll_over) {
                CursorPosition.x = rBounds.x0;
                CursorPosition.y += font->font_size.h;
            }
            else CursorPosition.x = rBounds.xf - font->font_size.w;
        }

        if (CursorPosition.y + font->font_size.h > rBounds.yf) {
            if (roll_over) {
                CursorPosition.x = rBounds.x0;
                CursorPosition.y = rBounds.y0;
            }
            else CursorPosition.y = CursorPosition.y - font->font_size.h;
        }

        if (CursorPosition.x < rBounds.x0) CursorPosition.x = rBounds.x0;
        if (CursorPosition.y < rBounds.y0) CursorPosition.y = rBounds.y0;
    }

    void Renderer::_clearRegion(Region r, uint32_t color) {
        check_region_o();

        unsigned int* pixPtr = (unsigned int*)fb->BaseAddress;
        for (unsigned int y = r.y0; y < r.yf; y++) {
            for (unsigned int x = r.x0; x < r.xf; x++) {
                if (is_authorized(x, y, false))
                    *(unsigned int*)(pixPtr + x + (y * fb->PixelsPerScanline)) = color;
            }
        }
    }

    /*
        BasicRenderer::Renderer - Authorization management
        Functions:

        bool a2r                                                "Able to run"
        bool is_authorized                                      "Renderer is authorized to operate at provided coordinate (pixel)"
        bool check_regio_o                          (1)         "Check region ownership" - No args - check if renderer is still in charge of its bounded region.
                                                    (2)                                  - Args provided - Check if the provided region and UID key pair are a valid region definition
        void lock                                   PUBLIC      Locks renderer with locked_extern set to TRUE
        void _unlock                                            Unlocks renderer, sets locked_extern to FALSE
        Region _limitRegion                                     Takes provided region and applies rBounds limitations to it.
        ReturnCode define_new_overlay_region        PUBLIC      Take ownership of a subset of the region of the renderer. NOTE:
                                                                    (i)     The renderer can still function and overwrite data in the newly defined region
                                                                    (ii)    The renderer can choose to de-allocate your region
                                                                    (iii)   Your region must conform to restriction imposed by rBounds
                                                                    (iv)    Your region ownership will be destroyed if the parent renderer is destroyed
                                                                    (v)     You must define a new BasicRenderer instance with your new key and region pair as arguments 
                                                                    (vi)    The regions will allow recursive overlay definitions only if __NO_RECURSIVE_OVERLAY is not defined. It is not recommended to define nested overlays. 
    */

   bool Renderer::a2r() {
        return (setup && !locked && !locked_extern);
    }

    bool Renderer::is_authorized(uint32_t x, uint32_t y, bool cR) {
        if (cR) check_region_o();
        return a2r() && (rBounds.x0 <= x) && (rBounds.xf >= x) && (rBounds.y0 <= y) && (rBounds.yf >= y);
    }

    bool Renderer::check_region_o(bool _NS, Region R, const char* S) {
        if (!_NS) {

            if (locked_extern) {  // If the renderer is locked by an external script, such as panic, then just return;
                locked = true;
                return false;
            }  

            locked = !dr.check_region_o(rBounds, RGN_K);

            return !locked;
        }

        return dr.check_region_o(R, S);

    }

    void Renderer::lock() {
        locked_extern = true; 
        locked = true;
        return;
    } 

    void Renderer::_unlock() {
        locked_extern = false; 
        locked = false; 
        return;
    }

    Region Renderer::_limitRegion(Region r) {
        Region newRegion; 

        if (r.x0 < rBounds.x0) newRegion.x0 = rBounds.x0;
        else newRegion.x0 = r.x0;

        if (r.xf > rBounds.xf) newRegion.xf = rBounds.xf; 
        else if (r.xf < rBounds.x0) newRegion.xf = rBounds.x0;
        else newRegion.xf = r.xf;

        if (r.y0 < rBounds.y0) newRegion.y0 = rBounds.y0;
        else newRegion.y0 = r.y0;

        if (r.yf > rBounds.yf) newRegion.yf = rBounds.yf; 
        else if (r.yf < rBounds.y0) newRegion.yf = rBounds.y0;
        else newRegion.yf = r.yf;

        return newRegion;
    }

    ReturnCode Renderer::define_new_overlay_region(Region r, const char* _K) {
        // Make sure the renderer is able to allocate sub-regions in the first place
        // Check for:
        // *    is the renderer setup
        // *    is the renderer locked
        // *    does the renderer still have ownership of what it thinks is its region.
        if (!dr.check_region_o(rBounds, RGN_K) || !a2r()) return ReturnCode::ERROR;

        // Allow access to a region of the render's region 
        return dr.define_overlay_region(r, _K, RGN_K); 
    }

    /*
        BasicRenderer::Renderer public functions
        Functions:

        * void clearRegion                      Clears the provided region and fills it with the provided color (in AutoColor form; default: use rBG)
        * void printString                      Prints a string at the CursorPosition
                                    OPTION          (i) sBG: set the color of the char-rect to rBG, excluding the pixels of the character. 
        * Region RendererBounds                 Returns rBounds
              
    */

    void Renderer::printString(const char* string, Color::Color fg, AutoColor bg, bool sBG) {
        if (!a2r()) return;
        unsigned long color_int = Color::createRGBA(fg);
        unsigned long background_int = bg.UseDefault ? Color::createRGBA(RegionBG) : createRGBA(bg.color);

        trapCursor();

        char* chr = (char*)string;
        while (*chr != 0) {
            if (*chr == '\n') {
                CursorPosition.y += font->font_size.h;
            }
            else if (*chr == '\r') {
                CursorPosition.x = 0;
            }
            else if (*chr == '\t') {
                for (int i = 0; i < 4; i++) {
                    _putChar(' ', CursorPosition.x, CursorPosition.y, color_int, background_int, sBG);
                    CursorPosition.x += font->font_size.w;
                    trapCursor();
                }
            }
            else {
                _putChar(*chr, CursorPosition.x, CursorPosition.y, color_int, background_int, sBG);
                CursorPosition.x += font->font_size.w;
            }

            trapCursor();
            chr++;
        }
    }

    void Renderer::clearRegion(Region region, AutoColor color) {
        check_region_o();
        if (!a2r()) return;

        Color::Color NColor;
        if (color.UseDefault) NColor = RegionBG;

        else NColor = color.color;
        _clearRegion(_limitRegion(region), Color::createRGBA(NColor));
    }

    Region Renderer::RendererBounds() {
        return rBounds;
    }

    ReturnCode Renderer::SetCursorPosition(CMath::Point p) {
        return SetCursorPosition(p.x, p.y);
    }

    ReturnCode Renderer::SetCursorPosition(uint32_t x, uint32_t y) {
        // This sets the coordinates relative to the renderer's bounds, not in an absolute plane.
        if (!is_authorized(rBounds.x0 + x, rBounds.y0 + y)) return ReturnCode::ERROR;
        CursorPosition = {rBounds.x0 + x, rBounds.y0 + y};

        return ReturnCode::SUCCESS;
    }

}
