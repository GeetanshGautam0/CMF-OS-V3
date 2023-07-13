#include <efi.h>
#include <efilib.h>
#include <elf.h>
#include <stddef.h>


#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04


typedef struct {
	void* BaseAddress;
	size_t BufferSize;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixelsPerScanline;
} FrameBuffer;


typedef struct {
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charsize;
} PSF1_HEADER;


typedef struct {
	unsigned char w;
	unsigned char h;
} FontSize;


typedef struct {
	PSF1_HEADER* psf1_Header;
	void* glyphBuffer;
	FontSize font_size;
} PSF1_FONT;


typedef struct {
	FrameBuffer* frameBuffer;
	PSF1_FONT* psf1_font;
	EFI_MEMORY_DESCRIPTOR* mMap;
	UINTN mMapSize;
	UINTN mMapDescriptorSize;
	void* rsdp;
} BootInfo;


UINTN strcmp (CHAR8* str1, CHAR8* str2, UINTN length) {
	for (UINTN index = 0; index < length; index++) {
		if (*str1 != *str2) return 0;
		++str1;
		++str2;
	}
	return 1;
}


FrameBuffer frameBuffer;
FrameBuffer* InitializeGOP() {
	EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
	EFI_STATUS status;

	status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop);
	if (EFI_ERROR(status)) {
		Print(L"Unable to locate GOP\n\r");
		return NULL;
	}
	Print(L"GOP located\n\r");

	frameBuffer.BaseAddress = (void*)gop->Mode->FrameBufferBase;
	frameBuffer.BufferSize = gop->Mode->FrameBufferSize;
	frameBuffer.Width = gop->Mode->Info->HorizontalResolution;
	frameBuffer.Height = gop->Mode->Info->VerticalResolution;
	frameBuffer.PixelsPerScanline = gop->Mode->Info->PixelsPerScanLine;

	return &frameBuffer;
}


EFI_FILE* LoadFile (EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
	EFI_FILE* LoadedFile;
	EFI_LOADED_IMAGE_PROTOCOL* LoadedImage;
	SystemTable->BootServices->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&LoadedImage);

	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
	SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&FileSystem);

	if (Directory == NULL) {
		FileSystem->OpenVolume(FileSystem, &Directory);
	}

	EFI_STATUS s = Directory->Open(Directory, &LoadedFile, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
	if (s != EFI_SUCCESS) {
		return NULL;
	}
	return LoadedFile;

}


PSF1_FONT* LoadPSF1Font(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
	EFI_FILE* font = LoadFile(Directory, Path, ImageHandle, SystemTable);
	if (font == NULL) return NULL;

	PSF1_HEADER* fontHeader;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_FONT), (void**)&fontHeader);
	UINTN size = sizeof(PSF1_HEADER);
	font->Read(font, &size, fontHeader);

	if (fontHeader->magic[0] != PSF1_MAGIC0 || fontHeader->magic[1] != PSF1_MAGIC1) return NULL;

	UINTN glyphBufferSize = fontHeader->charsize * 256;
	if (fontHeader->mode == 1) glyphBufferSize = fontHeader->charsize * 512; // 512 glyph mode

	void* glyphBuffer;
	{
		font->SetPosition(font, sizeof(PSF1_HEADER));
		SystemTable->BootServices->AllocatePool(EfiLoaderData, glyphBufferSize, (void**)&glyphBuffer);
		font->Read(font, &glyphBufferSize, glyphBuffer);
	}

	PSF1_FONT* finishedFont;
	FontSize newFontSize;
	newFontSize.w = 8;
	newFontSize.h = fontHeader->charsize;

	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_FONT), (void**)&finishedFont); 
	finishedFont->psf1_Header = fontHeader;
	finishedFont->glyphBuffer = glyphBuffer;
	finishedFont->font_size = newFontSize;
	
	return finishedFont;
}


int memcmp(const void* aptr, const void* bptr, size_t n) {
	const unsigned char* a = aptr, *b = bptr;
	for (size_t i = 0; i < n; i++) {
		if (a[i] < b[i]) return -1;
		else if (a[i] > b[i]) return 1;
	}
	return 0;
}


