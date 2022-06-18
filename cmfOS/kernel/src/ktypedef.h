#pragma once
#include "FrameBuffer.h" 
#include "math.h"
#include "SimpleFont.h"

struct BootInfo {
	FrameBuffer* frameBuffer;
	PSF1_FONT* psf1_font;
	void* mMap;
	uint64_t mMapSize;
	uint64_t mMapDescriptorSize;
};
