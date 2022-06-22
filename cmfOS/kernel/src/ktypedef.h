#pragma once
#include "ctypedef.h"
#include "FrameBuffer.h" 
#include "math.h"
#include "SimpleFont.h"
#include "efiMemory.h"


#define COLOR_MODE "BRGA"


struct BootInfo {
	FrameBuffer* frameBuffer;
	PSF1_FONT* psf1_font;
	EFI_MEMORY_DESCRIPTOR* mMap;
	uint64_t mMapSize;
	uint64_t mMapDescriptorSize;
};


struct Color {
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char A = 0xFF;
};


unsigned long createRGBA(Color color);


const Color RED  	= {0xff, 0x00, 0x00, 0x00};
const Color BLUE  	= {0x00, 0x00, 0xff, 0xff};
const Color GREEN 	= {0x00, 0xff, 0x00, 0x00};
const Color WHITE 	= {0xff, 0xff, 0xff, 0xff};
const Color CYAN  	= {0x00, 0xff, 0x00, 0xff};
const Color BLACK 	= {0x00, 0x00, 0x00, 0x00};
const Color YELLOW	= {0xff, 0xff, 0x00, 0x00};

