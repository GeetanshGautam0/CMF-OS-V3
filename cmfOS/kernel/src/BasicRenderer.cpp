#include "BasicRenderer.h"


bool authorized(Region region, unsigned int x, unsigned int y) {
    // return (region.x_start <= x <= region.x_end && region.y_start <= y <= region.y_end);
    return (region.x_start <= x && x <= region.x_end && region.y_start <= y && y <= region.y_end);
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


void _putChar(FrameBuffer* frameBuffer, PSF1_FONT* font, char chr, unsigned int xOff, unsigned int yOff, unsigned int color, unsigned int background) {
    // Doesn't run aditional checks
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


void BasicRenderer::printString(const char* string, unsigned int color, unsigned int background) {
    if (font == NULL || frameBuffer == NULL) return;  // not ready to print yet.

    if (CursorPosition.x + font->font_size.w > RendererBounds.x_end) {
            CursorPosition.x = RendererBounds.x_start;
            CursorPosition.y += font->font_size.h;
    }

    if (CursorPosition.y + font->font_size.h > RendererBounds.y_end) {
        CursorPosition.x = RendererBounds.x_start;
        CursorPosition.y = RendererBounds.y_start;
    }

    if (CursorPosition.x < RendererBounds.x_start) CursorPosition.x = RendererBounds.x_start;
    if (CursorPosition.y < RendererBounds.y_start) CursorPosition.y = RendererBounds.y_start;

    char* chr = (char*)string;
    while (*chr != 0) {
        _putChar(frameBuffer, font, *chr, CursorPosition.x, CursorPosition.y, color, background);
        CursorPosition.x += font->font_size.w;
        
        if (CursorPosition.x + font->font_size.w > RendererBounds.x_end) {
            CursorPosition.x = RendererBounds.x_start;
            CursorPosition.y += font->font_size.h;
        }

        if (CursorPosition.y + font->font_size.h > RendererBounds.y_end) {
            CursorPosition.x = RendererBounds.x_start;
            CursorPosition.y = RendererBounds.y_start;
        }

        if (CursorPosition.x < RendererBounds.x_start) CursorPosition.x = RendererBounds.x_start;
        if (CursorPosition.y < RendererBounds.y_start) CursorPosition.y = RendererBounds.y_start;

        chr++;
    }
}


void BasicRenderer::putChar(char chr, unsigned int xOff, unsigned int yOff, unsigned int color, unsigned int background) {
    if (font == NULL || frameBuffer == NULL) return;  // not ready to print yet.
    if (!(authorized(RendererBounds, xOff, yOff) && authorized(RendererBounds, xOff + font->font_size.w, yOff + font->font_size.h))) return;

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


void _clearRegion(FrameBuffer* frameBuffer, Region rb, Region region, unsigned int color) {
    unsigned int* pixPtr = (unsigned int*)frameBuffer->BaseAddress;
    for (unsigned int x = region.x_start; x <= region.x_end; x++) {
        for (unsigned int y = region.y_start; y <= region.y_end; y++) {
            if (authorized(rb, x, y)) 
                *(unsigned int*)(pixPtr + x + (y * frameBuffer->PixelsPerScanline)) = color;
        }   
    }
}


void BasicRenderer::clearRegion(Region region, unsigned int color) { _clearRegion(frameBuffer, RendererBounds, region, color); }
void BasicRenderer::clearScreen(unsigned int color) { _clearRegion(frameBuffer, RendererBounds, RendererBounds, color); }


// Shape Renderers (private)


void _drawHLine(FrameBuffer* fb, Region rBounds, unsigned int x_start, unsigned int x_end, unsigned int y, unsigned int color) {
     unsigned int* pixPtr = (unsigned int*)fb->BaseAddress;
     for (unsigned int x = x_start; x <= x_end; x++) {
        if (authorized(rBounds, x, y)) {
            *(unsigned int*)(pixPtr + x + (y * fb->PixelsPerScanline)) = color;
        }
     }
}

void _drawVLine(FrameBuffer* fb, Region rBounds, unsigned int y_start, unsigned int y_end, unsigned int x, unsigned int color) {
     unsigned int* pixPtr = (unsigned int*)fb->BaseAddress;
     for (unsigned int y = y_start; y <= y_end; y++) {
        if (authorized(rBounds, x, y)) {
            *(unsigned int*)(pixPtr + x + (y * fb->PixelsPerScanline)) = color;
        }
     }
}

void _drawHRect(FrameBuffer* fb, Region rb, Region region, unsigned int color) {
    unsigned int* pixPtr = (unsigned int*)fb->BaseAddress;
        for (unsigned int x = region.x_start; x <= region.x_end; x++) {
            if (authorized(rb, x, region.y_start)) {
                *(unsigned int*)(pixPtr + x + (region.y_start * fb->PixelsPerScanline)) = color;
            }
            if (authorized(rb, x, region.y_end)) {
                *(unsigned int*)(pixPtr + x + (region.y_end * fb->PixelsPerScanline)) = color;
            }
        }
        for (unsigned int y = region.y_start; y <= region.y_end; y++) {

            if (authorized(rb, region.x_start, y)) {
                *(unsigned int*)(pixPtr + region.x_start + (y * fb->PixelsPerScanline)) = color;
            }
            if (authorized(rb, region.x_end, y)) {
                *(unsigned int*)(pixPtr + region.x_end + (y * fb->PixelsPerScanline)) = color;
            }
        }
}


void _drawFRect(FrameBuffer* fb, Region rb, Region region, unsigned int color, unsigned int fill_color) {
    _clearRegion(fb, rb, region, fill_color);
    _drawHRect(fb, rb, region, color);
}


// Shape Renderers (public)


void BasicRenderer::drawHShape(Geometry geometry, unsigned int color) {
    if (geometry.shape == RECT) _drawHRect(frameBuffer, RendererBounds, geometry.region, color);
    else if (geometry.shape == HORIZ_LINE) _drawHLine(frameBuffer, RendererBounds, geometry.region.x_start, geometry.region.x_end, geometry.region.y_start, color);
    else if (geometry.shape == VERT_LINE) _drawVLine(frameBuffer, RendererBounds, geometry.region.y_start, geometry.region.y_end, geometry.region.x_start, color);
}


void BasicRenderer::drawFShape(Geometry geometry, unsigned int color, unsigned int fill_color) {
    if (geometry.shape == RECT) _drawFRect(frameBuffer, RendererBounds, geometry.region, color, fill_color);
    else if (geometry.shape == HORIZ_LINE) _drawHLine(frameBuffer, RendererBounds, geometry.region.x_start, geometry.region.x_end, geometry.region.y_start, color);
    else if (geometry.shape == VERT_LINE) _drawVLine(frameBuffer, RendererBounds, geometry.region.y_start, geometry.region.y_end, geometry.region.x_start, color);
}
