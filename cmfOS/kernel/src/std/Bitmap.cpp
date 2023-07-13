#include "Bitmap.h"


bool Bitmap::operator[](uint64_t index) {
    return Get(index);
}

bool Bitmap::Get(uint64_t index) {
    if (index > Size * 8) return false;

    uint64_t ByteIndex = index / 8;
    uint8_t BitIndex = index % 8;
    uint8_t BitIndexer = 0b10000000 >> BitIndex;

    return ((Buffer[ByteIndex] & BitIndexer) > 0) ? true : false;
}

bool Bitmap::Set(uint64_t index, bool value) {
    if (index >= Size * 8) return false; 

    uint64_t ByteIndex = index / 8;
    uint8_t BitIndex = index % 8;
    uint8_t BitIndexer = 0b10000000 >> BitIndex;

    Buffer[ByteIndex] &= ~BitIndexer;
    if (value) Buffer[ByteIndex] |= BitIndexer;

    return true;
}

