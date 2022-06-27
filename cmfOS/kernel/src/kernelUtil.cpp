#include "kernelUtil.h"
#include "std/cstr.h"
#include "gdt/gdt.h"

KernelInfo kernelInfo;
PageTableManager PTM = NULL;
bool kernelInit = false;

void PrepMem(BootInfo* bootInfo) {
    if (kernelInit) return;

    // Renderers
    BasicRenderer KernelRenderer;
    KernelRenderer.setup(bootInfo->frameBuffer,  bootInfo->psf1_font,  {0, bootInfo->frameBuffer->Width, (unsigned int)(bootInfo->psf1_font->font_size.h*2), bootInfo->frameBuffer->Height});
    BasicRenderer MenuRenderer;
    MenuRenderer.setup(bootInfo->frameBuffer, bootInfo->psf1_font, {0, bootInfo->frameBuffer->Width, 0, (unsigned int)(bootInfo->psf1_font->font_size.h*2)});

    // Kernel mem locking
    uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescriptorSize;
    KernelPageAllocator = PageFrameAllocator();
    KernelPageAllocator.ReadEFIMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescriptorSize);
    uint64_t KernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t KernelPages = KernelSize / 4096 + 1;
    KernelPageAllocator.LockPages(&_KernelStart, KernelPages);    

    // Mem mapping
    PageTable* PML4 = (PageTable*)KernelPageAllocator.RequestPage();
    memset(PML4, 0, 0x1000);
    PTM = PageTableManager(PML4);
    for (uint64_t t = 0; t < GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescriptorSize); t += 0x1000) 
    { 
        PTM.MapMemory((void*)t, (void*)t); 
    }
    uint64_t fbBase = (uint64_t)bootInfo->frameBuffer->BaseAddress;
    uint64_t fbSize = (uint64_t)bootInfo->frameBuffer->BufferSize + 0x1000;
    KernelPageAllocator.LockPages((void*)fbBase, fbSize / 0x1000 + 1);
    for (uint64_t t = fbBase; t < fbBase + fbSize; t += 4096) 
    { 
        PTM.MapMemory((void*)t, (void*)t); 
    }
    asm ("mov %0, %%cr3" : : "r" (PML4));
    
    // Set
    kernelInfo.pageTableManager = &PTM;
    kernelInfo.kernelRenderer = KernelRenderer;
    kernelInfo.menuRenderer = MenuRenderer;
}

KernelInfo InitKernel(BootInfo* bootInfo) {
    if (kernelInit) return kernelInfo;
    GDTDescriptor gdtDescriptor;
    gdtDescriptor.Size = sizeof(GDT) - 1;
    gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
    LoadGDT(&gdtDescriptor);
    PrepMem(bootInfo);

    memset (bootInfo->frameBuffer->BaseAddress, 0, bootInfo->frameBuffer->BufferSize);

    kernelInit = true;
    return kernelInfo;
}
