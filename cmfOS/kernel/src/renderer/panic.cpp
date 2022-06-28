#include "panic.h"
#include "KernelRenderer.h"
#include "MenuRenderer.h"
#include "../std/ctypedef.h"
#include "../std/ktypedef.h"

void Panic (const char* panicMessage, bool LR, bool halt) {
    KernelRenderer.clearScreen(WHITE);
    MenuRenderer.clearScreen(RED);

    MenuRenderer.printString("Panic!", WHITE, {true, MAGENTA});
    KernelRenderer.printString("Kernel Panic:\n\n\r\t", BLACK, { true, WHITE });
    KernelRenderer.printString(panicMessage, BLACK, { true, WHITE });

    if (LR) {
        MenuRenderer.Lock();
        KernelRenderer.Lock();
    }
    
    while (halt);
}
