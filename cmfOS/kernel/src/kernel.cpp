#include "kernelUtil.h"


extern "C" void _start(BootInfo* bootInfo) {
    KernelInfo kernelInfo = InitKernel(bootInfo);
    PageTableManager* PTM = kernelInfo.pageTableManager;
    Region KernelBounds = KernelRenderer.GetBounds();

    MenuRenderer.clearScreen(BLUE, true);
    KernelRenderer.clearScreen(WHITE, true);

    // ------------------------------------
    // NO MORE CODE UNDERNEATH THIS COMMENT
    // ------------------------------------

    while (true) {
        ProcessMousePacket();
    }
}

