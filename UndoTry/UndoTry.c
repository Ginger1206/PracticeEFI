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

#include "UndoTry.h"

EFI_APPLICATION_ENTRY_POINT(UndoTry)

SOLAR_STATUS
SOLAR_API
InitiateGraphicsMode(
	EFI_GRAPHICS_OUTPUT_PROTOCOL	**pGraph,
	UINT16							HorizonResolution,
	UINT16							VerticalResolution,
	UINT32							*ModeNUm
)
{
	EFI_SIMPLE_TEXT_OUT_PROTOCOL	*cOut;

	cOut = gST->ConOut;

	if(InitiateSolarGraphicsLib(pGraph)!=SOLAR_SUCCESS)
	{
		cOut->OutputString(cOut, L"\n EFIGraphics ini not ready.");
		return SOLAR_SUCCESS;
	}

	QueryRequestMode(*pGraph, HorizonResolution, VerticalResolution, ModeNUm);

	return SOLAR_SUCCESS;
}

SOLAR_STATUS
SOLAR_API
GraphicsTry(
	EFI_GRAPHICS_OUTPUT_PROTOCOL	**pGraph,
	SOLAR_IMAGE_INPUT				*Image
)
{
	EFI_STATUS			Status;//setting for pGraph
	/*
	SOLAR_IMAGE_INPUT		DrawBoard;

	DrawBoard.BltBuffer=(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)NULL;
	DrawBoard.Width=400;
	DrawBoard.Height=300;
	*/
	if((Image->BltBuffer=(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)EfiLibAllocatePool(Image->Width*Image->Height*sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)))==NULL)
	{
		return EFI_OUT_OF_RESOURCES;
	}

	EfiSetMem(Image->BltBuffer, Image->Width*Image->Height*sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_RED);

	FillScreen(*pGraph,SOLAR_RED);//failed without below Status. Because pGraph show did not complete 
	Status=(*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);

	gBS->FreePool(Image->BltBuffer);//EFI memory free

	return SOLAR_SUCCESS;
}

EFI_STATUS
EFIAPI
UndoTry(
	IN EFI_HANDLE                            ImageHandle,
	IN EFI_SYSTEM_TABLE* 					 SystemTable
)
{
	EFI_SIMPLE_TEXT_OUT_PROTOCOL* cOut;
	UINT8 i=1; 
	EFI_GRAPHICS_OUTPUT_PROTOCOL *pGraph;//need to initial GOP
    SOLAR_IMAGE_INPUT            BasicImage;
	UINT32						 ModeNum;
	UINT16						 HorizonV;
	UINT16						 VerticalV;
    
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

	HorizonV=HORIZONTAL_VALUE;
	VerticalV=VERTICAL_VALUE;

	if (InitiateGraphicsMode(&pGraph, HorizonV, VerticalV, &ModeNum)!= SOLAR_SUCCESS)
	{
		cOut->OutputString(cOut, L"\n Initiate Graphics Mode UNSUCCESS!");
		return EFI_UNSUPPORTED;
	}

	BasicImage.Height = VerticalV;//600;
	BasicImage.Width = HorizonV;//800;

	GraphicsTry(&pGraph, &BasicImage);

	return EFI_SUCCESS;
}
