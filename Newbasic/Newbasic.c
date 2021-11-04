/*++

Copyright (c) 2008 - 2009, Solar Studio

Module Name:

Newbasic.c

Abstract:

A basic demo module for EFI shell.

--*/

//#include <stdlib.h>

extern "C" {
//#include "stdlib.h"
#include "Efi.h"
#include "Solar.h"
#include "EfiDriverLib.h"
#include "SolarConsoleLib.h"
#include "SolarGraphicsLib.h"
#include EFI_PROTOCOL_DEFINITION (ConsoleControl)
}
#include "SolarCppLib.h"
#include "Newbasic.h"
//#include "DrawFunc.h"

#ifdef __DEBUG__
  #include "../_DevelopAddition/_Debug.h"
#endif

extern "C" {
EFI_APPLICATION_ENTRY_POINT(Newbasic);
}



SOLAR_SYSRET
SOLAR_API
DecToHex(
	IN	UINT32	X
)
{
	if(X>=58)
		X-=55;
	else
		X-=48;
	
	return X;
}

SOLAR_STATUS
SOLAR_API
Backgroundtest(
	EFI_GRAPHICS_OUTPUT_PROTOCOL  **pGraph,
	UINT16						  Horizon,
	UINT16						  Vertical,
	UINT32						  *ModeNum
)
{
	EFI_SIMPLE_TEXT_OUT_PROTOCOL	*COut;
	//EFI_INPUT_KEY					Keycode; //for test 210323

	COut = gST->ConOut; //Get console service table entry point.
	if (InitiateSolarGraphicsLib(pGraph) != SOLAR_SUCCESS) {
		COut->OutputString(COut, L"\n>> SolarGraphicsLib is not ready.");
		return SOLAR_UNSUCCESS;
	} //#include"SolarGraphicsLib"

	QueryRequestMode(*pGraph, Horizon, Vertical, ModeNum); //Page "start boot option"
	//getch(&Keycode);
	FillScreen(*pGraph, SOLAR_BLACK);
	//FillScreen(*pGraph, SOLAR_PALEGREEN);//test 
	//getch(&Keycode); //test this function  =>it will show a new black video or image
	
	return SOLAR_SUCCESS;
}

