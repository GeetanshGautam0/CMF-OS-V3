#pragma once
#include "ctypedef.h"
#include "FrameBuffer.h" 
#include "math.h"
#include "SimpleFont.h"
#include "../efiMemory.h"
#include "cstr.h"


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

bool Colors_AreEqual(Color one, Color two);

unsigned long createRGBA(Color color);
unsigned long createRGBA(unsigned long color);
unsigned int createRGBA(unsigned int color);


const static Color RED  		= {0xff, 0x00, 0x00, 0x00};
const static Color BLUE  	= {0x00, 0x00, 0xff, 0xff};
const static Color GREEN 	= {0x00, 0xff, 0x00, 0x00};
const static Color BLACK 	= {0x00, 0x00, 0x00, 0x00};
const static Color MAGENTA  	= {0xff, 0x00, 0x00, 0xff};
const static Color CYAN  	= {0x00, 0xff, 0x00, 0xff};
const static Color WHITE 	= {0xff, 0xff, 0xff, 0xff};
const static Color YELLOW	= {0xff, 0xff, 0x00, 0x00};
