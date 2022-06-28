#pragma once 
#include <stddef.h>
#include "renderer/KernelRenderer.h"
#include "renderer/MenuRenderer.h"
#include "std/ktypedef.h"       // Defines color mode in here
#include "std/cstr.h"
#include "paging/PageFrameAllocator.h"
#include "paging/PageMapIndexer.h"
#include "paging/PageTableManager.h"
#include "paging/paging.h"
#include "std/memory.h"
#include "gdt/gdt.h"
#include "interrupts/interrupts.h"
#include "interrupts/IDT.h"
#include "IO.h"


extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

struct KernelInfo {
    PageTableManager* pageTableManager;
};

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

KernelInfo InitKernel(BootInfo* bootInfo);
