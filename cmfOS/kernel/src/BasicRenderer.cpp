#include "BasicRenderer.h"


BasicRenderer::BasicRenderer(FrameBuffer* frame_buffer, PSF1_FONT* simple_font) 
{
    frameBuffer = frame_buffer;
    font = simple_font;
    CursorPosition = {0, 0};
}


void BasicRenderer::printString(const char* string, unsigned int color) {
    if (font == NULL || frameBuffer == NULL) return;  // not ready to print yet.

    char* chr = (char*)string;
    while (*chr != 0) {
        putChar(*chr, CursorPosition.x, CursorPosition.y, color);
        CursorPosition.x += 8;
        if (CursorPosition.x + 8 > frameBuffer->Width) {
            CursorPosition.x = 0;
            CursorPosition.y += font->psf1_Header->charsize;
        }
        chr++;
    }
}


void BasicRenderer::putChar(char chr, unsigned int xOff, unsigned int yOff, unsigned int color) {
    if (font == NULL || frameBuffer == NULL) return;  // not ready to print yet.

    unsigned int* pixPtr = (unsigned int*)frameBuffer->BaseAddress;
    char* fontPtr = (char*)font->glyphBuffer + (chr * font->psf1_Header->charsize);
    for (unsigned long y = yOff; y < yOff + font->psf1_Header->charsize; y++) {
        for (unsigned long x = xOff; x < xOff + 8; x++) {
            if ( (*fontPtr & (0b10000000 >> (x - xOff))) > 0 ) {
                *(unsigned int*)(pixPtr + x + (y * frameBuffer->PixelsPerScanline)) = color;
            }
        }   
        fontPtr ++; 
    }

}
