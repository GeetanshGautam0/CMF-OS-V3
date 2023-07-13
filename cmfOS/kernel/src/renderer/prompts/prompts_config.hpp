#pragma once
#include "../../std/ktypedef.h"
#include "../regions.hpp"
#include "../BasicRenderer.hpp"


namespace PromptConfig {

    #define MSG_LEN_PER_ROW 60
    #define MSG_N_ROWS 6

    extern Font::PSF1_FONT* fontPtr;
    extern Region r; 
    extern BasicRenderer::Renderer pR;

    void setFont(Font::PSF1_FONT* fontPtr);
    void setRegion(Region r); 
    void completeSetup(BasicRenderer::Renderer* baseRenderer, FrameBuffer::FrameBuffer* fb_ptr, Font::PSF1_FONT* font_ptr);

    bool Ready();

}