#include "HEADERS/std.hpp"
#include "HEADERS/ktypedef.hpp"


namespace KernelMain {

    void _on_boot(KERNEL::BootInfo* bootInfo) 
    {

    }

    void _main(KERNEL::BootInfo* bootInfo) 
    {
        unsigned int y = 50;
        unsigned int BBP = 4;

        KERNEL::FrameBuffer* framebuffer = bootInfo->frameBuffer;
        KERNEL::SimpleFont::PSF1_FONT* psf1_font = bootInfo->psf1_font;

        for (unsigned int x = 0; x < framebuffer->Width /2 * BBP; x += BBP) {
            *(unsigned int*)(x + (y * framebuffer->PixelsPerScanLine * BBP) + framebuffer->BaseAddress) = 0x0000ffff;
        }
    }

}


extern "C" void _start(KERNEL::BootInfo* bootInfo)
{
    KernelMain::_on_boot(bootInfo);
    KernelMain::_main(bootInfo);

    return;
}