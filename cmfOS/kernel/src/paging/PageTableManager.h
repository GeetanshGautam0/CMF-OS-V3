#pragma once
#include "paging.h"
#include "../BasicRenderer.h"


class PageTableManager {
    public:
        BasicRenderer DebugRenderer;
        PageTableManager (PageTable* PML4Address);
        PageTable* PML4;
        void MapMemory(void* virtualMemory, void* physMemory);
};
