#include "../HEADERS/std.hpp"
#include "../HEADERS/page_frame_allocator.hpp"
#include "../HEADERS/page_table_manager.hpp"
#include "../HEADERS/page_map_indexer.hpp"
#include "../HEADERS/memory.hpp"


namespace PageTableManager {

    PageTableManager GlobalManager = NULL;

    PageTableManager::PageTableManager(PageTable* PML4Address){
        this->PML4 = PML4Address;
    }

    void PageTableManager::MapMemory(void* virtualMemory, void* physicalMemory){
        PageMapIndexer indexer = PageMapIndexer((uint64_t)virtualMemory);
        PageDirectoryEntry PDE;

        PDE = PML4->entries[indexer.PDP_i];
        PageTable* PDP;
        if (!PDE.GetFlag(PageTable_Flag::Present)){
            PDP = (PageTable*)PageFrameAllocator::KernelPageAllocator.RequestPage();
            memset(PDP, 0, 0x1000);
            PDE.SetAddress((uint64_t)PDP >> 12);
            PDE.SetFlag(PageTable_Flag::Present, true);
            PDE.SetFlag(PageTable_Flag::ReadWrite, true);
            PML4->entries[indexer.PDP_i] = PDE;
        }
        else
        {
            PDP = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
        }
        
        
        PDE = PDP->entries[indexer.PD_i];
        PageTable* PD;
        if (!PDE.GetFlag(PageTable_Flag::Present)){
            PD = (PageTable*)PageFrameAllocator::KernelPageAllocator.RequestPage();
            memset(PD, 0, 0x1000);
            PDE.SetAddress((uint64_t)PD >> 12);
            PDE.SetFlag(PageTable_Flag::Present, true);
            PDE.SetFlag(PageTable_Flag::ReadWrite, true);
            PDP->entries[indexer.PD_i] = PDE;
        }
        else
        {
            PD = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
        }

        PDE = PD->entries[indexer.PT_i];
        PageTable* PT;
        if (!PDE.GetFlag(PageTable_Flag::Present)){
            PT = (PageTable*)PageFrameAllocator::KernelPageAllocator.RequestPage();
            memset(PT, 0, 0x1000);
            PDE.SetAddress((uint64_t)PT >> 12);
            PDE.SetFlag(PageTable_Flag::Present, true);
            PDE.SetFlag(PageTable_Flag::ReadWrite, true);
            PD->entries[indexer.PT_i] = PDE;
        }
        else
        {
            PT = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
        }

        PDE = PT->entries[indexer.P_i];
        PDE.SetAddress((uint64_t)physicalMemory >> 12);
        PDE.SetFlag(PageTable_Flag::Present, true);
        PDE.SetFlag(PageTable_Flag::ReadWrite, true);
        PT->entries[indexer.P_i] = PDE;
    }

}