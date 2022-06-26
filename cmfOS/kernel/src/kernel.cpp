#include "kernelUtil.h"

extern "C" void _start(BootInfo* bootInfo) {
    KernelInfo kernelInfo = InitKernel(bootInfo);
    PageTableManager* PTM = kernelInfo.pageTableManager;
    BasicRenderer KernelRenderer = kernelInfo.kernelRenderer;
    BasicRenderer MenuRenderer = kernelInfo.menuRenderer;

    MenuRenderer.clearScreen(BLUE, true);
    Region KernelRegion = KernelRenderer.GetBounds();
    MenuRenderer.printString("x_start -> x_end:\t", WHITE, {BLACK, true});
    MenuRenderer.printString(to_string((uint64_t)KernelRegion.x_start), WHITE, {BLACK, true}); 
    MenuRenderer.printString(" -> ", WHITE, {BLACK, true}); 
    MenuRenderer.printString(to_string((uint64_t)KernelRegion.x_end), WHITE, {BLACK, true});
    
    MenuRenderer.SetCursorPos(0, 1);
    MenuRenderer.printString("y_start -> y_end:\t", WHITE, {BLACK, true});
    MenuRenderer.printString(to_string((uint64_t)KernelRegion.y_start), WHITE, {BLACK, true}); 
    MenuRenderer.printString(" -> ", WHITE, {BLACK, true}); 
    MenuRenderer.printString(to_string((uint64_t)KernelRegion.y_end), WHITE, {BLACK, true});

    return;
}

