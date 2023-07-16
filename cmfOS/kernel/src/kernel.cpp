#include "HEADERS/std.hpp"
#include "HEADERS/ktypedef.hpp"
#include "KERNEL_UTIL/kernel_util.hpp"
#include "SH/sh_simple_renderer.hpp"


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

        SimpleRenderer SH_SR(
            boot_info->frameBuffer,
            boot_info->psf1_font,
            {0, 0, boot_info->frameBuffer->Width, boot_info->frameBuffer->Height},
            COLOR::VGA::BLUE    
        );

        if 
        (!(SH_SR.IsReady() && !SH_SR.IsLocked() && dr.IsSetup()))
        {
            unsigned int y = 50;
            unsigned int BBP = 4;
            uint32_t color = 0x00ff00ff;

            KERNEL::FrameBuffer* framebuffer = boot_info->frameBuffer;
            KERNEL::SimpleFont::PSF1_FONT* psf1_font = boot_info->psf1_font;

            for (unsigned int x = 0; x < framebuffer->Width /2 * BBP; x += BBP) {
                *(unsigned int*)(x + (y * framebuffer->PixelsPerScanLine * BBP) + framebuffer->BaseAddress) = color;
            }

            while(true);  // halt
        
        }
        
        const char* nrk = dr.GenerateID(999);
        SH_SR.PrintString(nrk, COLOR::VGA::YELLOW);

    }

}


extern "C" void 
_start(KERNEL::BootInfo * boot_info)
{
    KERNEL::KernelInfo kernel_info = KernelMain::_on_boot(boot_info);
    KernelMain::_main(kernel_info, boot_info);

    return;
}