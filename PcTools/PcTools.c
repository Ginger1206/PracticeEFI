/*++ 

Copyright (c) 2008 - 2009, Solar Studio                                                         
         
Module Name:

  PcTools.c

Abstract:

  This is an integrated system utility for IBM compatible PC.

--*/

extern "C" {
  #include "Efi.h"
  #include "EfiDriverLib.h"
  #include "Solar.h"
  #include "SolarConsoleLib.h"  // Library : SolarConsoleLib
}
#include "SolarCppLib.h"        // Library : SolarCppLib (Stardart C++ module implement)
#include "PcTools.h"
#include "Pci.h"
#ifdef __DEBUG__
  #include "../_DevelopAddition/_Debug.h"
#endif


extern "C" {
  EFI_APPLICATION_ENTRY_POINT (PcTools);
}

#define LOGO_STRING  L"SOLAR"
#define PUBLISH_UNIT L"SOLAR STUDIO"
#define PROJECT_NAME L"PC-TOOLS"

/////////////////////////////////////////////////////////////////////////////
//
//  PrintScreenFramework () - Layout the application program framework of screen.
//
//  ARGUMENTS:
//    None  
//
//  RETURNS:
//    SOLAR_SUCCESS         - The routine is regular.
//
/////////////////////////////////////////////////////////////////////////////
SOLAR_STATUS
SOLAR_API
PrintScreenFramework (
  void
  )
{
  UINT16 i;

  cls;	                                // Clear screen.
  hideCursor;                           // Hide cursor.

  // First line.
  setCursorPos (1, 0);
  AWPrintf (EFI_WHITE, EFI_BLACK, PROJECT_NAME);
  setCursorPos (80 - EfiStrLen (LOGO_STRING) - 1, 0);
  AWPrintf (EFI_RED, EFI_BLACK, L"S");
  AWPrintf (EFI_LIGHTRED, EFI_BLACK, L"O");
  AWPrintf (EFI_YELLOW, EFI_BLACK, L"L");
  AWPrintf (EFI_LIGHTGREEN, EFI_BLACK, L"A");
  AWPrintf (EFI_LIGHTBLUE, EFI_BLACK, L"R");

  // Second line.
  PAWPrintf (0, 1, EFI_LIGHTGRAY, EFI_BLUE, SPACE_LINE);
  i = MAIN_MENU_START_X;
  PWPrintf (i, 1, MAIN_MENU_FILE_STR);
  i += (UINT8) EfiStrLen (MAIN_MENU_FILE_STR) + MAIN_MENU_ITEM_INTERVAL;
  PWPrintf (i, 1, MAIN_MENU_INFO_STR);
  i += (UINT8) EfiStrLen (MAIN_MENU_INFO_STR) + MAIN_MENU_ITEM_INTERVAL;
  PWPrintf (i, 1, MAIN_MENU_PCI_STR);
  i += (UINT8) EfiStrLen (MAIN_MENU_PCI_STR) + MAIN_MENU_ITEM_INTERVAL;
  PWPrintf (i, 1, MAIN_MENU_ISA_STR);
  i += (UINT8) EfiStrLen (MAIN_MENU_ISA_STR) + MAIN_MENU_ITEM_INTERVAL;
  PWPrintf (i, 1, MAIN_MENU_MEMORY_STR);
  i += (UINT8) EfiStrLen (MAIN_MENU_MEMORY_STR) + MAIN_MENU_ITEM_INTERVAL;
  PWPrintf (i, 1, MAIN_MENU_HELP_STR);
  i += (UINT8) EfiStrLen (MAIN_MENU_HELP_STR) + MAIN_MENU_ITEM_INTERVAL;

  // Remainder lines.
  for (i = 2; i < 22; i ++) {
    PAWPrintf (0, 1 + i, EFI_LIGHTGRAY, EFI_LIGHTGRAY, SPACE_LINE);
  }

  // The last but one line.
  PAWPrintf (0, 23, EFI_LIGHTGRAY, EFI_LIGHTGRAY, SPACE_LINE);

  return SOLAR_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////
//
//  PcTools () - The entry point for this application program.
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
PcTools (
  IN EFI_HANDLE                 ImageHandle,
  IN EFI_SYSTEM_TABLE           *SystemTable
  )
{
  EFI_SIMPLE_TEXT_IN_PROTOCOL   *cIn;
  EFI_SIMPLE_TEXT_OUT_PROTOCOL  *cOut;
  EFI_INPUT_KEY                 KeyCode;
  UINT8                         PageCount = 0;
  UINT64                        PciAddress = 0;

  // Initialize "EfiDriverLib.Lib".
  EfiInitializeDriverLib (ImageHandle, SystemTable);

  // Initialixe "SolarCppLib.Lib"
  InitiateSolarCppLib (ImageHandle, SystemTable);

  // Console initiallize.
  cOut = gST->ConOut;
  cIn  = gST->ConIn;
  
  // Check the Solar console library is supported.
  if (InitiateSolarConsoleLib () != SOLAR_SUCCESS) {
    cOut->OutputString (cOut, L"\n>> SolarConsleLib isn't ready.");
    return EFI_UNSUPPORTED;
  }

  // Check PCI routine ready.
  if (PciInitiallize () != SOLAR_SUCCESS) {
    cOut->OutputString (cOut, L"\n>> PCI initiallize isn't ready.");
    return EFI_UNSUPPORTED;
  }
  
  // Screen framework setting.
  PrintScreenFramework ();              // Screen layout.

  // Starts PciViewer. 
  PciAddress = *(PciAddrList + PageCount);
  PciViewer (
    (UINT8) RShiftU64 ((PciAddress & 0x00FF00000000), 0x20), 
    (UINT8) RShiftU64 ((PciAddress & 0x0000FF000000), 0x18), 
    (UINT8) RShiftU64 ((PciAddress & 0x000000FF0000), 0x10), 
    (UINT8) RShiftU64 ((PciAddress & 0x00000000FF00), 0x08)
    );
  
  goto ProgramDone;                     // Jump for exit program.
////////////////////////////////////////////////////////////////////////
//                                                                    //
// Program Ending, please release memory and others exit code below ! //
//                                                                    //
////////////////////////////////////////////////////////////////////////
ProgramDone:                            // Exit program.

  getch (&KeyCode);                     // Wait any in-key.
  gBS->FreePool (PciAddrList);
  setAttr (7, 0);                       // Setup to normal attribute.
  cls;
  showCursor;                           // Show cursor.

  
#ifdef __DEBUG__
  _DebugPrintf (0, 24, 15, 0, L"[DEBUG]:END OF DEBUG !!", 0x00);
#endif

#ifdef __DEBUG__
  //_DebugWaitKey ();
#endif
  
  return EFI_SUCCESS;
}
