#pragma once
#include "paging.h"


namespace PageTableManager {

    class PageTableManager {
        public:
            PageTableManager (PageTable* PML4Address);
            PageTable* PML4;
            void MapMemory(void* virtualMemory, void* physMemory);
    };

    extern PageTableManager GlobalManager;

}