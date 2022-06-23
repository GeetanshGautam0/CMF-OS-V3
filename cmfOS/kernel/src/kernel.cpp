#include <stddef.h>
#include "BasicRenderer.h"
#include "ktypedef.h"       // Defines color mode in here
#include "cstr.h"
#include "paging/PageFrameAllocator.h"
#include "paging/PageMapIndexer.h"
#include "paging/PageTableManager.h"
#include "paging/paging.h"
#include "memory.h"


extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;


extern "C" void _start(BootInfo* bootInfo) {
    KernelPageAllocator = PageFrameAllocator();
    KernelPageAllocator.ReadEFIMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescriptorSize);

    uint64_t KernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t KernelPages = KernelSize / 4096 + 1;
    KernelPageAllocator.LockPages(&_KernelStart, KernelPages);

    uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescriptorSize;

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

    KernelRenderer.clearScreen(BLACK);

    PageTable* PML4 = (PageTable*)KernelPageAllocator.RequestPage();
    memset(PML4, 0, 0x1000);

    PageTableManager PTM = PageTableManager(PML4);
    for (uint64_t t = 0; t < GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescriptorSize); t += 0x1000) {
        PTM.MapMemory((void*)t, (void*)t);
    }

    uint64_t fbBase = (uint64_t)bootInfo->frameBuffer->BaseAddress;
    uint64_t fbSize = (uint64_t)bootInfo->frameBuffer->BufferSize + 0x1000;
    for (uint64_t t = fbBase; t < fbBase + fbSize; t += 4096) {
        PTM.MapMemory((void*)t, (void*)t);
    }

    asm ("mov %0, %%cr3" : : "r" (PML4));

    MenuRenderer.clearScreen(BLUE);
    KernelRenderer.drawFShape({RECT, {100, 200, 300, 400}, 1}, WHITE, GREEN);
    KernelRenderer.drawFShape({RECT, {125, 175, 325, 375}, 1}, WHITE, BLACK);

    unsigned char i, j, k;
    while (true) {
        i = 0xa0; // why doens't the for loop does this automatically? 
        for (i = 0xa0; i < 0xff; i += 0x01) {
            j = 0xa0; // why doens't the for loop does this automatically? 
            for (j = 0xa0; j < 0xff; j += 0x01) {
                k = 0xa0; // why doens't the for loop does this automatically? 
                for (k = 0xa0; k < 0xff; k += 0x01) {
                    MenuRenderer.SetCursorPos(0, 0);
                    KernelRenderer.SetCursorPos(0, 0);
                    MenuRenderer.printString("https://github.com/GeetanshGautam0/CMF-OS-V3", {i, j, k, 0xff}, BLUE);
                    KernelRenderer.printString(to_hstring(i), RED, YELLOW);
                    KernelRenderer.printString(to_hstring(j), YELLOW, RED);
                    KernelRenderer.printString(to_hstring(k), RED, YELLOW);
                }
            }
        }
    }
    return;
}

