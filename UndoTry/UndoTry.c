/*++

Copyright (c) 2008 - 2009, Solar Studio

Module Name:

  Try.c

Abstract:

  A hello demo module for EFI shell.

--*/

extern "C"{
#include "Efi.h"
#include "Solar.h"
#include "EfiDriverLib.h"
#include "SolarConsoleLib.h"
#include "SolarGraphicsLib.h"
#include "memory.h"//memcpy
#include EFI_PROTOCOL_DEFINITION (ConsoleControl)
}
#include "SolarCppLib.h"
#include "UndoTry.h"
#include "DrawFunc.h"

#ifdef __DEBUG__
#include "../_DevelopAddition/_Debug.h"
#endif

extern "C"{
EFI_APPLICATION_ENTRY_POINT(UndoTry)
}

INT32					  	  Locate[25][25];
INT32					  	  RecAry[25][25];

SOLAR_STATUS
SOLAR_LIB
DrawBoundary (
  IN     EFI_GRAPHICS_OUTPUT_PROTOCOL *pGraphOutput,
  IN OUT SOLAR_IMAGE_INPUT            Image,
  IN     UINT16                       X,
  IN     UINT16                       Y,
  IN 	 UINT16						  Widge,
  IN	 UINT32						  Color
  )
{
	UINT16 				  x2 = X + Widge;
	UINT16 				  y2 = Y + Widge;
	                                                        
	DrawLine( pGraphOutput, &Image,   X,   Y,  x2,   Y, Color, FullLine); //上移
	DrawLine( pGraphOutput, &Image,  x2,   Y,  x2,  y2, Color, FullLine); //右移
	DrawLine( pGraphOutput, &Image,  x2,  y2,   X,  y2, Color, FullLine); //下移
	DrawLine( pGraphOutput, &Image,   X,  y2,   X,   Y, Color, FullLine); //左移

	return SOLAR_SUCCESS;
}

SOLAR_STATUS
SOLAR_API
InitiateGraphicsMode(
	EFI_GRAPHICS_OUTPUT_PROTOCOL	**pGraph,
	UINT16							HorizontalResolution,
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

	QueryRequestMode(*pGraph, HorizontalResolution, VerticalResolution, ModeNUm);

	//FillScreen(*pGraph, SOLAR_BLUE);

	return SOLAR_SUCCESS;
}

