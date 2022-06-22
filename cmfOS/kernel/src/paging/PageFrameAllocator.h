#pragma once
#include "../ctypedef.h"
#include "../ktypedef.h"
#include "../Bitmap.h"
#include "../memory.h"


class PageFrameAllocator {
    public:
        void ReadEFIMemoryMap(EFI_MEMORY_DESCRIPTOR* mMap, size_t mMapSize, size_t mMapDescSize);
        void FreePage(void* addr);
        void FreePages(void* addr, uint64_t pageCount);
        void LockPage(void* addr);
        void LockPages(void* addr, uint64_t pageCount);
        uint64_t GetFreeRAM();
        uint64_t GetUsedRAM();
        uint64_t GetReservedRAM();
        void* RequestPage();
        Bitmap PageBitmap;
    
    private:
        void initBitmap(size_t bitmapSize, void* bufferAddr);
        void ReservePage(void* addr);
        void ReservePages(void* addr, uint64_t pageCount);
        void UnreservePage(void* addr);
        void UnreservePages(void* addr, uint64_t pageCount);
};


extern PageFrameAllocator KernelPageAllocator;
