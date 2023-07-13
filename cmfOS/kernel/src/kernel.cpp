#include "HEADERS/std.hpp"


void _start(KERNEL::BootInfo* bootInfo)
{
    unsigned int y = 50;
    unsigned int BBP = 4;

	KERNEL::FrameBuffer* framebuffer = bootInfo->frameBuffer;
	KERNEL::PSF1_FONT* psf1_font = bootInfo->psf1_font;

    for (unsigned int x = 0; x < framebuffer->Width /2 * BBP; x += BBP) {
        *(unsigned int*)(x + (y * framebuffer->PixelsPerScanLine * BBP) + framebuffer->BaseAddress) = 0xff00ffff;
    }

    return;
}