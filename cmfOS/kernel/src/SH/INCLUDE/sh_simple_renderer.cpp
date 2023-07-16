#include "../sh_simple_renderer.hpp"


const char* PANIC_AUTH = "jah7f3hlf8a9fy^&(#*Uf82*#(@#88))";


bool 
SimpleRenderer::a2r() 
{
    #if !defined(_SH_SIMPLE_RENDERER) || !defined(_SH_DEFINED_REGIONS) || !defined(_SH_R_CONSTS)
        return false;
    #endif

    return setup && !(locked || in_panic_mode);
}


void 
SimpleRenderer::_unlock() 
{
    locked = false;
    in_panic_mode = false;
}


ReturnCode
SimpleRenderer::PanicMode(
    const char* ak,
    const char* pk
)
{

    #if !defined(_SH_SIMPLE_RENDERER) || !defined(_SH_DEFINED_REGIONS) || !defined(_SH_R_CONSTS)
        return ReturnCode::OTHER;
    #endif

    if (!setup) return ReturnCode::OTHER;
    if (in_panic_mode) return ReturnCode::WARNING;

    if (ak == PANIC_AUTH) 
    {
        panic_key = (char*)pk;
        in_panic_mode = true;
        locked = true;

        return ReturnCode::SUCCESS;
    }

    return ReturnCode::ERROR;

}


void 
SimpleRenderer::DisablePanicMode(
    const char* ak,
    const char* k
) 
{
    #if !defined(_SH_SIMPLE_RENDERER) || !defined(_SH_DEFINED_REGIONS) || !defined(_SH_R_CONSTS)
        return;
    #endif

    if (!setup) return;

    if (k == panic_key && ak == PANIC_AUTH) {
        _unlock();
        panic_key = "uoaydfaheipmw3i47w9joiH^*&(o@il)";
    }
}


bool 
SimpleRenderer::IsReady() 
{
    return a2r();
}


bool 
SimpleRenderer::IsLocked()
{
    return locked;
}


SimpleRenderer::SimpleRenderer(
    KERNEL::FrameBuffer* fbptr,
    KERNEL::SimpleFont::PSF1_FONT* fontptr,
    Region r,
    COLOR::Color rbg,
    bool cro,
    const char* rk
)
{

    #if !defined(_SH_SIMPLE_RENDERER) || !defined(_SH_DEFINED_REGIONS) || !defined(_SH_R_CONSTS)
        return; 
    #endif 

    if (setup) return;
    
    if (!dr.IsSetup()) dr.setup();
    
    if (!cro) {

        region_id = (char*)dr.GenerateID(
            (uint32_t)((int)(PIT::TimeSinceBoot * PIT::GetDivisor()) % 29639)
        );

        ReturnCode defined = dr.DefineNewRegion(r, true, (const char*)region_id);
        if (defined != ReturnCode::SUCCESS) return;

        r_bounds = r;
    }

    else if (dr.CheckRegionO(r, rk)) {

        region_id = (char*)rk;
        r_bounds = r;

    }

    else return;

    setup = true;
    locked = false;
    in_panic_mode = false;
    region_background = rbg;
    font_ptr = fontptr;
    fb_ptr = fbptr;
    CursorPosition = {0, 0};

    if (!(

            (r_bounds.x0 >= 0)
        &&  (r_bounds.xf <= fb_ptr->Width)
        &&  (r_bounds.x0 < r_bounds.xf)
        &&  (r_bounds.y0 >= 0)
        &&  (r_bounds.yf <= fb_ptr->Height)
        &&  (r_bounds.y0 < r_bounds.yf )

    )) 
    {

        dr.RemoveRegion(r, region_id);
    
        setup = false;
        locked = true;
        in_panic_mode = false;

        return;

    }

    _clearRegion(r, COLOR::createRGBA(rbg));

}


void
SimpleRenderer::_clearRegion(
    Region r,
    uint32_t rgba
)
{
    CheckRegionO();
    if (!a2r()) return;

    unsigned int* pixPtr = (unsigned int*)fb_ptr->BaseAddress;

    for (unsigned int y = r.y0; y < r.yf; y++)
    {
        for (unsigned int x = r.x0; x < r.xf; x++) 
        {
            if (IsAuthorized(x, y, false))
                *(unsigned int*)(pixPtr + x + (y * fb_ptr->PixelsPerScanLine)) = rgba;
        
        }
    }

    return;

}


