#pragma once
#include "std.hpp"
#include "efi_memory.hpp"
#include "page_table_manager.hpp"
#include "acpi.hpp"


#ifndef _KERNEL_TYPEDEF
    #define _KERNEL_TYPEDEF

    #ifndef __COLOR_MODE
        #define __COLOR_MODE
        #define __COLOR_MODE_BRGA

    #endif

    #ifndef __COLOR
        #define __COLOR

        namespace VGA_COLOR {
            struct Color {
                unsigned char R;
                unsigned char G;
                unsigned char B;
                unsigned char A = 0xFF;
            };

            bool 			AreEqual(Color one, Color two);
            bool operator	==(Color one, Color two);
            unsigned long 	createRGBA(unsigned long color);
            unsigned int 	createRGBA(unsigned int color);
            unsigned long 	createRGBA(Color color);

            namespace VGA {

                const static Color RED  		= {0xff, 0x00, 0x00, 0x00};
                const static Color BLUE  		= {0x00, 0x00, 0xff, 0xff};
                const static Color GREEN 		= {0x00, 0xff, 0x00, 0x00};
                const static Color BLACK 		= {0x00, 0x00, 0x00, 0x00};
                const static Color MAGENTA  	= {0xff, 0x00, 0x00, 0xff};
                const static Color CYAN  		= {0x00, 0xff, 0x00, 0xff};
                const static Color WHITE 		= {0xff, 0xff, 0xff, 0xff};
                const static Color YELLOW		= {0xff, 0xff, 0x00, 0x00};    

            }
        }

    #endif

#endif


namespace KERNEL {

    namespace SimpleFont {
        
        struct FontSize {
            unsigned char w;
            unsigned char h;
        };

        struct PSF1_HEADER {
            unsigned char magic[2];
            unsigned char mode;
            unsigned char charsize;
        };

        struct PSF1_FONT {
            PSF1_HEADER* psf1_Header;
            void* glyphBuffer;
            FontSize font_size;
        };

    }

    struct KernelInfo {
        PageTableManager::PageTableManager* page_table_manager;
        ACPI::MCFGHeader* mcfg;
    };

    struct FrameBuffer {
        void* BaseAddress;
        size_t BufferSize;
        unsigned int Width;
        unsigned int Height;
        unsigned int PixelsPerScanLine;
    };

    struct BootInfo {
        FrameBuffer* frameBuffer;
        SimpleFont::PSF1_FONT* psf1_font;
        EFI_MEMORY_DESCRIPTOR* mMap;
        uint64_t mMapSize;
        uint64_t mMapDescriptorSize;
        ACPI::RSDP2* rsdp;
    };

}
