#pragma once
#include "ctypedef.h"
#include "FrameBuffer.h"
#include "math.h"
#include "SimpleFont.h"
#include "../efiMemory.h"
#include "cstr.h"
#include "../acpi/acpi.h"
#include "../scheduling/pit/pit.h"


#ifndef KERNEL__COLOR_MODE

	#define __COLOR_MODE_BRGA
	#define KERNEL__COLOR_MODE

#endif


namespace Boot {

	struct BootInfo {
		FrameBuffer::FrameBuffer* frameBuffer;
		Font::PSF1_FONT* psf1_font;
		EFI_MEMORY_DESCRIPTOR* mMap;
		uint64_t mMapSize;
		uint64_t mMapDescriptorSize;
		ACPI::RSDP2* rsdp;
	};

}

namespace Color {
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

	const static Color RED  		= {0xff, 0x00, 0x00, 0x00};
	const static Color BLUE  		= {0x00, 0x00, 0xff, 0xff};
	const static Color GREEN 		= {0x00, 0xff, 0x00, 0x00};
	const static Color BLACK 		= {0x00, 0x00, 0x00, 0x00};
	const static Color MAGENTA  	= {0xff, 0x00, 0x00, 0xff};
	const static Color CYAN  		= {0x00, 0xff, 0x00, 0xff};
	const static Color WHITE 		= {0xff, 0xff, 0xff, 0xff};
	const static Color YELLOW		= {0xff, 0xff, 0x00, 0x00};

}
