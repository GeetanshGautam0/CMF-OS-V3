#pragma once
#include "std.hpp"
#include "memory.hpp"


namespace Heap {
    struct HeapSegHdr {
        size_t length;
        HeapSegHdr* next;
        HeapSegHdr* last;
        bool free;
        void CombineForward();
        void CombineBackward();
        HeapSegHdr* Split(size_t splitLength);
    };

    void Init(void* heapAddress, size_t pageCount);
    void Expand(size_t length);
}

void* malloc(size_t size);
void free(void* address);

inline void* operator new(size_t size) {return malloc(size);}
inline void* operator new[](size_t size) {return malloc(size);}

inline void operator delete(void* p) {free(p);}