SOLAR_STATUS
SOLAR_API
GraphicsSimpleDemo(
	EFI_GRAPHICS_OUTPUT_PROTOCOL  **pGraph,
	SOLAR_IMAGE_INPUT			  *Image
)
{
	EFI_STATUS    		Status;
	EFI_INPUT_KEY 		KeyCode;
	UINT32				Color=0;
	UINT16 				XL=190;//XLocation
	UINT32              UC[6]={0};
	UINT16              N=0;
	UINT32				T[6];//T[6]={0}
	//UINT32				Test=0;
	UINT32              R=0,G=0,B=0;

    if ((Image->BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool (Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL))) == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

	// Demo Starts
	EfiSetMem(Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);// Fill the black color into buffer.
	FillScreen(*pGraph, SOLAR_PURPLE );//SOLAR_BLACK
	PrintString(*pGraph, Image, 50, 200, SOLAR_WHITE, L"The Graphics Draw Game. Width = %d, Height = %d", Image->Width, Image->Height); 
    PrintString(*pGraph, Image, 50, 300, SOLAR_FLORALW, L"Please type any key : #________");
	Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
	//getch(&KeyCode);


	UINT16 loop=1;
	while(loop){
		getch(&KeyCode);
		XL+=10;
		
		switch (KeyCode.UnicodeChar)//建0~9 A~F
		{
		case CHAR_A:
		case CHAR_a:
			PrintString(*pGraph, Image, XL, 300, SOLAR_LIME, L"A");
			UC[N]='A';
			break;
		
		case CHAR_B:
		case CHAR_b:
			PrintString(*pGraph, Image, XL, 300, SOLAR_LIME, L"B");
			UC[N]='B';
			break;
		
		case CHAR_C:
		case CHAR_c:
			PrintString(*pGraph, Image, XL, 300, SOLAR_LIME, L"C");
			UC[N]='C';
			break;

		case CHAR_D:
		case CHAR_d:
			PrintString(*pGraph, Image, XL, 300, SOLAR_LIME, L"D");
			UC[N]='D';
			break;

		case CHAR_E:
		case CHAR_e:
			PrintString(*pGraph, Image, XL, 300, SOLAR_LIME, L"E");
			UC[N]='E';
			break;

		case CHAR_F:
		case CHAR_f:
			PrintString(*pGraph, Image, XL, 300, SOLAR_LIME, L"F");
			UC[N]='F';
			break;

		case CHAR_0:
			PrintString(*pGraph, Image, XL, 300, SOLAR_LIME, L"0");
			UC[N]='0';
			break;
		
		case CHAR_1:
			PrintString(*pGraph, Image, XL, 300, SOLAR_LIME, L"1");
			UC[N]='1';
			break;

		case CHAR_2:
			PrintString(*pGraph, Image, XL, 300, SOLAR_LIME, L"2");
			UC[N]='2';
			break;

		case CHAR_3:
			PrintString(*pGraph, Image, XL, 300, SOLAR_LIME, L"3");
			UC[N]='3';
			break;

		case CHAR_4:
			PrintString(*pGraph, Image, XL, 300, SOLAR_LIME, L"4");
			UC[N]='4';
			break;

		case CHAR_5:
			PrintString(*pGraph, Image, XL, 300, SOLAR_LIME, L"5");
			UC[N]='5';
			break;

		case CHAR_6:
			PrintString(*pGraph, Image, XL, 300, SOLAR_LIME, L"6");
			UC[N]='6';
			break;

		case CHAR_7:
			PrintString(*pGraph, Image, XL, 300, SOLAR_LIME, L"7");
			UC[N]='7';
			break;

		case CHAR_8:
			PrintString(*pGraph, Image, XL, 300, SOLAR_LIME, L"8");
			UC[N]='8';
			break;

		case CHAR_9:
			PrintString(*pGraph, Image, XL, 300, SOLAR_LIME, L"9");
			UC[N]='9';
			break;

		case  CHAR_CARRIAGE_RETURN:
			if(N<5)
				PrintString(*pGraph, Image, 435,  100, SOLAR_YELLOW, L"[error]"); 
			/*
			else
			{
				R = T[0]*16 + T[1];
				G = T[2]*16 + T[3];
				B = T[4]*16 + T[5];
				Color=CombineRGB(R, G, B);
				PrintString(*pGraph, Image, 435,  111, Color, L"Final color is here!!! "); 
			}
			*/
			loop=0;
			break;
		}

		//for char show test
		for(UINT16 i=0;i<6;i++)
		{
			PrintString(*pGraph, Image, 30,  400+i*11, SOLAR_RED, L"Array [%d] is %c",i, UC[i] ); //L"Array [%d] is %s",i, UC[i]
			T[i]=DecToHex(UC[i]);
		}

		if(N>5)
		{	
			if(KeyCode.UnicodeChar == CHAR_CARRIAGE_RETURN)
			{
				R = T[0]*16 + T[1];
				G = T[2]*16 + T[3];
				B = T[4]*16 + T[5];
				Color=CombineRGB(R, G, B);
				PrintString(*pGraph, Image, 435,  115, Color, L"Final color is here!!! "); 
				PrintString(*pGraph, Image, 435,  100, SOLAR_RED, L"[Array ENTER]"); //final show
				Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
				loop=0;
				break;
			}
			
			PrintString(*pGraph, Image, XL, 300, SOLAR_RED, L"  over type!");
			XL=190;
			N=0;
			DrawRectangle(*pGraph, Image, 195, 295, 268, 315, SOLAR_BLACK, 0, Fill, SOLAR_BLACK, 3); 
			PrintString(*pGraph, Image, XL, 300, SOLAR_FLORALW, L"#________");
			loop=0;
		}
		else
		{
			N+=1;
		}

		Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);

	}

	//Value test show
	for(UINT16 i=0;i<6;i++)
	{
		PrintString(*pGraph, Image, 200, 400+i*11, SOLAR_GREEN, L"Array[%d] value is %d", i, T[i]);
		Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
	}

	//RGB Color test
	R = T[0]*16 + T[1];
	PrintString(*pGraph, Image, 400,  400, SOLAR_RED, L"R is [%d] ", R); 
	G = T[2]*16 + T[3];
	PrintString(*pGraph, Image, 400,  411, SOLAR_RED, L"G is [%d] ", G); 
	B = T[4]*16 + T[5];
	PrintString(*pGraph, Image, 400,  422, SOLAR_RED, L"B is [%d] ", B); 
	Color=CombineRGB(R, G, B);
	PrintString(*pGraph, Image, 400,  433, Color, L"RGB color is here!!! "); 

	Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
	
	//for test
	// for(UINT16 i=0;i<6;i++)
	// {
	// 	PrintString(*pGraph, Image, 30,  400+i*11, SOLAR_RED, L"Array [%d] is %c",i, UC[i]); //L"Array [%d] is %s",i, UC[i]
	// 	Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
	// }
	
	gBS->FreePool(Image->BltBuffer);

	return SOLAR_SUCCESS;
}


