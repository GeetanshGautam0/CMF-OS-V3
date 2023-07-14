#include "kernel_util.hpp"
#include "../GDT/gdt.hpp"
#include "../HEADERS/io.hpp"
#include "../HEADERS/idt.hpp"
#include "../HEADERS/std.hpp"
#include "../HEADERS/str.hpp"
#include "../HEADERS/pit.hpp"
#include "../HEADERS/pci.hpp"
#include "../HEADERS/ahci.hpp"
#include "../HEADERS/math.hpp"
#include "../HEADERS/acpi.hpp"
#include "../HEADERS/code.hpp"
#include "../HEADERS/heap.hpp"
#include "../HEADERS/mouse.hpp"
#include "../HEADERS/paging.hpp"
#include "../HEADERS/memory.hpp"
#include "../HEADERS/bitmap.hpp"
#include "../HEADERS/pic_time.hpp"
#include "../HEADERS/keyboard.hpp"
#include "../HEADERS/ktypedef.hpp"
#include "../HEADERS/efi_memory.hpp"
#include "../HEADERS/interrupts.hpp"
#include "../HEADERS/page_map_indexer.hpp"
#include "../HEADERS/page_table_manager.hpp"
#include "../HEADERS/page_frame_allocator.hpp"


#ifndef _KERNEL_UTIL_HPP
    #define _KERNEL_UTIL_HPP
#endif


#ifndef _PANIC_HPP
    // Needed for interrupts.hpp
    void Panic(const char* str, bool lck, bool hlt) { return; }
#endif


static bool kernel_init = false;
static KERNEL::KernelInfo kernel_info;
static IDTR idtr;


void 
PrepareMemory(
    KERNEL::BootInfo* boot_info
) 
{
    if (kernel_init) return;

    // Kernel memory locking
    uint64_t memory_map_entries = boot_info->mMapSize / boot_info->mMapDescriptorSize;
    PageFrameAllocator::KernelPageAllocator = PageFrameAllocator::PageFrameAllocator();
    PageFrameAllocator::KernelPageAllocator.ReadEFIMemoryMap(boot_info->mMap, boot_info->mMapSize, boot_info->mMapDescriptorSize);
    uint64_t KernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t KernelPages = KernelSize / 4096 + 1;
    PageFrameAllocator::KernelPageAllocator.LockPages(&_KernelStart, KernelPages);

    // Memory mapping
    PageTable* PML4 = (PageTable*)PageFrameAllocator::KernelPageAllocator.RequestPage();
    memset(PML4, 0, 0x1000);
    PageTableManager::GlobalManager = PageTableManager::PageTableManager(PML4);
    for 
        (
            uint64_t t = 0;
            t < GetMemorySize(boot_info->mMap, memory_map_entries, boot_info->mMapDescriptorSize);
            t += 0x1000 
        )
    {
        PageTableManager::GlobalManager.MapMemory((void*)t, (void*)t);
    }

    uint64_t fbBase = (uint64_t)boot_info->frameBuffer->BaseAddress;
    uint64_t fbSize = (uint64_t)boot_info->frameBuffer->BufferSize + 0x1000;

    PageFrameAllocator::KernelPageAllocator.LockPages((void*)fbBase, fbSize / 0x1000 + 1);
    for
        (
            uint64_t t = fbBase;
            t < fbBase + fbSize;
            t += 4096
        )
    {
        PageTableManager::GlobalManager.MapMemory((void*)t, (void*)t);
    }

    asm ("mov %0, %%cr3" : : "r" (PML4));

    kernel_info.page_table_manager = &PageTableManager::GlobalManager;
}


void 
SetIDTGate(
    void* handler, 
    uint8_t entry_offset, 
    uint8_t type_attr,
    uint8_t selector
) 
{

    #ifndef _IDT_HPP
        return;
    #endif

    IDTDescEntry* interrupt = (IDTDescEntry*)(idtr.Offset + entry_offset * sizeof(IDTDescEntry));
    interrupt->SetOffset((uint64_t)handler);
    interrupt->type_attr = type_attr;
    interrupt->selector = selector;

}


void 
PrepareInterrupts()
{
    idtr.Limit = 0x0FFF;
    idtr.Offset = (uint64_t)PageFrameAllocator::KernelPageAllocator.RequestPage();

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
    SetIDTGate((void*)MouseInt_Handler,             0x2C,                       IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)PITInt_Handler,               0x20,                       IDT_TA_InterruptGate, 0x08);

    asm ("lidt %0" : : "m" (idtr));
    RemapPIC();
    asm("sti");
}


void PrepareACPI(KERNEL::BootInfo* boot_info) {
    ACPI::SDTHeader*    xsdt = (ACPI::SDTHeader*) (boot_info->rsdp->XSDTAddress);
    ACPI::MCFGHeader*   mcfg = (ACPI::MCFGHeader*)ACPI::FindTable(xsdt, (char*)"MCFG");

    kernel_info.mcfg = mcfg;

    PCI::EnumeratePCI(mcfg);
}


KERNEL::KernelInfo 
InitKernel(
    KERNEL::BootInfo* boot_info
) 
{
    if (kernel_init) return kernel_info;

    GDTDescriptor gdtDescriptor;
    gdtDescriptor.Size = sizeof(GDT) - 1;
    gdtDescriptor.Offset = (uint64_t)&DefaultGDT;

    LoadGDT(&gdtDescriptor);
    PrepareMemory(boot_info);
    Heap::Init((void*)0x0000100000000000, 0x10);
    PrepareInterrupts();
    InitPS2Mouse();
    PrepareACPI(boot_info);

    outb(PIC1_DATA, 0b11111000);
    outb(PIC2_DATA, 0b11101111);

    // memset(boot_info->frameBuffer->BaseAddress, 0, boot_info->frameBuffer->BufferSize);

    kernel_init = true;
    return kernel_info;
}