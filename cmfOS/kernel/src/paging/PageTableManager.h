#pragma once
#include "paging.h"


class PageTableManager {
    public:
        PageTableManager (PageTable* PML4Address);
        PageTable* PML4;
        void MapMemory(void* virtualMem, void* physMem);
};
