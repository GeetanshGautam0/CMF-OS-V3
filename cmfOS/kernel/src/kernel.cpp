#include <stddef.h>
#include "BasicRenderer.h"
#include "ktypedef.h"       // Defines color mode in here
#include "cstr.h"
#include "PageFrameAllocator.h"


extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;


extern "C" void _start(BootInfo* bootInfo) {
    PageFrameAllocator KernelAllocator;
    KernelAllocator.ReadEFIMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescriptorSize);

    uint64_t KernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t KernelPages = KernelSize / 4096 + 1;
    KernelAllocator.LockPages(&_KernelStart, KernelPages);
    
    BasicRenderer KernelRenderer = BasicRenderer(
        bootInfo->frameBuffer, 
        bootInfo->psf1_font, 
        {0, bootInfo->frameBuffer->Width, (unsigned int)(bootInfo->psf1_font->font_size.h*2), bootInfo->frameBuffer->Height}
    );
    
    BasicRenderer MenuRenderer = BasicRenderer( 
        bootInfo->frameBuffer,
        bootInfo->psf1_font,
        {0, bootInfo->frameBuffer->Width, 0, (unsigned int)(bootInfo->psf1_font->font_size.h*2)}
    );

    KernelRenderer.clearScreen(WHITE);
    MenuRenderer.clearScreen(BLUE);
    MenuRenderer.CursorPosition = {0, 0};
    MenuRenderer.printString("6");

    KernelRenderer.CursorPosition = {0, KernelRenderer.CursorPosition.y + 16};
    KernelRenderer.printString("Free RAM: ");  KernelRenderer.printString(to_string(KernelAllocator.GetFreeRAM() / 1024)); KernelRenderer.printString("KB");
    KernelRenderer.CursorPosition = {0, KernelRenderer.CursorPosition.y + 16};
    KernelRenderer.printString("Used RAM: ");  KernelRenderer.printString(to_string(KernelAllocator.GetUsedRAM() / 1024)); KernelRenderer.printString("KB");
    KernelRenderer.CursorPosition = {0, KernelRenderer.CursorPosition.y + 16};
    KernelRenderer.printString("Reserved RAM: ");  KernelRenderer.printString(to_string(KernelAllocator.GetReservedRAM() / 1024)); KernelRenderer.printString("KB");

    for (int t = 0; t < 20; t++) {
        void* addr = KernelAllocator.RequestPage(); 
        KernelRenderer.CursorPosition = {0, KernelRenderer.CursorPosition.y + 16};
        KernelRenderer.printString(to_hstring((uint64_t)addr));
    }



    return;
}