EFI_STATUS efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	InitializeLib(ImageHandle, SystemTable);
	
	EFI_FILE* Kernel = LoadFile(NULL, L"kernel.elf", ImageHandle, SystemTable);
	if (Kernel == NULL) {
		Print(L"Could not load kernel.\n\r");
		while (true);
	}
	else Print(L"Kernel loaded successfully\n\r");

	Elf64_Ehdr header;
	{
		UINTN FileInfoSize;
		EFI_FILE_INFO* FileInfo;
		Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &FileInfoSize, NULL);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, FileInfoSize, (void**)&FileInfo);
		Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &FileInfoSize, (void**)&FileInfo);

		UINTN size = sizeof(header);
		Kernel->Read(Kernel, &size, &header);
	}

	if (
		memcmp(&header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
		header.e_ident[EI_CLASS] != ELFCLASS64 ||
		header.e_ident[EI_DATA] != ELFDATA2LSB ||
		header.e_type != ET_EXEC ||
		header.e_machine != EM_X86_64 || 
		header.e_version != EV_CURRENT
	) {
		Print(L"Kernel header format fails one or more tests.\n\r");
		while (true);
	} 
	else Print(L"Kernel header OK\n\r");

	Elf64_Phdr* phdrs;
	{
		Kernel->SetPosition(Kernel, header.e_phoff);
		UINTN size = header.e_phnum * header.e_phentsize;
		SystemTable->BootServices->AllocatePool(EfiLoaderData, size, (void**)&phdrs);
		Kernel->Read(Kernel, &size, phdrs);
	}

	for (
		Elf64_Phdr* phdr = phdrs; 
		(char*)phdr < (char*)phdrs + header.e_phnum * header.e_phentsize;
		phdr = (Elf64_Phdr*)((char*)phdr + header.e_phentsize)
	) {
		switch (phdr->p_type) {
			case PT_LOAD:
				{
					int pages = (phdr->p_memsz + 0x1000 - 1) / 0x1000;
					Elf64_Addr segment = phdr->p_paddr;
					SystemTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pages, &segment);

					Kernel->SetPosition(Kernel, phdr->p_offset);
					UINTN size = phdr->p_filesz;
					Kernel->Read(Kernel, &size, (void*)segment);
					break; 
				}
		}
	}

	Print(L"Kernel loaded successfully\n\r");

	
	PSF1_FONT* newFont= (LoadPSF1Font(NULL, L"zap-light16.psf", ImageHandle, SystemTable));	
	if (newFont == NULL) {
		Print(L"Font not found.");
		while (true);
	} else Print(L"Font found. char size = %d\n\r", newFont->psf1_Header->charsize);

	FrameBuffer* newBuffer = InitializeGOP();

	Print(
		L"GOP:\n\r\tBase:		0x%x\n\r\tSize:		0x%x\n\r\tWidth:		%d\n\r\tHeight:		%d\n\r\tPPS:		%d\n\r", 
		newBuffer->BaseAddress, newBuffer->BufferSize, newBuffer->Width, newBuffer->Height, newBuffer->PixelsPerScanline
	);

	EFI_MEMORY_DESCRIPTOR* Map = NULL;
	UINTN MapSize, MapKey; 
	UINTN DescriptorSize;
	UINT32 DescriptorVersion;
	{
		SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, MapSize, (void**)&Map);
		SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);
	}

	EFI_CONFIGURATION_TABLE* configTable = SystemTable->ConfigurationTable;
	void* rsdp = NULL; 
	EFI_GUID Acpi2TableGuid = ACPI_20_TABLE_GUID;

	for (UINTN index = 0; index < SystemTable->NumberOfTableEntries; index++){
		if (CompareGuid(&configTable[index].VendorGuid, &Acpi2TableGuid)){
			if (strcmp((CHAR8*)"RSD PTR ", (CHAR8*)configTable->VendorTable, 8) == 1){
				rsdp = (void*)configTable->VendorTable;
			}
		}
		configTable++;
	}

	BootInfo bootInfo;

	bootInfo.frameBuffer = newBuffer;
	bootInfo.psf1_font = newFont;
	bootInfo.mMap = Map;
	bootInfo.mMapSize = MapSize;
	bootInfo.mMapDescriptorSize = DescriptorSize;
	bootInfo.rsdp = rsdp;

	void (*KernelStart)() = 
		((__attribute__((sysv_abi)) void (*)(BootInfo*) ) header.e_entry);


	SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);


	KernelStart(&bootInfo);


	return EFI_SUCCESS; // Exit the UEFI application
}
