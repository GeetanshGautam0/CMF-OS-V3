#pragma once
#include "std.hpp"
#include "efi_memory.hpp"


#ifndef _KERNEL_TYPEDEF
    #define _KERNEL_TYPEDEF

#endif


namespace KERNEL {

    struct KernelInfo {

    };

    struct FrameBuffer {
        void* BaseAddress;
        size_t BufferSize;
        unsigned int Width;
        unsigned int Height;
        unsigned int PixelsPerScanLine;
    };

    struct PSF1_HEADER {
        unsigned char magic[2];
        unsigned char mode;
        unsigned char charsize;
    };

    struct PSF1_FONT {
        PSF1_HEADER* psf1_Header;
        void* glyphBuffer;
    };

    struct BootInfo {
        FrameBuffer* frameBuffer;
        PSF1_FONT* psf1_font;
        EFI_MEMORY_DESCRIPTOR* mMap;
        uint64_t mMapSize;
        uint64_t mMapDescriptorSize;
        void* rsdp;
    };

}
