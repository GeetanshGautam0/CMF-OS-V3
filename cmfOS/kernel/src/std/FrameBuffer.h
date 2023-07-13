#pragma once
#include <stddef.h>

namespace FrameBuffer {

	struct FrameBuffer {
		void* BaseAddress;
		size_t BufferSize;
		unsigned int Width;
		unsigned int Height;
		unsigned int PixelsPerScanline;
	};

}
