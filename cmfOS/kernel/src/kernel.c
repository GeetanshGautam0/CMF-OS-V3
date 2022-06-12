#include <stddef.h>

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

typedef struct {
	void* BaseAddress;
	size_t BufferSize;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixelsPerScanline;
} FrameBuffer;

typedef struct {
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charsize;
} PSF1_HEADER;


typedef struct {
	PSF1_HEADER* psf1_Header;
	void* glyphBuffer;
} PSF1_FONT;


typedef struct {
    unsigned int x; 
    unsigned int y; 
} Point;


Point CursorPosition;
FrameBuffer* __KERNEL_frameBuffer = NULL;
PSF1_FONT* __KERNEL_psf1Font = NULL;


void putChar(char chr, unsigned int xOff, unsigned int yOff, unsigned int color) {
    if (__KERNEL_psf1Font == NULL || __KERNEL_frameBuffer == NULL) return;  // not ready to print yet.

    unsigned int* pixPtr = (unsigned int*)__KERNEL_frameBuffer->BaseAddress;
    char* fontPtr = __KERNEL_psf1Font->glyphBuffer + (chr * __KERNEL_psf1Font->psf1_Header->charsize);
    for (unsigned long y = yOff; y < yOff + __KERNEL_psf1Font->psf1_Header->charsize; y++) {
        for (unsigned long x = xOff; x < xOff + 8; x++) {
            if ( (*fontPtr & (0b10000000 >> (x - xOff))) > 0 ) {
                *(unsigned int*)(pixPtr + x + (y * __KERNEL_frameBuffer->PixelsPerScanline)) = color;
            }
        }   
        fontPtr ++; 
    }

}


void putStr(char* string, unsigned int color) {
    char* chr = string;
    while (*chr != 0) {
        putChar(*chr, CursorPosition.x, CursorPosition.y, color);
        CursorPosition.x += 8;
        if (CursorPosition.x + 8 > __KERNEL_frameBuffer->Width) {
            CursorPosition.x = 0;
            CursorPosition.y += __KERNEL_psf1Font->psf1_Header->charsize;
        }
        chr++;
    }
}


void _start(FrameBuffer* frameBuffer, PSF1_FONT* psf1_font) {
    __KERNEL_psf1Font = psf1_font;
    __KERNEL_frameBuffer = frameBuffer;

    CursorPosition.x = 0;
    CursorPosition.y = 0;

    return;
}
