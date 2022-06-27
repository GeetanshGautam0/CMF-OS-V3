#include "BasicRenderer.h"


#ifndef RENDERER_BLOCK_CHUNK_SIZE
#define RENDERER_BLOCK_CHUNK_SIZE 256
#endif


void BasicRenderer::trapCursor(bool rollover) {
    if (!ready) return;
    if (font == NULL || frameBuffer == NULL) return;  // not ready to print yet.

    if (CursorPosition.x + font->font_size.w > RendererBounds.x_end) {
        if (rollover) {
            CursorPosition.x = RendererBounds.x_start;
            CursorPosition.y += font->font_size.h;
        } 
        else CursorPosition.x = RendererBounds.x_end - font->font_size.w;
    }

    if (CursorPosition.y + font->font_size.h > RendererBounds.y_end) {
        if (rollover) {
            CursorPosition.x = RendererBounds.x_start;
            CursorPosition.y = RendererBounds.y_start;
        } 
        else CursorPosition.y = CursorPosition.y - font->font_size.h;
    }

    if (CursorPosition.x < RendererBounds.x_start) CursorPosition.x = RendererBounds.x_start;
    if (CursorPosition.y < RendererBounds.y_start) CursorPosition.y = RendererBounds.y_start;
}

bool BasicRenderer::authorized(unsigned int x, unsigned int y) {
    return (
           RendererBounds.x_start <= x 
        && RendererBounds.y_start <= y 
        && x <= RendererBounds.x_end 
        && y <= RendererBounds.y_end
        && ready
    );
}

Point BasicRenderer::GetCursorPos() { return CursorPosition; }

FontSize BasicRenderer::GetFontSize() { return font->font_size; }

Region BasicRenderer::GetBounds() { return RendererBounds; }

Color BasicRenderer::GetBackgroundColor() { return RegionBackgroundColor; }

bool BasicRenderer::IsReady() { return ready; }

void BasicRenderer::SetCursorPos(unsigned int x, unsigned int y) {
    if (!ready) return;
    x = x * font->font_size.w + RendererBounds.x_start;  // Treats like lines and characters now, instead of pixels
    y = y * font->font_size.h + RendererBounds.y_start;  // Treats like lines and characters now, instead of pixels

    CursorPosition = {x, y};
    trapCursor(false);
}


void BasicRenderer::setup(FrameBuffer* frame_buffer, PSF1_FONT* simple_font, Region bounds) 
{
    if (! (
            0 <= bounds.x_start 
         && bounds.x_end <= frame_buffer->Width 
         && 0 <= bounds.y_start
         && bounds.y_end <= frame_buffer->Height
         && bounds.x_start < bounds.x_end
         && bounds.y_start < bounds.y_end
    ) ) {
        return; 
    } else {
        RendererBounds = bounds;
    }

    frameBuffer = frame_buffer;
    font = simple_font;
    CursorPosition = {0, 0};
    ready = true;
}


void BasicRenderer::_putChar(char chr, unsigned int xOff, unsigned int yOff, unsigned int color, unsigned int background) {
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


void BasicRenderer::printString(const char* string, Color color, AutoColor background) {
    if (!ready) return;
    unsigned long color_int = createRGBA(color);
    unsigned long background_int = background.UseDefault ? createRGBA(RegionBackgroundColor) : createRGBA(background.color);

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
                _putChar(' ', CursorPosition.x, CursorPosition.y, color_int, background_int);
                CursorPosition.x += font->font_size.w;
                trapCursor(); 
            }
        }
        else {
            _putChar(*chr, CursorPosition.x, CursorPosition.y, color_int, background_int);
            CursorPosition.x += font->font_size.w;
        }

        trapCursor();
        chr++;
    }
}


void BasicRenderer::printChar(const char character, Color color, AutoColor background) {
    if (!ready) return;
    unsigned long color_int = createRGBA(color);
    unsigned long background_int = background.UseDefault ? createRGBA(RegionBackgroundColor) : createRGBA(background.color);

    trapCursor();
    if (character == '\n') {
            CursorPosition.y += font->font_size.h;
    }
    else if (character == '\r') {
        CursorPosition.x = 0;
    }
    else if (character == '\t') {
        for (int i = 0; i < 4; i++) {
            _putChar(' ', CursorPosition.x, CursorPosition.y, color_int, background_int);
            CursorPosition.x += font->font_size.w;
            trapCursor(); 
        }
    }
    else {
        _putChar(character, CursorPosition.x, CursorPosition.y, color_int, background_int);
        CursorPosition.x += font->font_size.w;
    }
    trapCursor();
}


void BasicRenderer::putChar(char chr, unsigned int xOff, unsigned int yOff, Color color, Color background) {    
    if (!ready) return;
    if (font == NULL || frameBuffer == NULL) return;  // not ready to print yet.
    if (!(authorized(xOff, yOff) && authorized(xOff + font->font_size.w, yOff + font->font_size.h))) return;

    _putChar(chr, xOff, yOff, createRGBA(color), createRGBA(background));
}


