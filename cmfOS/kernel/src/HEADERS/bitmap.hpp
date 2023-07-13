#pragma once
#include "std.hpp"


class Bitmap {
    public:
        size_t Size;
        uint8_t* Buffer;
        bool operator[](uint64_t index);  // can use like an array
        bool Set(uint64_t index, bool value);

    private:
        bool Get(uint64_t index);
};