SOLAR_STATUS
SOLAR_API
GraphicsTry(
	EFI_GRAPHICS_OUTPUT_PROTOCOL	**pGraph,
	SOLAR_IMAGE_INPUT				*Image
)
{
	//when entry the function background show blue
	EFI_STATUS			Status;//setting for pGraph
	EFI_INPUT_KEY		KeyCode;
	
	SOLAR_IMAGE_INPUT		DrawBoard, RecordBoard;
	UINT16					X = 50, Y = 25;
	UINT16					CoordinateX = 50; 
	UINT16					CoordinateY = 25; 
	UINT16              	LX = 0, LY = 0;
	UINT32					SelectedColor=SOLAR_YELLOW;

	DrawBoard.BltBuffer=(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)NULL;//set area with black background
	DrawBoard.Width=600;//400
	DrawBoard.Height=550;//300

	RecordBoard.BltBuffer=(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)NULL;
	RecordBoard.Width=200;
	RecordBoard.Height=200;
	
	if((DrawBoard.BltBuffer=(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)EfiLibAllocatePool(DrawBoard.Width * DrawBoard.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)))==NULL)
	{
		return EFI_OUT_OF_RESOURCES;
	}//show blue

	if((RecordBoard.BltBuffer=(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)EfiLibAllocatePool(RecordBoard.Width * RecordBoard.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)))==NULL)
	{
		return EFI_OUT_OF_RESOURCES;
	}

	if((Image->BltBuffer=(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)EfiLibAllocatePool(Image->Width*Image->Height*sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)))==NULL)
	{
		return EFI_OUT_OF_RESOURCES;
	}//show blue
	
	EfiSetMem(Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), 0);//same as above
	//FillScreen(*pGraph,SOLAR_PURPLE);//failed without below Status. Because pGraph show did not complete //show purple
	PrintString(*pGraph, Image,150,300,SOLAR_RED,L"Undo Testing");
	Status=(*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
	getch(&KeyCode);//show black

	EfiSetMem(DrawBoard.BltBuffer, DrawBoard.Width * DrawBoard.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), 0);
	FillScreen(*pGraph,SOLAR_GREEN);//SOLAR_GREEN
	//PrintString(*pGraph, &DrawBoard,150,150,SOLAR_RED,L"Testing");//background color is black
	
	EfiSetMem(RecordBoard.BltBuffer, RecordBoard.Width * RecordBoard.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), 0);
	DrawHorizontalLine(*pGraph, &RecordBoard, 0, 0, 5, SOLAR_WHITE, FullLine);
	DrawVerticalLine(*pGraph, &RecordBoard, 0, 0, 5, SOLAR_WHITE, FullLine);

	DrawHorizontalLine(*pGraph, &RecordBoard, 173, 0, 5, SOLAR_WHITE, FullLine);
	DrawVerticalLine(*pGraph, &RecordBoard, 178, 0, 5, SOLAR_WHITE, FullLine);

	DrawHorizontalLine(*pGraph, &RecordBoard, 0, 178, 5, SOLAR_WHITE, FullLine);
	DrawVerticalLine(*pGraph, &RecordBoard, 0, 173, 5, SOLAR_WHITE, FullLine);

	DrawHorizontalLine(*pGraph, &RecordBoard, 174, 178, 5, SOLAR_WHITE, FullLine);
	DrawVerticalLine(*pGraph, &RecordBoard, 178, 174, 5, SOLAR_WHITE, FullLine);

	//background boundary line drawing
	for (UINT16 i = 0; i < 26; i++) {
		DrawHorizontalLine(*pGraph, &DrawBoard, 50, 25 + i * 20, 500, SOLAR_WHITE, FullLine); 
		DrawVerticalLine(*pGraph, &DrawBoard, 50 + i * 20, 25, 500, SOLAR_WHITE, FullLine); 
	}
	DrawBoundary(*pGraph, DrawBoard, 50, 25, 20, SOLAR_RED);

	Status=(*pGraph)->Blt(*pGraph, DrawBoard.BltBuffer, EfiBltBufferToVideo,0,0,0,0,DrawBoard.Width,DrawBoard.Height, 0);//black size on left up area 400*300 and background color is green
	Status = (*pGraph)->Blt(*pGraph, RecordBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 600, 400, RecordBoard.Width, RecordBoard.Height, 0);  // 0, 0, 630, 530,
	
	UINT16 loop=1;
	while(loop)
	{
		getch(&KeyCode);
		switch (KeyCode.ScanCode)
		{
		case SCAN_UP:
				DrawBoundary(*pGraph, DrawBoard, X, Y, 20, SOLAR_WHITE);
				if(Y <= CoordinateY)
					Y = CoordinateY + 24*20;
				else
					Y -= 20;
				DrawBoundary(*pGraph, DrawBoard, X, Y, 20, SOLAR_RED);
				break;

		case SCAN_DOWN:
				DrawBoundary(*pGraph, DrawBoard, X, Y, 20, SOLAR_WHITE);
				if(Y >= CoordinateY + 24*20)
					Y = CoordinateY;
				else
					Y += 20;
				DrawBoundary(*pGraph, DrawBoard, X, Y, 20, SOLAR_RED);
				break;

		case SCAN_RIGHT:
				DrawBoundary(*pGraph, DrawBoard, X, Y, 20, SOLAR_WHITE);
				if (X >= CoordinateX + 24*20)
					X = CoordinateX;
				else
					X += 20;
				DrawBoundary(*pGraph, DrawBoard, X, Y, 20, SOLAR_RED);
				break;

		case SCAN_LEFT:
				DrawBoundary(*pGraph, DrawBoard, X, Y, 20, SOLAR_WHITE);
				if (X <= CoordinateX)
					X = CoordinateX + 24*20;
				else
					X -= 20;
				DrawBoundary(*pGraph, DrawBoard, X, Y, 20, SOLAR_RED);
				break;
		
		case SCAN_ESC:
				loop = 0;
				break;

		}
		Status = (*pGraph)->Blt(*pGraph, DrawBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawBoard.Width, DrawBoard.Height, 0);
		
		switch (KeyCode.UnicodeChar)
		{
		case CHAR_SPACE  :
				//make store action
				memcpy(&RecAry,&Locate,sizeof(Locate));
				DrawRectangle(*pGraph, &DrawBoard, X, Y, X+20, Y+20, SOLAR_RED, 0, Fill, SelectedColor, 3);  
				LX = (X - 50) / 20;
				LY = (Y - 25) / 20;
				if(SelectedColor==SOLAR_YELLOW)
				{
					Locate[LX][LY] = SOLAR_YELLOW;
				}
				else
				{
					Locate[LX][LY] = SelectedColor;
				}
				break;

		//刪除單格
		case CHAR_D:
		case CHAR_d:
				//make store action
				memcpy(&RecAry,&Locate,sizeof(Locate));
				DrawRectangle(*pGraph, &DrawBoard, X, Y, X+20, Y+20, SOLAR_RED, 0, Fill, SOLAR_BLACK, 3);
				LX = (X - 50) / 20;
				LY = (Y - 25) / 20;
				Locate[LX][LY] = SOLAR_BLACK;
				break;
		
		case CHAR_R: 
		case CHAR_r:
				for(UINT16 j=0; j<25 ; j++)
				{
					for(UINT16 k=0; k<25 ; k++)
					{
						UINT16 CX=k+6*k+2; //CX=k+6*k+1
						UINT16 CY=j+6*j+2;
						DrawRectangle(*pGraph, &RecordBoard, CX, CY, CX+6, CY+6, RecAry[k][j], 0, Fill, RecAry[k][j], 3);  
					}
				}
				Status = (*pGraph)->Blt(*pGraph, RecordBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 600, 400, RecordBoard.Width, RecordBoard.Height, 0);  //G
			
				break;	
		
			
		}
		Status = (*pGraph)->Blt(*pGraph, DrawBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawBoard.Width, DrawBoard.Height, 0); 


	}


	gBS->FreePool(Image->BltBuffer);//EFI memory free
	gBS->FreePool(DrawBoard.BltBuffer);
	gBS->FreePool(RecordBoard.BltBuffer);

	return SOLAR_SUCCESS;
}