extern "C"	// For x64
EFI_STATUS
EFIAPI
Newbasic(
	IN EFI_HANDLE				ImageHandle,
	IN EFI_SYSTEM_TABLE			*SystemTable
)
{
	//EFI_SIMPLE_TEXT_IN_PROTOCOL		*cIn;
	EFI_SIMPLE_TEXT_OUT_PROTOCOL	*cOut;
	EFI_INPUT_KEY					KeyCode;
	EFI_STATUS						Status = EFI_SUCCESS;
	EFI_GRAPHICS_OUTPUT_PROTOCOL	*pGraph;
	UINT16							Horizon;
	UINT16							Vertical;
	UINT32							ModeNum;
	SOLAR_IMAGE_INPUT               DesktopImage;

	//CHAR16							Unum = 0;
	
#ifdef EFI_NT_EMULATOR
	EFI_CONSOLE_CONTROL_PROTOCOL	*pConsole = (EFI_CONSOLE_CONTROL_PROTOCOL *) NULL; 
#endif 

	ALWAYS_USED(Status)

	EfiInitializeDriverLib(ImageHandle, SystemTable);

	cOut = gST->ConOut;
	//cIn = gST->ConIn;
	//cOut->OutputString(cOut, L"Try for learn basic output");
	//getch(&KeyCode);

	// Check the Solar C++ library is supported.
	if (InitiateSolarCppLib (ImageHandle, SystemTable) != SOLAR_SUCCESS) {
		cOut->OutputString (cOut, L"\n>> SolarCppLib is not ready.");
		return EFI_UNSUPPORTED;
	}

	// Check the Solar console library is supported.
	if (InitiateSolarConsoleLib () != SOLAR_SUCCESS) {
		cOut->OutputString (cOut, L"\n>> SolarConsleLib is not ready.");
		return EFI_UNSUPPORTED;
	}


#ifdef EFI_NT_EMULATOR
	// Locate Console Control Protocol.
	Status = gBS->LocateProtocol(
		&gEfiConsoleControlProtocolGuid,
		NULL,
		(void**)&pConsole
	);
	if (EFI_ERROR(Status)) {
		return Status;
	}
	// Set console control mode for graphics using.
	pConsole->SetMode(pConsole, EfiConsoleControlScreenGraphics);
#endif  // EFI_NT_EMULATOR
	
	//Initial video
	Horizon = HORIZON_H;
	Vertical = VERTICAL_H;

	if (Backgroundtest(&pGraph, Horizon, Vertical, &ModeNum) != SOLAR_SUCCESS) {
		cOut->OutputString(cOut, L"\n>> Initiate Graphics Mode UNSUCCESS!");
		return EFI_UNSUPPORTED;
	}

	DesktopImage.Height = Vertical;
	DesktopImage.Width = Horizon;
	GraphicsSimpleDemo(&pGraph, &DesktopImage);

	  goto ProgramDone;                     // Jump for exit program.
////////////////////////////////////////////////////////////////////////
//                                                                    //
// Program Ending, please release memory and others exit code below ! //
//                                                                    //
////////////////////////////////////////////////////////////////////////
ProgramDone:                            // Exit program.

    getch (&KeyCode);                     // Wait any in-key.
#ifdef EFI_NT_EMULATOR
	pConsole->SetMode(pConsole, EfiConsoleControlScreenText);
#endif  // EFI_NT_EMULATOR
    setAttr (7, 0);                       // Setup to normal attribute.
	cls;
	showCursor;                           // Show cursor.

  
#ifdef __DEBUG__
  //_DebugPrintf (0, 24, 15, 0, L"[DEBUG]:END OF DEBUG !!", 0x00);
#endif  // _DEBUG_

#ifdef __DEBUG__
  //_DebugWaitKey ();
#endif  // _DEBUG_

	return Status;

}
