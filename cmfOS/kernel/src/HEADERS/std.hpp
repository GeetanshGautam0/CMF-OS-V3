#pragma once
// #include "efi_memory.hpp"

#ifndef _STD_HPP

    #define _STD_HPP 
    #include <stdbool.h>
    #include <stddef.h>
    #include <stdint.h>

#endif


#ifndef _KERNEL_TYPE_DEF
    #define _KERNEL_TYPE_DEF

    namespace KERNEL {

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
            void* mMap;
            uint64_t mMapSize;
            uint64_t mMapDescriptorSize;
            void* rsdp;
        };
        
    }

#endif