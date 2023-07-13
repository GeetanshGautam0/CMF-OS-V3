#pragma once


namespace Font {

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