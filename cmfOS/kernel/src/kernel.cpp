#include "HEADERS/std.hpp"
#include "HEADERS/ktypedef.hpp"
#include "KERNEL_UTIL/kernel_util.hpp"


namespace KernelMain {

    KERNEL::KernelInfo 
    _on_boot(KERNEL::BootInfo* boot_info) 
    {
        KERNEL::KernelInfo kernel_info;  

        #if defined(_KERNEL_UTIL_HPP)
            return InitKernel(boot_info);
        #else
            return kernel_info;
        #endif

    }

    void 
    _main(KERNEL::KernelInfo kernel_info, KERNEL::BootInfo* boot_info) 
    {
        unsigned int y = 50;
        unsigned int BBP = 4;

        KERNEL::FrameBuffer* framebuffer = boot_info->frameBuffer;
        KERNEL::SimpleFont::PSF1_FONT* psf1_font = boot_info->psf1_font;

        for (unsigned int x = 0; x < framebuffer->Width /2 * BBP; x += BBP) {
            *(unsigned int*)(x + (y * framebuffer->PixelsPerScanLine * BBP) + framebuffer->BaseAddress) = 0xff00ffff;
        }
    }

}


extern "C" void 
_start(KERNEL::BootInfo * boot_info)
{
    KERNEL::KernelInfo kernel_info = KernelMain::_on_boot(boot_info);
    KernelMain::_main(kernel_info, boot_info);

    return;
}