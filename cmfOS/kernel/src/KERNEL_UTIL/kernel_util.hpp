#pragma once
#include "../HEADERS/std.hpp"
#include "../HEADERS/ktypedef.hpp"


extern uint64_t _KernelStart; 
extern uint64_t _KernelEnd;


KERNEL::KernelInfo InitKernel(KERNEL::BootInfo* bootInfo);
