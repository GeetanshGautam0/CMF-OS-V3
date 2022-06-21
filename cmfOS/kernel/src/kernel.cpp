#include <stddef.h>
#include "BasicRenderer.h"
#include "ktypedef.h"       // Defines color mode in here
#include "cstr.h"


extern "C" void _start(BootInfo* bootInfo) {
    BasicRenderer KernelRenderer = BasicRenderer(
        bootInfo->frameBuffer, 
        bootInfo->psf1_font, 
        {0, bootInfo->frameBuffer->Width, (unsigned int)(bootInfo->psf1_font->font_size.h*2), bootInfo->frameBuffer->Height}
    );
    
    BasicRenderer MenuRenderer = BasicRenderer( 
        bootInfo->frameBuffer,
        bootInfo->psf1_font,
        {0, bootInfo->frameBuffer->Width, 0, (unsigned int)(bootInfo->psf1_font->font_size.h*2)}
    );

    KernelRenderer.clearScreen(WHITE);

    uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescriptorSize;
    for (int i = 0; i < mMapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)bootInfo->mMap + (i * bootInfo->mMapDescriptorSize));
        KernelRenderer.printString(EFI_MEMORY_TYPE_STRINGS[desc->type]);
        KernelRenderer.printString(" ");
        KernelRenderer.printString(to_string(desc->numPages*4096/1024), YELLOW);
        KernelRenderer.printString("KB", RED);
        KernelRenderer.CursorPosition = {0, KernelRenderer.CursorPosition.y + KernelRenderer.font->psf1_Header->charsize};
    }

    // Testing Renderer
    KernelRenderer.drawHShape({RECT, {600, 900, 100, 1000}, 1});
    KernelRenderer.drawFShape({RECT, {650, 870, 130, 990}, 1});
    KernelRenderer.drawFShape({HORIZ_LINE, {100, 870, 500, (unsigned int)NULL}, 1});
    KernelRenderer.drawHShape({VERT_LINE, {200, (unsigned int)NULL, 100, 1000}, 1});
    
    // Tests
    KernelRenderer.CursorPosition = {500, 500};
    KernelRenderer.printString(to_string((uint64_t)KernelRenderer.RendererBounds.x_start),  BLACK, YELLOW);
    KernelRenderer.printString(to_string((uint64_t)KernelRenderer.RendererBounds.x_end),    WHITE, RED);
    KernelRenderer.printString(to_string((uint64_t)KernelRenderer.RendererBounds.y_start),  BLACK, YELLOW);
    KernelRenderer.printString(to_string((uint64_t)KernelRenderer.RendererBounds.y_end),    WHITE, RED);
    KernelRenderer.printString(to_string(authorized(KernelRenderer.RendererBounds, 0, 0)),  BLACK, YELLOW);
    KernelRenderer.printString(to_string((uint64_t)clamp(0.0, -1, 10.0)),                   WHITE, RED);
    KernelRenderer.printString(to_string((uint64_t)clamp(0.0, 5, 10.0)),                    BLACK, YELLOW);
    KernelRenderer.printString(to_string((uint64_t)clamp(0.0, 11, 10.0)),                   WHITE, RED);
    KernelRenderer.printString(to_string((uint64_t)clamp(0.0, 10, 10.0)),                   BLACK, YELLOW);

    MenuRenderer.clearScreen(BLUE);

    return;
}

