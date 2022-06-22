#include "Bitmap.h"


bool Bitmap::operator[](uint64_t index) {
    uint64_t ByteIndex = index / 8;
    uint8_t BitIndex = index % 8;
    uint8_t BitIndexer = 0b10000000 >> BitIndex;

    return ((Buffer[ByteIndex] & BitIndexer) > 0) ? true : false;
}

void Bitmap::Set(uint64_t index, bool value) {
    uint64_t ByteIndex = index / 8;
    uint8_t BitIndex = index % 8;
    uint8_t BitIndexer = 0b10000000 >> BitIndex;

    Buffer[ByteIndex] &= ~BitIndexer;
    if (value) Buffer[ByteIndex] |= BitIndexer;
}

