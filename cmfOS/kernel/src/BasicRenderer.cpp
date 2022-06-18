#include "BasicRenderer.h"


bool authorized(Region bounds, unsigned int x, unsigned int y) {
    return bounds.x_start <= x <= bounds.x_end && bounds.y_start <= y <= bounds.y_end;
}


BasicRenderer::BasicRenderer(FrameBuffer* frame_buffer, PSF1_FONT* simple_font, Region bounds) 
{
    if (! (
            0 <= bounds.x_start 
         && bounds.x_end <= frame_buffer->Width 
         && 0 <= bounds.y_start
         && bounds.y_end <= frame_buffer->Height
         && bounds.x_start < bounds.x_end
         && bounds.y_start < bounds.y_end
    ) ) {
        if (!(bounds.x_start < bounds.x_end && bounds.y_start < bounds.y_end)) RendererBounds = {0, 0, 0, 0};
        else RendererBounds = { 
            (0 <= bounds.x_start ? bounds.x_start : 0),
            (bounds.x_end <= frame_buffer->Width ? bounds.x_end : frame_buffer->Width),
            (0 <= bounds.y_start ? bounds.y_start : 0),
            (bounds.y_end <= frame_buffer->Height ? bounds.y_end : frame_buffer->Height)
        };
    } else {
        RendererBounds = bounds;
    }

    frameBuffer = frame_buffer;
    font = simple_font;
    CursorPosition = {0, 0};
}


void BasicRenderer::printString(const char* string, unsigned int color, unsigned int background) {
    if (font == NULL || frameBuffer == NULL) return;  // not ready to print yet.

    char* chr = (char*)string;
    while (*chr != 0) {
        putChar(*chr, CursorPosition.x, CursorPosition.y, color, background);
        CursorPosition.x += font->font_size.w;
        if (CursorPosition.x + font->font_size.w > RendererBounds.x_end) {
            CursorPosition.x = RendererBounds.x_start;
            CursorPosition.y += font->font_size.h;
        }

        if (CursorPosition.y > RendererBounds.y_end) {
            CursorPosition.x = RendererBounds.x_start;
            CursorPosition.y = RendererBounds.y_start;
        }
        chr++;
    }
}


void BasicRenderer::putChar(char chr, unsigned int xOff, unsigned int yOff, unsigned int color, unsigned int background) {
    if (font == NULL || frameBuffer == NULL) return;  // not ready to print yet.

    unsigned int* pixPtr = (unsigned int*)frameBuffer->BaseAddress;
    char* fontPtr = (char*)font->glyphBuffer + (chr * font->psf1_Header->charsize);
    for (unsigned long y = yOff; y < yOff + font->psf1_Header->charsize; y++) {
        for (unsigned long x = xOff; x < xOff + font->font_size.w; x++) {
            if ( (*fontPtr & (0b10000000 >> (x - xOff))) > 0 ) {
                *(unsigned int*)(pixPtr + x + (y * frameBuffer->PixelsPerScanline)) = color;
            } else {
                *(unsigned int*)(pixPtr + x + (y * frameBuffer->PixelsPerScanline)) = background;
            }
        }   
        fontPtr ++; 
    }

}


void BasicRenderer::clearRegion(Region region, unsigned int color) {
    unsigned int* pixPtr = (unsigned int*)frameBuffer->BaseAddress;
    for (unsigned int x = region.x_start; x <= region.x_end; x++) {
        for (unsigned int y = region.y_start; y <= region.y_end; y++) {
            if (authorized(RendererBounds, x, y)) 
                *(unsigned int*)(pixPtr + x + (y * frameBuffer->PixelsPerScanline)) = color;
        }   
    }
}


void BasicRenderer::clearScreen(unsigned int color) {
    unsigned int* pixPtr = (unsigned int*)frameBuffer->BaseAddress;
    for (unsigned int x = RendererBounds.x_start; x <= RendererBounds.x_end; x++) {
        for (unsigned int y = RendererBounds.y_start; y <= RendererBounds.y_end; y++) {
            *(unsigned int*)(pixPtr + x + (y * frameBuffer->PixelsPerScanline)) = color;
        }   
    }
}


void BasicRenderer::drawHRect(Region region, unsigned int color) {
    unsigned int* pixPtr = (unsigned int*)frameBuffer->BaseAddress;
    for (unsigned int x = region.x_start; x <= region.x_end; x++) {
        if (authorized(RendererBounds, x, region.y_start)) {
            *(unsigned int*)(pixPtr + x + (region.y_start * frameBuffer->PixelsPerScanline)) = color;
        }
        if (authorized(RendererBounds, x, region.y_end)) {
            *(unsigned int*)(pixPtr + x + (region.y_end * frameBuffer->PixelsPerScanline)) = color;
        }
    }
    for (unsigned int y = region.y_start; y <= region.y_end; y++) {

        if (authorized(RendererBounds, region.x_start, y)) {
            *(unsigned int*)(pixPtr + region.x_start + (y * frameBuffer->PixelsPerScanline)) = color;
        }
        if (authorized(RendererBounds, region.x_end, y)) {
            *(unsigned int*)(pixPtr + region.x_end + (y * frameBuffer->PixelsPerScanline)) = color;
        }
    }

}


void BasicRenderer::drawFRect(Region region, unsigned int color, unsigned int fill_color) {
    clearRegion(region, fill_color);
    drawHRect(region, color);
}
