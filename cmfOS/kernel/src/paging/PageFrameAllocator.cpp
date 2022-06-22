#include "PageFrameAllocator.h"


uint64_t freeMemory;
uint64_t reservedMemory;
uint64_t usedMemory;
bool init = false; 
PageFrameAllocator KernelPageAllocator;

uint64_t PageFrameAllocator::GetFreeRAM() { return freeMemory; }
uint64_t PageFrameAllocator::GetUsedRAM() { return usedMemory; }
uint64_t PageFrameAllocator::GetReservedRAM() { return reservedMemory; }

void PageFrameAllocator::FreePage(void* addr) {
    uint64_t index = (uint64_t)addr / 4096;
    if (PageBitmap[index] == false) return;
    PageBitmap.Set(index, false);
    freeMemory += 4096;
    usedMemory -= 4096;
}

void PageFrameAllocator::LockPage(void* addr) {
    uint64_t index = (uint64_t)addr / 4096;
    if (PageBitmap[index] == true) return;
    PageBitmap.Set(index, true);
    freeMemory -= 4096;
    usedMemory += 4096;
}

void PageFrameAllocator::UnreservePage(void* addr) {
    uint64_t index = (uint64_t)addr / 4096;
    if (PageBitmap[index] == false) return;
    PageBitmap.Set(index, false);
    freeMemory += 4096;
    reservedMemory -= 4096;
}

void PageFrameAllocator::ReservePage(void* addr) {
    uint64_t index = (uint64_t)addr / 4096;
    if (PageBitmap[index] == true) return;
    PageBitmap.Set(index, true);
    freeMemory -= 4096;
    reservedMemory += 4096;
}


void PageFrameAllocator::FreePages(void* addr, uint64_t pageCount) {
    for (int t = 0; t < pageCount; t++) {
        FreePage((void*)((uint64_t)addr + (t * 4096)));
    }
}

void PageFrameAllocator::LockPages(void* addr, uint64_t pageCount) {
    for (int t = 0; t < pageCount; t++) {
        LockPage((void*)((uint64_t)addr + (t * 4096)));
    }
}

void PageFrameAllocator::ReservePages(void* addr, uint64_t pageCount) {
    for (int t = 0; t < pageCount; t++) {
        ReservePage((void*)((uint64_t)addr + (t * 4096)));
    }
}

void PageFrameAllocator::UnreservePages(void* addr, uint64_t pageCount) {
    for (int t = 0; t < pageCount; t++) {
        UnreservePage((void*)((uint64_t)addr + (t * 4096)));
    }
}

void PageFrameAllocator::ReadEFIMemoryMap(EFI_MEMORY_DESCRIPTOR* mMap, size_t mMapSize, size_t mMapDescSize) {
    if (init) return;
    init = true;
    uint64_t mMapEntries = mMapSize / mMapDescSize;
    void* largestFreeMemSeg = NULL;
    size_t largestFreeMemSegSize = 0;

    for (int i = 0; i < mMapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + (i * mMapDescSize));
        if (desc->type == 7) {  // EFI_CONVENTIAL_MEMORY
            if (desc->numPages * 4096 > largestFreeMemSegSize) {
                largestFreeMemSeg = desc->physAddr;
                largestFreeMemSegSize = desc->numPages * 4096;
            }
        }
    }


    uint64_t memSize = GetMemorySize(mMap, mMapEntries, mMapDescSize); 
    freeMemory = memSize;

    uint64_t bitmapSize = memSize / 4096 / 8 + 1;

    // Init bitmap
    initBitmap(bitmapSize, largestFreeMemSeg);

    // lock pages of bitmap
    LockPages(&PageBitmap, PageBitmap.Size / 4096 + 1);
    for (int i = 0; i < mMapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + (i * mMapDescSize));
        if (desc->type != 7) {
            ReservePages(desc->physAddr, desc->numPages);
        }
    }

    // reserve pages of unusable / reserved mem
}


void PageFrameAllocator::initBitmap(size_t bitmapSize, void* bufferAddr) {
    PageBitmap.Size = bitmapSize;
    PageBitmap.Buffer = (uint8_t*)bufferAddr;
    for (int i = 0; i < bitmapSize; i ++) {
        *(uint8_t*)(PageBitmap.Buffer + i) = 0;
    }
}

void* PageFrameAllocator::RequestPage() {
    for (uint64_t index = 0; index < PageBitmap.Size * 8; index ++) {
        if (PageBitmap[index] == true) continue;
        LockPage((void*)(index * 4096));
        return (void*)(index * 4096);
    }

    return NULL;  // Page frame swap to file
}

