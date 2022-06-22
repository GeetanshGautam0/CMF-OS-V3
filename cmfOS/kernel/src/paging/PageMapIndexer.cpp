#include "PageMapIndexer.h"

PageMapIndexer::PageMapIndexer(uint64_t virtualAddr) {
    virtualAddr >>= 12;
    P_i = virtualAddr & 0x1ff;
    virtualAddr >>= 9;
    PT_i = virtualAddr & 0x1ff;
    virtualAddr >>= 9;
    PD_i = virtualAddr & 0x1ff;
    virtualAddr >>= 9;
    PDP_i = virtualAddr & 0x1ff;
}
