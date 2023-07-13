#include "kernelUtil.h"


namespace __Kernel {

    #define __KernelDef 

    void _on_start(Boot::BootInfo* bootInfo, KernelInfo kernelInfo) {
        #ifndef __KernelInit 
            return;
        #endif

        PageTableManager::PageTableManager* PTM = kernelInfo.pageTableManager;
        PIT::SetDivisor(65535); 
        PCI::EnumeratePCI(kernelInfo.mcfg);    
        KeyboardDebugMode = KB_DEBUG_MODE::NONE;
    }

    void _setup_prompts(BasicRenderer::Renderer* br_ptr, FrameBuffer::FrameBuffer* fb, Font::PSF1_FONT* font) {
        PromptConfig::completeSetup(br_ptr, fb, font);
    }

    void _main(BasicRenderer::Renderer* br, Boot::BootInfo* bootInfo) {
        bool alt_state = true;
        uint32_t y = (uint32_t)(br->RendererBounds().yf - bootInfo->psf1_font->font_size.h);

        while (true) {
            br->SetCursorPosition({0, y});
            br->printString("__Kernel::_main() ", alt_state ? Color::GREEN : Color::BLUE);
            br->printString(dr.gen_key(rand()), Color::BLACK); 
            alt_state != alt_state;
            
            PIT::Sleep(100);
        }
    }

}

extern "C" void _start(Boot::BootInfo* bootInfo) {        
    KernelInfo kernelInfo = InitKernel(bootInfo);
    __Kernel::_on_start(bootInfo, kernelInfo);
    BasicRenderer::Renderer br(bootInfo->frameBuffer, bootInfo->psf1_font, {0, 0, bootInfo->frameBuffer->Width, bootInfo->frameBuffer->Height}, Color::WHITE);
    __Kernel::_setup_prompts(&br, bootInfo->frameBuffer, bootInfo->psf1_font);

    dr.rfRgn();

    for (uint32_t i = 0; i < NRegions; i++) {
        if (dr.Regions[i]._occ) {
            br.printString("\n\rRegionDefinition ", Color::BLACK);
            
            br.printString(to_string((uint64_t)i), Color::BLACK);
            br.printString(":\n\r\t* Region: {", Color::BLACK);
            
            br.printString(to_string((uint64_t)dr.Regions[i].region.x0), Color::RED);       br.printString(", ", Color::BLACK);
            br.printString(to_string((uint64_t)dr.Regions[i].region.y0), Color::GREEN);     br.printString(", ", Color::BLACK);
            br.printString(to_string((uint64_t)dr.Regions[i].region.xf), Color::RED);       br.printString(", ", Color::BLACK);
            br.printString(to_string((uint64_t)dr.Regions[i].region.yf), Color::GREEN);     
            br.printString("}\n\r\t* Key: ", Color::BLACK);
            br.printString(dr.Regions[i].key, Color::RED);
        }
    }

    __Kernel::_main(&br, bootInfo);

}
