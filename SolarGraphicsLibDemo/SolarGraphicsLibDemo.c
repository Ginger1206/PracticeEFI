/*++ 

Copyright (c) 2008 - 2009, Solar Studio                                                         
         
Module Name:

  SolarGraphicsLibDemo.c

Abstract:

  This is the demo application program for SolarGraphicsLib.

--*/

extern "C" {
  #include "Efi.h"
  #include "EfiDriverLib.h"
  #include "Solar.h"
  #include "SolarConsoleLib.h"  // Library : SolarConsoleLib
  #include "SolarGraphicsLib.h" // Library : SolarGraphicsLib
  #include EFI_PROTOCOL_DEFINITION (ConsoleControl)
}
#include "SolarCppLib.h"        // Library : SolarCppLib (Stardart C++ module implement)
#include "SolarGraphicsLibDemo.h"
#ifdef __DEBUG__
  #include "../_DevelopAddition/_Debug.h"
#endif

extern "C" {
  EFI_APPLICATION_ENTRY_POINT (SolarGraphicsLibDemo);
}

SOLAR_STATUS
SOLAR_API
InitiateGraphicsMode (
  EFI_GRAPHICS_OUTPUT_PROTOCOL **pGraph,
  UINT16                       HorizontalResolution,
  UINT16                       VerticalResolution,
  UINT32                       *ModeNum
  )
{
  EFI_SIMPLE_TEXT_OUT_PROTOCOL *COut;

  // Get the console service table entry point.
  COut = gST->ConOut;

  // Check the Solar graphics library is supported.
  if (InitiateSolarGraphicsLib (pGraph) != SOLAR_SUCCESS) {
    COut->OutputString (COut, L"\n>> SolarGraphicsLib is not ready.");
    return SOLAR_UNSUCCESS;
  }

  QueryRequestMode (*pGraph, HorizontalResolution, VerticalResolution, ModeNum);

  FillScreen (*pGraph, SOLAR_BLACK);

  return SOLAR_SUCCESS;
}

