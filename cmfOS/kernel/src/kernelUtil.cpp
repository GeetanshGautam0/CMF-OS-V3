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
void PrepInterrupts() {
    idtr.Limit = 0x0FFF;
    idtr.Offset = (uint64_t)KernelPageAllocator.RequestPage();

    IDTDescEntry* int_DivByZeroFault = (IDTDescEntry*)(idtr.Offset + 0x0 * sizeof(IDTDescEntry));
    int_DivByZeroFault->SetOffset((uint64_t)DivisionByZero_Handler);
    int_DivByZeroFault->type_attr = IDT_TA_InterruptGate;
    int_DivByZeroFault->selector = 0x08;

    IDTDescEntry* int_BoundsRangeExceeded = (IDTDescEntry*)(idtr.Offset + 0x5 * sizeof(IDTDescEntry));
    int_BoundsRangeExceeded->SetOffset((uint64_t)BoundRangeExceeded_Handler);
    int_BoundsRangeExceeded->type_attr = IDT_TA_InterruptGate;
    int_BoundsRangeExceeded->selector = 0x08;

    IDTDescEntry* int_InvalidOpcode = (IDTDescEntry*)(idtr.Offset + 0x6 * sizeof(IDTDescEntry));
    int_InvalidOpcode->SetOffset((uint64_t)InvalidOpcode_Handler);
    int_InvalidOpcode->type_attr = IDT_TA_InterruptGate;
    int_InvalidOpcode->selector = 0x08;

    IDTDescEntry* int_DeviceNotAvailable = (IDTDescEntry*)(idtr.Offset + 0x7 * sizeof(IDTDescEntry));
    int_DeviceNotAvailable->SetOffset((uint64_t)DeviceNotAvailable_Handler);
    int_DeviceNotAvailable->type_attr = IDT_TA_InterruptGate;
    int_DeviceNotAvailable->selector = 0x08;

    IDTDescEntry* int_DoubleFault = (IDTDescEntry*)(idtr.Offset + 0x8 * sizeof(IDTDescEntry));
    int_DoubleFault->SetOffset((uint64_t)DoubleFault_Handler);
    int_DoubleFault->type_attr = IDT_TA_InterruptGate;
    int_DoubleFault->selector = 0x08;

    IDTDescEntry* int_InvTTS = (IDTDescEntry*)(idtr.Offset + 0xA * sizeof(IDTDescEntry));
    int_InvTTS->SetOffset((uint64_t)SystemError_Handler);
    int_InvTTS->type_attr = IDT_TA_InterruptGate;
    int_InvTTS->selector = 0x08;
    
    IDTDescEntry* int_SegNotPres = (IDTDescEntry*)(idtr.Offset + 0xB * sizeof(IDTDescEntry));
    int_SegNotPres->SetOffset((uint64_t)SegmentNotPresent_Handler);
    int_SegNotPres->type_attr = IDT_TA_InterruptGate;
    int_SegNotPres->selector = 0x08;
    
    IDTDescEntry* int_StackSegFault = (IDTDescEntry*)(idtr.Offset + 0xC * sizeof(IDTDescEntry));
    int_StackSegFault->SetOffset((uint64_t)StackSegmentFault_Handler);
    int_StackSegFault->type_attr = IDT_TA_InterruptGate;
    int_StackSegFault->selector = 0x08;

    IDTDescEntry* int_GPFault = (IDTDescEntry*)(idtr.Offset + 0xD * sizeof(IDTDescEntry));
    int_GPFault->SetOffset((uint64_t)GPFault_Handler);
    int_GPFault->type_attr = IDT_TA_InterruptGate;
    int_GPFault->selector = 0x08;

    IDTDescEntry* int_PageFault = (IDTDescEntry*)(idtr.Offset + 0xE * sizeof(IDTDescEntry));
    int_PageFault->SetOffset((uint64_t)PageFault_Handler);
    int_PageFault->type_attr = IDT_TA_InterruptGate;
    int_PageFault->selector = 0x08;

    IDTDescEntry* int_SecurityException = (IDTDescEntry*)(idtr.Offset + 0x1E * sizeof(IDTDescEntry));
    int_SecurityException->SetOffset((uint64_t)SystemError_Handler);
    int_SecurityException->type_attr = IDT_TA_InterruptGate;
    int_SecurityException->selector = 0x08;

    IDTDescEntry* int_KB = (IDTDescEntry*)(idtr.Offset + 0x21 * sizeof(IDTDescEntry));
    int_KB->SetOffset((uint64_t)KeyboardInt_Handler);
    int_KB->type_attr = IDT_TA_InterruptGate;
    int_KB->selector = 0x08;

    asm ("lidt %0" : : "m" (idtr));

    RemapPIC();

    outb(PIC1_DATA, 0b11111101);
    outb(PIC2_DATA, 0b11111111);

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
    
    memset(bootInfo->frameBuffer->BaseAddress, 0, bootInfo->frameBuffer->BufferSize);

    kernelInit = true;
    return kernelInfo;
}
