/*++ 

Copyright (c) 2008 - 2009, Solar Studio                                                         
         
Module Name:

  SolarGraphicsLib123.c

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
#include "SolarGraphicsLib123.h"
#include "ImageBlock.h"    //201126G
//#include "ObstructChessRound.h" //201201G
#ifdef __DEBUG__
  #include "../_DevelopAddition/_Debug.h"
#endif

extern "C" {
  EFI_APPLICATION_ENTRY_POINT (SolarGraphicsLib123);
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
  //CHAR16        Unum = 0; //
  UINTN         BmpWidth;
  UINTN         BmpHeight;
//  UINT16                        CheckColumn = 0;
//  UINT16                       CheckBlockSize = 0;


  // Allocate a screen size memory for buffer.
  if ((Image->BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool (Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL))) == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  // Fill the black color into buffer.
  EfiSetMem (Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);
  
  
  // Demo Starts  
  EfiSetMem (Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);  
  FillScreen (*pGraph, SOLAR_BLACK); 
  PrintString (*pGraph, Image, 150, 290, SOLAR_WHITE, L"The Graphics Library Testing Program of Solar System. (Width = %d, Height = %d)", Image->Width, Image->Height); 
  //PrintString (*pGraph, Image, 150, 310, SOLAR_WHITE, L"Please input number: ");
  /*
  //Want to get type content (include L71 Unum declare
  getch(&KeyCode);
  
  Unum=(CHAR16) KeyCode.UnicodeChar  ;// Unum=(CHAR16) KeyCode.UnicodeChar + (UINT16)1
  //wprintf(L"%c", (CHAR16) KeyCode.UnicodeChar);
  PrintString (*pGraph, Image, 270, 310, SOLAR_WHITE, L"%c", Unum);//L"%d", KeyCode.ScanCode    L"%c", (CHAR16) KeyCode.UnicodeChar
  Unum = Unum + (UINT16)1; //Unum + = Unum(C++
  PrintString (*pGraph, Image, 350, 310, SOLAR_WHITE, L"%c", Unum); 
  */
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
  
  // Chess board setting - starts
  {
      //UINT16 ItemPtr = 0xFF;
      //UINT16 ItemNum = 4;
      //UINT16 CheckBlockSize ; //201126G
      //UINT16 CheckColumn ; //201126G


      /*
      while (ItemPtr == 0xFF) {
          ItemPtr = SelectItem(
              *pGraph,
              *Image,
              102,
              ItemNum * ITEM_HEIGHT,
              BLOCK_SET_TO_CENTER_X(102),
              BLOCK_SET_TO_CENTER_Y(ItemNum * ITEM_HEIGHT) + 30,
              SOLAR_BLACK,
              SOLAR_WHITE,
              SOLAR_WHITE,
              SOLAR_BLUE,
              ItemNum,
              L"10 x 10 \\25 x 25\\50 x 50 \\"
          );
      }
      
      CheckBlockSize = 50;    //CheckBlockOneSideOnChessBoard = 11;
      switch (ItemPtr) {
      case 0:                            // 10X10
          CheckBlockSize = 11;
          CheckColumn=50;
          break;
      case 1:                            // 25x25
          CheckBlockSize = 26;  //CheckBlockOneSideOnChessBoard = 7;
          CheckColumn=20;
          break;
      case 2:                            // 50*50
          CheckBlockSize = 51; // CheckBlockOneSideOnChessBoard = 9;
          CheckColumn=10;
          break;
      }   

      switch (ItemPtr) {
      case 0:
          cOut->OutputString(cOut, L"select one");
      }
      */
  }
  
 /*
  
  //for test
  UINT16 CheckColumn = 25;
  UINT16 CheckBlockSize = 10;
  
  EfiSetMem(Image->BltBuffer, Image->Width* Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);
  FillScreen(*pGraph, SOLAR_BLACK);
  for (UINT16 i = 0; i < CheckBlockSize; i++)
  {
      DrawHorizontalLine(*pGraph, Image, 150, 50 + i * CheckColumn, 500, SOLAR_WHITE, FullLine);
      DrawVerticalLine(*pGraph, Image, 150 + i * CheckColumn, 50, 500, SOLAR_WHITE, FullLine);
  }
  Status = (*pGraph)->Blt(
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
  getch(&KeyCode);
  //}
  
  */

  
  // Line Demo => 10*10
  EfiSetMem (Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);  
  FillScreen (*pGraph, SOLAR_BLACK);
  //PrintString (*pGraph, Image, 350, 310, SOLAR_WHITE, L"%c", Unum); 
  for (UINT16 i = 0; i < 11; i ++) {
    DrawHorizontalLine (*pGraph, Image, 150, 50 + i * 50, 500, SOLAR_WHITE, FullLine); // DrawHorizontalLine (*pGraph, Image, 355, 50 + i * 10, 90, SOLAR_WHITE, (UINT8) i)
    DrawVerticalLine (*pGraph, Image, 150 + i * 50, 50, 500, SOLAR_WHITE, FullLine); // DrawVerticalLine (*pGraph, Image, 355 + i * 10, 50, 90, SOLAR_WHITE, (UINT8) i)
  }

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


  // Line Demo => 25*25
  EfiSetMem(Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);
  FillScreen(*pGraph, SOLAR_BLACK);
  //PrintString (*pGraph, Image, 350, 310, SOLAR_WHITE, L"%c", Unum); 
  for (UINT16 i = 0; i < 26; i++) {
      DrawHorizontalLine(*pGraph, Image, 150, 50 + i * 20, 500, SOLAR_WHITE, FullLine); // DrawHorizontalLine (*pGraph, Image, 355, 50 + i * 10, 90, SOLAR_WHITE, (UINT8) i)
      DrawVerticalLine(*pGraph, Image, 150 + i * 20, 50, 500, SOLAR_WHITE, FullLine); // DrawVerticalLine (*pGraph, Image, 355 + i * 10, 50, 90, SOLAR_WHITE, (UINT8) i)
  }

  Status = (*pGraph)->Blt(
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
  getch(&KeyCode);


  // Line Demo => 50*50
  EfiSetMem(Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);
  FillScreen(*pGraph, SOLAR_BLACK);
  for (UINT16 i = 0; i < 51; i++) {
      DrawHorizontalLine(*pGraph, Image, 150, 50 + i * 10, 500, SOLAR_WHITE, FullLine);
      DrawVerticalLine(*pGraph, Image, 150 + i * 10, 50, 500, SOLAR_WHITE, FullLine); 
  }

  Status = (*pGraph)->Blt(
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
  getch(&KeyCode);
  
  
  // BMP Demo
  EfiSetMem (Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);
  FillScreen (*pGraph, SOLAR_BLACK);
  BmpFileToSolarImage (*pGraph, L"SolarSystem.bmp", Image, &BmpWidth, &BmpHeight);//
  //BmpFileToSolarImage (*pGraph, L"111.bmp", Image, &BmpWidth, &BmpHeight);
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
  
  //Status = (EFI_STATUS)Round(pGraph, DesktopImage, ChessBlock, isSinglePlayer); //210208G

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
SolarGraphicsLib123 (
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
    COut->OutputString (COut, L"\n>> SolarCppLib is not ready.");
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

  //Status = (EFI_STATUS)Round(pGraph, DesktopImage, ChessBlock, isSinglePlayer); //210208G

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