SOLAR_STATUS
SOLAR_API
LibraryCapabilityDemo (
  EFI_GRAPHICS_OUTPUT_PROTOCOL **pGraph,
  SOLAR_IMAGE_INPUT            *Image
  )
{
  EFI_STATUS    Status;
  EFI_INPUT_KEY KeyCode;
//  UINTN         BmpWidth;
//  UINTN         BmpHeight;

  // Allocate a screen size memory for buffer.
  if ((Image->BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool (Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL))) == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  // Fill the black color into buffer.
  EfiSetMem (Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);

  /*
  // Draw pixel demo. 
  DrawPixel (*pGraph, Image, 10, 10, SOLAR_RED);
  DrawPixel (*pGraph, Image, 10, 13, SOLAR_GREEN);
  DrawPixel (*pGraph, Image, 13, 10, SOLAR_BLUE);
  DrawPixel (*pGraph, Image, 13, 13, SOLAR_YELLOW);
  */

  // Demo Starts  
  EfiSetMem (Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);  
  FillScreen (*pGraph, SOLAR_BLACK);
  PrintString (*pGraph, Image, 150, 290, SOLAR_WHITE, L"The Graphics Library Testing Program of Solar System. (Width = %d, Height = %d)", Image->Width, Image->Height);
  Status = (*pGraph)->Blt (
           *pGraph,
           Image->BltBuffer,
           EfiBltBufferToVideo,
           0,
           0,
           0,
           0,
           Image->Width,
           Image->Height,
           0
           );
  getch (&KeyCode);

  // Line Demo 
  EfiSetMem (Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);  
  FillScreen (*pGraph, SOLAR_BLACK);
//  for (UINT16 i = 0; i < 10; i ++) {
//    DrawHorizontalLine (*pGraph, Image, 355, 50 + i * 10, 90, SOLAR_WHITE, (UINT8) i);
//    DrawVerticalLine (*pGraph, Image, 355 + i * 10, 50, 90, SOLAR_WHITE, (UINT8) i);
//  }
  DrawLine (*pGraph, Image, 400, 100, 200, 400, SOLAR_WHITE, FullLine); // DrawLine (*pGraph, Image, 200, 200, 600, 500, SOLAR_WHITE, FullLine);
  DrawLine (*pGraph, Image, 400, 100, 600, 400, SOLAR_WHITE, FullLine);
  DrawLine (*pGraph, Image, 200, 400, 600, 400, SOLAR_WHITE, FullLine);
//  DrawLine (*pGraph, Image, 600, 200, 200, 500, SOLAR_GREEN, DottedLine);
  Status = (*pGraph)->Blt (
           *pGraph,
           Image->BltBuffer,
           EfiBltBufferToVideo,
           0,
           0,
           0,
           0,
           Image->Width,
           Image->Height,
           0
           );
  getch (&KeyCode);

  // Rectangle Demo 1
  EfiSetMem (Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);
  FillScreen (*pGraph, SOLAR_BLACK);
  DrawRectangle (*pGraph, Image, 300, 200, 500, 400, SOLAR_WHITE , 0, Fill, SOLAR_BLACK , 3); // (*pGraph, Image,350, 250, 449, 349, SOLAR_WHITE , 0, UR_ObliqueLine, SOLAR_WHITE , 3);
//  DrawRectangle (*pGraph, Image,   0,   0,  99,  99, SOLAR_RED   , 0, UR_ObliqueLine, SOLAR_WHITE , 3);
//  DrawRectangle (*pGraph, Image,   0, 500,  99, 599, SOLAR_GREEN , 0, UR_ObliqueLine, SOLAR_WHITE , 3);
//  DrawRectangle (*pGraph, Image, 700,   0, 799,  99, SOLAR_BLUE  , 0, UR_ObliqueLine, SOLAR_WHITE , 3);
//  DrawRectangle (*pGraph, Image, 700, 500, 799, 599, SOLAR_YELLOW, 0, UR_ObliqueLine, SOLAR_WHITE , 3);
//  DrawRectangle (*pGraph, Image,   0, 250,  99, 349, SOLAR_WHITE , 0, UR_ObliqueLine, SOLAR_RED   , 3);
//  DrawRectangle (*pGraph, Image, 350,   0, 449,  99, SOLAR_WHITE , 0, UR_ObliqueLine, SOLAR_GREEN , 3);
//  DrawRectangle (*pGraph, Image, 700, 250, 799, 349, SOLAR_WHITE , 0, UR_ObliqueLine, SOLAR_BLUE  , 3);
//  DrawRectangle (*pGraph, Image, 350, 500, 449, 599, SOLAR_WHITE , 0, UR_ObliqueLine, SOLAR_YELLOW, 3);
  Status = (*pGraph)->Blt (
           *pGraph,
           Image->BltBuffer,
           EfiBltBufferToVideo,
           0,
           0,
           0,
           0,
           Image->Width,
           Image->Height,
           0
           );
  getch (&KeyCode);

  // Rectangle Demo 2
  EfiSetMem (Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);
  FillScreen (*pGraph, SOLAR_BLACK); 
  DrawCircle (*pGraph, Image, 400, 300, 100, SOLAR_WHITE , 0, Fill, SOLAR_BLACK , 3);  
//  DrawRectangle (*pGraph, Image,   0,   0,  99,  99, SOLAR_WHITE , 0, UL_ObliqueLine, SOLAR_RED   , 3);
//  DrawRectangle (*pGraph, Image,   0, 500,  99, 599, SOLAR_WHITE , 0, UL_ObliqueLine, SOLAR_GREEN , 3);
//  DrawRectangle (*pGraph, Image, 700,   0, 799,  99, SOLAR_WHITE , 0, UL_ObliqueLine, SOLAR_BLUE  , 3);
//  DrawRectangle (*pGraph, Image, 700, 500, 799, 599, SOLAR_WHITE , 0, UL_ObliqueLine, SOLAR_YELLOW, 3);
//  DrawRectangle (*pGraph, Image,   0, 250,  99, 349, SOLAR_RED   , 0, UL_ObliqueLine, SOLAR_WHITE , 3);
//  DrawRectangle (*pGraph, Image, 350,   0, 449,  99, SOLAR_GREEN , 0, UL_ObliqueLine, SOLAR_WHITE , 3);
//  DrawRectangle (*pGraph, Image, 700, 250, 799, 349, SOLAR_BLUE  , 0, UL_ObliqueLine, SOLAR_WHITE , 3);
//  DrawRectangle (*pGraph, Image, 350, 500, 449, 599, SOLAR_YELLOW, 0, UL_ObliqueLine, SOLAR_WHITE , 3);
  Status = (*pGraph)->Blt (
           *pGraph,
           Image->BltBuffer,
           EfiBltBufferToVideo,
           0,
           0,
           0,
           0,
           Image->Width,
           Image->Height,
           0
           );
  getch (&KeyCode);
/*
  // Rectangle Demo 3
  EfiSetMem (Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);
  FillScreen (*pGraph, SOLAR_BLACK);
  DrawRectangle (*pGraph, Image, 370, 250, 429, 349, SOLAR_WHITE , 0, UR_ObliqueLine, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image,   0,   0,  59,  99, SOLAR_RED   , 0, UR_ObliqueLine, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image,   0, 500,  59, 599, SOLAR_GREEN , 0, UR_ObliqueLine, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image, 740,   0, 799,  99, SOLAR_BLUE  , 0, UR_ObliqueLine, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image, 740, 500, 799, 599, SOLAR_YELLOW, 0, UR_ObliqueLine, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image,   0, 250,  59, 349, SOLAR_WHITE , 0, UR_ObliqueLine, SOLAR_RED   , 3);
  DrawRectangle (*pGraph, Image, 370,   0, 429,  99, SOLAR_WHITE , 0, UR_ObliqueLine, SOLAR_GREEN , 3);
  DrawRectangle (*pGraph, Image, 740, 250, 799, 349, SOLAR_WHITE , 0, UR_ObliqueLine, SOLAR_BLUE  , 3);
  DrawRectangle (*pGraph, Image, 370, 500, 429, 599, SOLAR_WHITE , 0, UR_ObliqueLine, SOLAR_YELLOW, 3);
  Status = (*pGraph)->Blt (
           *pGraph,
           Image->BltBuffer,
           EfiBltBufferToVideo,
           0,
           0,
           0,
           0,
           Image->Width,
           Image->Height,
           0
           );
  getch (&KeyCode);

  // Rectangle Demo 4
  EfiSetMem (Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);
  FillScreen (*pGraph, SOLAR_BLACK);
  DrawRectangle (*pGraph, Image, 370, 250, 429, 349, SOLAR_WHITE , 0, UL_ObliqueLine, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image,   0,   0,  59,  99, SOLAR_WHITE , 0, UL_ObliqueLine, SOLAR_RED   , 3);
  DrawRectangle (*pGraph, Image,   0, 500,  59, 599, SOLAR_WHITE , 0, UL_ObliqueLine, SOLAR_GREEN , 3);
  DrawRectangle (*pGraph, Image, 740,   0, 799,  99, SOLAR_WHITE , 0, UL_ObliqueLine, SOLAR_BLUE  , 3);
  DrawRectangle (*pGraph, Image, 740, 500, 799, 599, SOLAR_WHITE , 0, UL_ObliqueLine, SOLAR_YELLOW, 3);
  DrawRectangle (*pGraph, Image,   0, 250,  59, 349, SOLAR_RED   , 0, UL_ObliqueLine, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image, 370,   0, 429,  99, SOLAR_GREEN , 0, UL_ObliqueLine, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image, 740, 250, 799, 349, SOLAR_BLUE  , 0, UL_ObliqueLine, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image, 370, 500, 429, 599, SOLAR_YELLOW, 0, UL_ObliqueLine, SOLAR_WHITE , 3);
  Status = (*pGraph)->Blt (
           *pGraph,
           Image->BltBuffer,
           EfiBltBufferToVideo,
           0,
           0,
           0,
           0,
           Image->Width,
           Image->Height,
           0
           );
  getch (&KeyCode);

  // Rectangle Demo 5
  EfiSetMem (Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);
  FillScreen (*pGraph, SOLAR_BLACK);
  DrawRectangle (*pGraph, Image, 350, 270, 449, 329, SOLAR_WHITE , 0, UR_ObliqueLine, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image,   0,   0,  99,  59, SOLAR_RED   , 0, UR_ObliqueLine, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image,   0, 540,  99, 599, SOLAR_GREEN , 0, UR_ObliqueLine, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image, 700,   0, 799,  59, SOLAR_BLUE  , 0, UR_ObliqueLine, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image, 700, 540, 799, 599, SOLAR_YELLOW, 0, UR_ObliqueLine, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image,   0, 270,  99, 329, SOLAR_WHITE , 0, UR_ObliqueLine, SOLAR_RED   , 3);
  DrawRectangle (*pGraph, Image, 350,   0, 449,  59, SOLAR_WHITE , 0, UR_ObliqueLine, SOLAR_GREEN , 3);
  DrawRectangle (*pGraph, Image, 700, 270, 799, 329, SOLAR_WHITE , 0, UR_ObliqueLine, SOLAR_BLUE  , 3);
  DrawRectangle (*pGraph, Image, 350, 540, 449, 599, SOLAR_WHITE , 0, UR_ObliqueLine, SOLAR_YELLOW, 3);
  Status = (*pGraph)->Blt (
           *pGraph,
           Image->BltBuffer,
           EfiBltBufferToVideo,
           0,
           0,
           0,
           0,
           Image->Width,
           Image->Height,
           0
           );
  getch (&KeyCode);

  // Rectangle Demo 6
  EfiSetMem (Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);
  FillScreen (*pGraph, SOLAR_BLACK);
  DrawRectangle (*pGraph, Image, 350, 270, 449, 329, SOLAR_WHITE , 0, UL_ObliqueLine, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image,   0,   0,  99,  59, SOLAR_WHITE , 0, UL_ObliqueLine, SOLAR_RED   , 3);
  DrawRectangle (*pGraph, Image,   0, 540,  99, 599, SOLAR_WHITE , 0, UL_ObliqueLine, SOLAR_GREEN , 3);
  DrawRectangle (*pGraph, Image, 700,   0, 799,  59, SOLAR_WHITE , 0, UL_ObliqueLine, SOLAR_BLUE  , 3);
  DrawRectangle (*pGraph, Image, 700, 540, 799, 599, SOLAR_WHITE , 0, UL_ObliqueLine, SOLAR_YELLOW, 3);
  DrawRectangle (*pGraph, Image,   0, 270,  99, 329, SOLAR_RED   , 0, UL_ObliqueLine, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image, 350,   0, 449,  59, SOLAR_GREEN , 0, UL_ObliqueLine, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image, 700, 270, 799, 329, SOLAR_BLUE  , 0, UL_ObliqueLine, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image, 350, 540, 449, 599, SOLAR_YELLOW, 0, UL_ObliqueLine, SOLAR_WHITE , 3);
  Status = (*pGraph)->Blt (
           *pGraph,
           Image->BltBuffer,
           EfiBltBufferToVideo,
           0,
           0,
           0,
           0,
           Image->Width,
           Image->Height,
           0
           );
  getch (&KeyCode);

  // Rectangle Demo 7
  EfiSetMem (Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);
  FillScreen (*pGraph, SOLAR_BLACK);
  DrawRectangle (*pGraph, Image, 350, 250, 449, 349, SOLAR_WHITE , 0, Dot, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image,   0,   0,  99,  99, SOLAR_RED   , 0, Dot, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image,   0, 500,  99, 599, SOLAR_GREEN , 0, Dot, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image, 700,   0, 799,  99, SOLAR_BLUE  , 0, Dot, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image, 700, 500, 799, 599, SOLAR_YELLOW, 0, Dot, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image,   0, 250,  99, 349, SOLAR_WHITE , 0, Dot, SOLAR_RED   , 3);
  DrawRectangle (*pGraph, Image, 350,   0, 449,  99, SOLAR_WHITE , 0, Dot, SOLAR_GREEN , 3);
  DrawRectangle (*pGraph, Image, 700, 250, 799, 349, SOLAR_WHITE , 0, Dot, SOLAR_BLUE  , 3);
  DrawRectangle (*pGraph, Image, 350, 500, 449, 599, SOLAR_WHITE , 0, Dot, SOLAR_YELLOW, 3);
  Status = (*pGraph)->Blt (
           *pGraph,
           Image->BltBuffer,
           EfiBltBufferToVideo,
           0,
           0,
           0,
           0,
           Image->Width,
           Image->Height,
           0
           );
  getch (&KeyCode);

  // Rectangle Demo 8
  EfiSetMem (Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);
  FillScreen (*pGraph, SOLAR_BLACK);
  DrawRectangle (*pGraph, Image, 350, 250, 449, 349, SOLAR_WHITE , 0, Fill, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image,   0,   0,  99,  99, SOLAR_RED   , 0, Fill, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image,   0, 500,  99, 599, SOLAR_GREEN , 0, Fill, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image, 700,   0, 799,  99, SOLAR_BLUE  , 0, Fill, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image, 700, 500, 799, 599, SOLAR_YELLOW, 0, Fill, SOLAR_WHITE , 3);
  DrawRectangle (*pGraph, Image,   0, 250,  99, 349, SOLAR_WHITE , 0, Fill, SOLAR_RED   , 3);
  DrawRectangle (*pGraph, Image, 350,   0, 449,  99, SOLAR_WHITE , 0, Fill, SOLAR_GREEN , 3);
  DrawRectangle (*pGraph, Image, 700, 250, 799, 349, SOLAR_WHITE , 0, Fill, SOLAR_BLUE  , 3);
  DrawRectangle (*pGraph, Image, 350, 500, 449, 599, SOLAR_WHITE , 0, Fill, SOLAR_YELLOW, 3);
  Status = (*pGraph)->Blt (
           *pGraph,
           Image->BltBuffer,
           EfiBltBufferToVideo,
           0,
           0,
           0,
           0,
           Image->Width,
           Image->Height,
           0
           );
  getch (&KeyCode);

  // BMP Demo
  EfiSetMem (Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);
  FillScreen (*pGraph, SOLAR_BLACK);
  BmpFileToSolarImage (*pGraph, L"SolarSystem.bmp", Image, &BmpWidth, &BmpHeight);
  Status = (*pGraph)->Blt (
           *pGraph,
           Image->BltBuffer,
           EfiBltBufferToVideo,
           0,
           0,
           0,
           0,
           Image->Width,
           Image->Height,
           0
           );
  getch (&KeyCode);
*/
  // Demo Ends  
  EfiSetMem (Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);  
  FillScreen (*pGraph, SOLAR_BLACK);
  PrintString (*pGraph, Image, 370, 290, SOLAR_WHITE, L"THE END");
  Status = (*pGraph)->Blt (
           *pGraph,
           Image->BltBuffer,
           EfiBltBufferToVideo,
           0,
           0,
           0,
           0,
           Image->Width,
           Image->Height,
           0
           );

  gBS->FreePool (Image->BltBuffer);

  return SOLAR_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////
//
//  SolarGraphicsLibDemo () - The entry point for this application program.
//
//  ARGUMENTS:
//    ImageHandle           - EFI_HANDLE: The firmware allocated handle for 
//                            the EFI Driver image.
//    SystemTable           - EFI_SYSTEM_TABLE: A pointer to the EFI system table  
//
//  RETURNS:
//    EFI_SUCCESS           - The program is loaded success.
//
/////////////////////////////////////////////////////////////////////////////
extern "C"	// For x64
EFI_STATUS
EFIAPI
SolarGraphicsLibDemo (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
{
  EFI_STATUS                   Status = EFI_SUCCESS;
  EFI_SIMPLE_TEXT_OUT_PROTOCOL *COut;
  EFI_INPUT_KEY                KeyCode;
  UINT16                       HorizontalResolution;
  UINT16                       VerticalResolution;
  EFI_GRAPHICS_OUTPUT_PROTOCOL *pGraph;
  SOLAR_IMAGE_INPUT            DesktopImage;
  UINT32                       ModeNum;
#ifdef EFI_NT_EMULATOR
  EFI_CONSOLE_CONTROL_PROTOCOL  *pConsole = (EFI_CONSOLE_CONTROL_PROTOCOL *) NULL;
#endif  // EFI_NT_EMULATOR

  // Never be detected by compiler that variable is used or not.
  ALWAYS_USED (Status)

  // Initialize "EfiDriverLib.Lib".
  EfiInitializeDriverLib (ImageHandle, SystemTable);

  // Get the console service table entry point.
  COut = gST->ConOut;
  
  // Check the Solar C++ library is supported.
  if (InitiateSolarCppLib (ImageHandle, SystemTable) != SOLAR_SUCCESS) {
    COut->OutputString (COut, L"\n>> SolarCppLib isn not ready.");
    return EFI_UNSUPPORTED;
  }

  // Check the Solar console library is supported.
  if (InitiateSolarConsoleLib () != SOLAR_SUCCESS) {
    COut->OutputString (COut, L"\n>> SolarConsleLib is not ready.");
    return EFI_UNSUPPORTED;
  }

#ifdef EFI_NT_EMULATOR
  // Locate Console Control Protocol.
  Status = gBS->LocateProtocol (
                &gEfiConsoleControlProtocolGuid,
                NULL,
                (void **) &pConsole
                );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  // Set console control mode for graphics using.
  pConsole->SetMode(pConsole, EfiConsoleControlScreenGraphics);
#endif  // EFI_NT_EMULATOR

  HorizontalResolution = 800;   //800->1000
  VerticalResolution = 600;     //600->1000

  if (InitiateGraphicsMode (&pGraph, HorizontalResolution, VerticalResolution, &ModeNum) != SOLAR_SUCCESS) {
    COut->OutputString (COut, L"\n>> Initiaate Graphics Mode UNSUCCESS!");
    return EFI_UNSUPPORTED;
  }

  DesktopImage.Height = VerticalResolution;
  DesktopImage.Width  = HorizontalResolution;
  LibraryCapabilityDemo (&pGraph, &DesktopImage);

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
