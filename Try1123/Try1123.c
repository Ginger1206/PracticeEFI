/*++

Copyright (c) 2008 - 2009, Solar Studio

Module Name:

  Try.c

Abstract:

  A hello demo module for EFI shell.

--*/

#include "Efi.h"
#include "Solar.h"
#include "EfiDriverLib.h"
#include "SolarConsoleLib.h"
#include "SolarGraphicsLib.h"
#include "SolarCppLib.h"

EFI_APPLICATION_ENTRY_POINT(Try1123)

EFI_STATUS
Try1123(
	IN EFI_HANDLE                            ImageHandle,
	/*
	EFI_GRAPHICS_OUTPUT_PROTOCOL *pGraph,
    SOLAR_IMAGE_INPUT            DesktopImage,
    CHESS_BLOCK_INFO             *ChessBlock,
	*/
	IN EFI_SYSTEM_TABLE* SystemTable
)
{
	EFI_SIMPLE_TEXT_OUT_PROTOCOL* cOut;
	UINT8 i=2; //

	/*
	UINT16                       Sn;
	UINT8                        ChessBlockStatus;
	*/
	EfiInitializeDriverLib(ImageHandle, SystemTable);
	
	//
	// Console initial.
	//
	cOut = gST->ConOut;
	/// 
	//cOut->OutputString(cOut, L"Hello EFI123 ! \t"); //add "\n" show enter&tab 
	//cOut->OutputString(cOut, L"qqqqqqq!!!");

	switch (i) {
	case 0:                            // 10X10
		cOut->OutputString(cOut, L"this is 1");
		break;
	case 1:                            // 25x25
		cOut->OutputString(cOut, L"Here is 2");
		break;
	case 2:                            // 50*50
		cOut->OutputString(cOut, L"See ya");
		break;
	}

	return EFI_SUCCESS;
}
