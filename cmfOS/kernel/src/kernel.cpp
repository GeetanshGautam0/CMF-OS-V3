#include "kernelUtil.h"


bool ANIMATE = false;


extern "C" void _start(BootInfo* bootInfo) {
    KernelInfo kernelInfo = InitKernel(bootInfo);
    PageTableManager* PTM = kernelInfo.pageTableManager;
    Region KernelBounds = KernelRenderer.GetBounds();

    MenuRenderer.clearScreen(BLUE, true);
    KernelRenderer.clearScreen(WHITE, true);

    // ------------------------------------
    // NO MORE CODE UNDERNEATH THIS COMMENT
    // ------------------------------------

    while (!ANIMATE); // If not animating, stop here.

    Region RECT_REGION;
    Color RECT_COLORS[7] = { RED, CYAN, GREEN, YELLOW, MAGENTA, BLACK, BLUE };
    Color RECT_COLOR;
    unsigned int RECT_WIDTH = 50;
    unsigned int RECT_HEIGHT = RECT_WIDTH;
    unsigned int acc;

    // Sample "animation"
    while (ANIMATE) {
        acc = 0;

        for (unsigned int x = KernelBounds.x_start; x <= KernelBounds.x_end - RECT_WIDTH; x += RECT_WIDTH) {
            RECT_COLOR = RECT_COLORS[acc % 7];
            MenuRenderer.SetCursorPos(0, 0); 
            MenuRenderer.printString(
                "SOURCE CODE: https://github.com/GeetanshGautam0/CMF-OS-V3",
                RECT_COLOR,
                {true, RECT_COLOR}
            );
            for (unsigned int y = KernelBounds.y_start; y <= KernelBounds.y_end - RECT_HEIGHT; y++) {
                RECT_REGION = {x, x + RECT_WIDTH, y, y + RECT_HEIGHT};
                KernelRenderer.drawFShape({RECT, RECT_REGION, 0}, WHITE, RECT_COLOR);    
                // ^^ is the same as saying KR.clearRegion(RECT, {false, MAGENTA}) since border width is 0

                for (int f = 0; f < 10e5; f++) continue; // Delay (don't have PID setup yet)
                KernelRenderer.clearRegion({
                    RECT_REGION.x_start,
                    RECT_REGION.x_end,
                    RECT_REGION.y_start - 1,
                    RECT_REGION.y_start,
                }, {true, RED}); // No red TRAIL should be visible; indicates an error
            }
            KernelRenderer.clearRegion(RECT_REGION, {true, RED}); // No red TRAIL should be visible; indicates an error
            
            ++ acc;
            if (acc > 201) acc = 0;
        }
    }

    return;
}