void BasicRenderer::_clearRegion(Region region, unsigned int color) {
    unsigned int* pixPtr = (unsigned int*)frameBuffer->BaseAddress;
    for (unsigned int y = region.y_start; y < region.y_end; y++) {
        for (unsigned int x = region.x_start; x < region.x_end; x++) { 
            if (authorized(x, y)) 
                *(unsigned int*)(pixPtr + x + (y * frameBuffer->PixelsPerScanline)) = color;
        }   
    }
}


void BasicRenderer::clearRegion(Region region, AutoColor color) { 
    Color NColor;
    if (color.UseDefault) NColor = RegionBackgroundColor;
    else NColor = color.color;
    if (!ready) return;
    _clearRegion(region, createRGBA(NColor)); 
}

void BasicRenderer::clearScreen(Color color, bool ResetCursorPos) { 
    if (!ready) return;
    _clearRegion(RendererBounds, createRGBA(color));
    RegionBackgroundColor = color;
    if (ResetCursorPos) SetCursorPos(0, 0);
}

void BasicRenderer::_drawHLine(unsigned int x_start, unsigned int x_end, unsigned int y, unsigned int color, unsigned int stroke) {
    --stroke; 
     unsigned int* pixPtr = (unsigned int*)frameBuffer->BaseAddress;
     for (unsigned int x = x_start; x <= x_end; x++) {
        if (authorized(x, y)) {
            *(unsigned int*)(pixPtr + x + (y * frameBuffer->PixelsPerScanline)) = color;
        }
     }
}

void BasicRenderer::_drawVLine(unsigned int y_start, unsigned int y_end, unsigned int x, unsigned int color, unsigned int stroke) {
    --stroke; 
     unsigned int* pixPtr = (unsigned int*)frameBuffer->BaseAddress;
     for (unsigned int y = y_start; y <= y_end; y++) {
        if (authorized(x, y)) {
            *(unsigned int*)(pixPtr + x + (y * frameBuffer->PixelsPerScanline)) = color;
        }
     }
}

void BasicRenderer::_drawHRect(Geometry geo, unsigned int color) {
    if (geo.stroke_width <= 0) return;

    unsigned int* pixPtr = (unsigned int*)frameBuffer->BaseAddress;
        for (unsigned int x = geo.region.x_start; x <= geo.region.x_end; x++) {
            if (authorized(x, geo.region.y_start)) {
                *(unsigned int*)(pixPtr + x + (geo.region.y_start * frameBuffer->PixelsPerScanline)) = color;
            }
            if (authorized(x, geo.region.y_end)) {
                *(unsigned int*)(pixPtr + x + (geo.region.y_end * frameBuffer->PixelsPerScanline)) = color;
            }
        }
        for (unsigned int y = geo.region.y_start; y <= geo.region.y_end; y++) {

            if (authorized(geo.region.x_start, y)) {
                *(unsigned int*)(pixPtr + geo.region.x_start + (y * frameBuffer->PixelsPerScanline)) = color;
            }
            if (authorized(geo.region.x_end, y)) {
                *(unsigned int*)(pixPtr + geo.region.x_end + (y * frameBuffer->PixelsPerScanline)) = color;
            }
        }
}

void BasicRenderer::_drawFRect(Geometry geo, unsigned int color, unsigned int fill_color) {
    _clearRegion(geo.region, fill_color);
    if (geo.stroke_width > 0) _drawHRect(geo, color);
}

void BasicRenderer::drawHShape(Geometry geometry, Color color) {
    if (!ready) return;
    unsigned long color_int = createRGBA(color);

    if (geometry.shape == RECT) 
        _drawHRect(geometry, color_int);
    else if (geometry.shape == HORIZ_LINE) 
        _drawHLine(geometry.region.x_start, geometry.region.x_end, geometry.region.y_start, color_int, geometry.stroke_width);
    else if (geometry.shape == VERT_LINE) 
        _drawVLine(geometry.region.y_start, geometry.region.y_end, geometry.region.x_start, color_int, geometry.stroke_width);
}


void BasicRenderer::drawFShape(Geometry geometry, Color color, Color fill_color) {
    if (!ready) return;
    unsigned long color_int = createRGBA(color);
    unsigned long fill_color_int = createRGBA(fill_color);

    if (geometry.shape == RECT)
        _drawFRect(geometry, color_int, fill_color_int);
    else if (geometry.shape == HORIZ_LINE) 
        _drawHLine(geometry.region.x_start, geometry.region.x_end, geometry.region.y_start, color_int, geometry.stroke_width);
    else if (geometry.shape == VERT_LINE) 
        _drawVLine(geometry.region.y_start, geometry.region.y_end, geometry.region.x_start, color_int, geometry.stroke_width);
}