bool 
SimpleRenderer::IsAuthorized(
    uint32_t x, 
    uint32_t y, 
    bool cR
) 
{
    
    if (cR) CheckRegionO();
    return a2r() && (r_bounds.x0 <= x) && (r_bounds.xf >= x) && (r_bounds.y0 <= y) && (r_bounds.yf >= y);

}

bool
SimpleRenderer::CheckRegionO(
    bool cc,
    Region r,
    const char* k
)
{

    if (!cc) {

        if (in_panic_mode) {
            locked = true;
            return false;
        }

        locked = !dr.CheckRegionO(r_bounds, region_id);
        return !locked;

    }

    return dr.CheckRegionO(r, k);

}   


void
SimpleRenderer::_putChar(
    char c,
    uint32_t x,
    uint32_t y,
    uint32_t fg,
    uint32_t bg,
    bool sbg
)
{

    CheckRegionO();
    if (!a2r()) return;

    uint32_t* pix_ptr = (uint32_t*)fb_ptr->BaseAddress;
    char* f = (char*)font_ptr->glyphBuffer + (c * font_ptr->psf1_Header->charsize);

    for (unsigned long Y = y; Y < y + font_ptr->psf1_Header->charsize; Y++) {
        for (unsigned long X = x; X < x + font_ptr->font_size.w; X++) {
            if ( (*f & (0b10000000 >> (X - x))) > 0 ) {
                *(uint32_t*)(pix_ptr + X + (Y * fb_ptr->PixelsPerScanLine)) = fg;
            } else if (sbg) {
                *(uint32_t*)(pix_ptr + X + (Y * fb_ptr->PixelsPerScanLine)) = bg;
            }
        }
        f ++;
    }

}


void 
SimpleRenderer::_trapCursor(
    bool ro
)
{

    if (!a2r()) return;

    #ifdef _SH_RPOL_CURSOR_SNAP_TO_GRID
        uint32_t rem_y = CursorPosition.y % font_ptr->font_size.h;
        uint32_t rem_x = CursorPosition.x % font_ptr->font_size.w;
        
        if (rem_x)
            CursorPosition.x += (font_ptr->font_size.w - rem_x);
        
        if (rem_y)
            CursorPosition.y += (font_ptr->font_size.h - rem_y);

    #endif

    if (CursorPosition.x + font_ptr->font_size.w > r_bounds.xf)
    {

        if (ro) 
        {
            CursorPosition.x = r_bounds.x0;
            CursorPosition.y += font_ptr->font_size.h;
        }
        else 
            CursorPosition.x = r_bounds.xf - font_ptr->font_size.w;

    }

    if (CursorPosition.y + font_ptr->font_size.h > r_bounds.yf)
    {

        if (ro)
            CursorPosition.y = r_bounds.y0;

        else 
            CursorPosition.y = r_bounds.xf - font_ptr->font_size.h; 

    }

    if (CursorPosition.x < r_bounds.x0) CursorPosition.x = r_bounds.x0;
    if (CursorPosition.y < r_bounds.y0) CursorPosition.y = r_bounds.y0;

}


void
SimpleRenderer::PrintString(
    const char* str,
    COLOR::Color fg,
    AutoColor bg,
    bool sbg 
)
{

    CheckRegionO();
    if (!a2r()) return;

    unsigned long fg_int = COLOR::createRGBA(fg);
    unsigned long bg_int = \
        bg.use_default ? COLOR::createRGBA(region_background) : createRGBA(bg.color);

    _trapCursor(true);
    char* chr = (char*)str;

    while 
    (*chr != 0) 
    {

        if (*chr == '\n')
            CursorPosition.y += font_ptr->font_size.h;

        else if (*chr == '\r')
            CursorPosition.x = r_bounds.x0;

        else if (*chr == '\t')
        {

            for 
            (
                int i = 0;
                i < 4;
                i ++
            )
            {
                
                // Incrementing the cursor position's x-value by four units
                // is not effective here the region skipped will not be
                // cleared.

                _putChar(' ', CursorPosition.x, CursorPosition.y, fg_int, bg_int, sbg);
                CursorPosition.x += font_ptr->font_size.w;
                _trapCursor(true);

            }

        }
        else 
        {

            _putChar(
                *chr,
                CursorPosition.x,
                CursorPosition.y,
                fg_int, bg_int, sbg
            );
            CursorPosition.x += font_ptr->font_size.w;

        }

        _trapCursor();
        chr++;

    } 

}