extern "C"
EFI_STATUS
EFIAPI
UndoTry(
	IN EFI_HANDLE                            ImageHandle,
	IN EFI_SYSTEM_TABLE* 					 SystemTable
)
{
	EFI_STATUS					 Status=EFI_SUCCESS;
	EFI_SIMPLE_TEXT_OUT_PROTOCOL* cOut;
	UINT8 i=2; 
	EFI_GRAPHICS_OUTPUT_PROTOCOL *pGraph;//need to initial GOP
    SOLAR_IMAGE_INPUT            BasicImage;
	UINT32						 ModeNum;
	UINT16						 HorizonV;
	UINT16						 VerticalV;

	ALWAYS_USED(Status)

	EfiInitializeDriverLib(ImageHandle, SystemTable);
	
	// Console initial.
	cOut = gST->ConOut;

	if(InitiateSolarCppLib(ImageHandle,SystemTable)!=SOLAR_SUCCESS)
	{
		cOut->OutputString(cOut,L"\n>> SolarCppLib is not ready.");
		return EFI_UNSUPPORTED;
	}

	//define pConsole which instead RegisterProtocolNotify()=>Service UEFI driver 211109G
	#ifdef EFI_NT_EMULATOR
		EFI_CONSOLE_CONTROL_PROTOCOL* pConsole=(EFI_CONSOLE_CONTROL_PROTOCOL*)NULL;
	#endif

	if(InitiateSolarConsoleLib() != SOLAR_SUCCESS)
	{
		cOut->OutputString(cOut, L"\n>>SoalrConsoleLib is nor resdy.");
		return EFI_UNSUPPORTED;
	}

	#ifdef EFI_NT_EMULATOR
		Status=gBS->LocateProtocol(&gEfiConsoleControlProtocolGuid, NULL, (void**)&pConsole);
		if(EFI_ERROR(Status))
			return Status;

		pConsole->SetMode(pConsole, EfiConsoleControlScreenGraphics);
	#endif
	
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

	#ifdef EFI_NT_EMULATOR
		pConsole->SetMode(pConsole, EfiConsoleControlScreenText);
	#endif
	
	  setAttr(7,0);
	  cls;
	  showCursor;
	
	return Status;
}
