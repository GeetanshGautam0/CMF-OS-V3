#pragma once
#include <stddef.h>
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
#include "input/mouse.h"
#include "acpi/acpi.h"
#include "pci/pci.h"
#include "memory/heap.h"
#include "scheduling/pit/pit.h"
#include "input/KB.h"
#include "renderer/panic.h"
#include "renderer/prompts/prompts_config.hpp"
#include "renderer/prompts/error_prompt.h"


extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

struct KernelInfo {
    PageTableManager::PageTableManager* pageTableManager;
    ACPI::MCFGHeader* mcfg;
};

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

KernelInfo InitKernel(Boot::BootInfo* bootInfo);
