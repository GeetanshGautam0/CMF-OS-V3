#include <stddef.h>
#include "BasicRenderer.h"
#include "cstr.h"
#include "ctypedef.h"
#include "ktypedef.h"
#include "efiMemory.h"


extern "C" void _start(BootInfo* bootInfo) {
    BasicRenderer KernelRenderer = BasicRenderer(
        bootInfo->frameBuffer, 
        bootInfo->psf1_font, 
        {0, bootInfo->frameBuffer->Width, 0, bootInfo->frameBuffer->Height}
    );
    
    KernelRenderer.clearScreen(0xff001100);

    uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescriptorSize;
    for (int i = 0; i < mMapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)bootInfo->mMap + (i * bootInfo->mMapDescriptorSize));
        KernelRenderer.printString(EFI_MEMORY_TYPE_STRINGS[desc->type]);
        KernelRenderer.printString(" ");
        KernelRenderer.printString(to_string(desc->numPages*4096/1024), 0xffff0000);
        KernelRenderer.printString("KB");
        KernelRenderer.CursorPosition = {0, KernelRenderer.CursorPosition.y + KernelRenderer.font->psf1_Header->charsize};
    }
    
    KernelRenderer.CursorPosition = {0, 0};
    KernelRenderer.printString(to_string((uint64_t)KernelRenderer.RendererBounds.x_start), 0xffffffff, 0x00ff0031);
    KernelRenderer.printString(to_string((uint64_t)KernelRenderer.RendererBounds.x_end), 0xffffffff, 0xff00ff00);
    KernelRenderer.printString(to_string((uint64_t)KernelRenderer.RendererBounds.y_start), 0xffffffff, 0x00ff0031);
    KernelRenderer.printString(to_string((uint64_t)KernelRenderer.RendererBounds.y_end), 0xffffffff, 0xff00ff00);

    KernelRenderer.drawHRect({600, 900, 100, 1000});
    KernelRenderer.drawFRect({650, 870, 130, 990});

    return;
}

