#include "kernelUtil.h"

KernelInfo kernelInfo;
PageTableManager PTM = NULL;
bool kernelInit = false;

void PrepMem(BootInfo* bootInfo) {
    if (kernelInit) return;
    // Renderers
    KernelRenderer.setup(bootInfo->frameBuffer, bootInfo->psf1_font, {0, bootInfo->frameBuffer->Width, (unsigned int)(bootInfo->psf1_font->font_size.h*2), bootInfo->frameBuffer->Height});
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
}

IDTR idtr;

void SetIDTGate(void* handler, uint8_t entryOffset, uint8_t type_attr, uint8_t selector) {
    IDTDescEntry* interrupt = (IDTDescEntry*)(idtr.Offset + entryOffset * sizeof(IDTDescEntry));
    interrupt->SetOffset((uint64_t)handler);
    interrupt->type_attr = type_attr;
    interrupt->selector = selector;
}

void PrepInterrupts() {
    idtr.Limit = 0x0FFF;
    idtr.Offset = (uint64_t)KernelPageAllocator.RequestPage();

    SetIDTGate((void*)DivisionByZero_Handler,       DivByZero_Offset,           IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)BoundRangeExceeded_Handler,   BoundRangeExceeded_Offset,  IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)InvalidOpcode_Handler,        InvalidOpcode_Offset,       IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)DeviceNotAvailable_Handler,   DeviceNotAvailable_Offset,  IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)DoubleFault_Handler,          DoubleFault_Offset,         IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)SystemError_Handler,          InvTTS_Offset,              IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)SegmentNotPresent_Handler,    SegmentNotPresent_Offset,   IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)StackSegmentFault_Handler,    StackSegmentFault_Offset,   IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)GPFault_Handler,              GPFault_Offset,             IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)PageFault_Handler,            PageFault_Offset,           IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)SystemError_Handler,          SecurityException_Offset,   IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)KeyboardInt_Handler,          0x21,                       IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)MouseInt_Handler,              0x2C,                       IDT_TA_InterruptGate, 0x08);

    asm ("lidt %0" : : "m" (idtr));

    RemapPIC();

    asm ("sti");
}

KernelInfo InitKernel(BootInfo* bootInfo) {
    if (kernelInit) return kernelInfo;
    GDTDescriptor gdtDescriptor;
    gdtDescriptor.Size = sizeof(GDT) - 1;
    gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
    
    LoadGDT(&gdtDescriptor);
    PrepMem(bootInfo);
    PrepInterrupts();
    InitPS2Mouse();

    outb(PIC1_DATA, 0b11111001);
    outb(PIC2_DATA, 0b11101111);

    memset(bootInfo->frameBuffer->BaseAddress, 0, bootInfo->frameBuffer->BufferSize);

    kernelInit = true;
    return kernelInfo;
}
