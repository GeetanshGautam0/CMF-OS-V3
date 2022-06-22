#include "PageTableManager.h"
#include "PageMapIndexer.h"
#include <stdint.h>
#include "PageFrameAllocator.h"
#include "../memory.h"

PageTableManager::PageTableManager(PageTable* PML4Address) { this->PML4 = PML4Address; }


void PageTableManager::MapMemory(void* virtualMem, void* physMem) {
    PageMapIndexer indexer = PageMapIndexer((uint64_t)virtualMem);
    PageDirectoryEntry PDE;

    PDE = PML4->entries[indexer.PDP_i];
    PageTable* PDP;
    if (!PDE.Present) {
        PDP = (PageTable*)KernelPageAllocator.RequestPage();
        memset(PDP, 0, 0x1000);
        PDE.Address = (uint64_t)PDP >> 12;
        PDE.Present = true;
        PDE.ReadWrite = true;   // false = ROM
        PML4->entries[indexer.PDP_i] = PDE;
    }
    else {
        PDP = (PageTable*)((uint64_t)PDE.Address << 12);
    }

    PDE = PDP->entries[indexer.PD_i];
    PageTable* PD;
    if (!PDE.Present) {
        PD = (PageTable*)KernelPageAllocator.RequestPage();
        memset(PD, 0, 0x1000);
        PDE.Address = (uint64_t)PD >> 12;
        PDE.Present = true;
        PDE.ReadWrite = true;   // false = ROM
        PDP->entries[indexer.PD_i] = PDE;
    }
    else {
        PD = (PageTable*)((uint64_t)PDE.Address << 12);
    }

    PDE = PD->entries[indexer.PT_i];
    PageTable* PT;
    if (!PDE.Present) {
        PT = (PageTable*)KernelPageAllocator.RequestPage();
        memset(PT, 0, 0x1000);
        PDE.Address = (uint64_t)PT >> 12;
        PDE.Present = true;
        PDE.ReadWrite = true;   // false = ROM
        PD->entries[indexer.PT_i] = PDE;
    }
    else {
        PT = (PageTable*)((uint64_t)PDE.Address << 12);
    }

    PDE = PT->entries[indexer.P_i];
    PDE.Address = (uint64_t)physMem >> 12; 
    PDE.Present = true; 
    PDE.ReadWrite = true;

    PT->entries[indexer.P_i] = PDE;

} 
