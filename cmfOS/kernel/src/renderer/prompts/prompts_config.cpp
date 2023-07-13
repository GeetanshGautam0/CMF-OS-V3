#include "prompts_config.hpp"


namespace PromptConfig {

    Region r; 
    Font::PSF1_FONT* fontPtr;

    static bool fontSet = false;
    static bool regionSet = false;
    const static char* _PCK = dr.gen_key(rand());;

    void setFont(Font::PSF1_FONT* fPtr) {
        if (fontSet) return;

        fontPtr = fPtr; 
        fontSet = true;
    }

    void setRegion(Region reg) {
        if (regionSet) return;

        r = reg;
        regionSet = true;
    }

    bool Ready() { return (fontSet && regionSet); }

    void completeSetup(BasicRenderer::Renderer* _br, FrameBuffer::FrameBuffer* fb, Font::PSF1_FONT* font) {
        Region PRB = _br->RendererBounds();
        uint32_t PRB_w = PRB.xf - PRB.x0;
        uint32_t PRB_h = PRB.yf - PRB.y0;
        
        Region pRegion = {
            PRB.x0 + (unsigned int)((PRB_w / 2) - (int)(MSG_LEN_PER_ROW / 2)*font->font_size.w),    // x0
            PRB.y0 + (unsigned int)((PRB_h / 2) - (int)(MSG_N_ROWS / 2 + 1)*font->font_size.h),     // y0
            PRB.x0 + (unsigned int)((PRB_w / 2) + (int)(MSG_LEN_PER_ROW / 2)*font->font_size.w),    // xf
            PRB.y0 + (unsigned int)((PRB_h / 2) + (int)(MSG_N_ROWS / 2 + 1)*font->font_size.h)      // yf
        };

        setRegion(pRegion);
        setFont(font);

        _br->define_new_overlay_region(r, _PCK);
        BasicRenderer::Renderer pR(fb, font, r, Color::BLACK, _PCK, true);
    }

}