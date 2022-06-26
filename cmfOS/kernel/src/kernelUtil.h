#pragma once 
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

struct KernelInfo {
    PageTableManager* pageTableManager;
    BasicRenderer kernelRenderer;
    BasicRenderer menuRenderer;
};

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

KernelInfo InitKernel(BootInfo* bootInfo);
