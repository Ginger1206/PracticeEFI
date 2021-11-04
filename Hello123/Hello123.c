/*++ 

Copyright (c) 2008 - 2009, Solar Studio                                                         
         
Module Name:

  Hello123.c

Abstract:

  A hello demo module for EFI shell.

--*/

#include "Efi.h"
#include "Solar.h" //
#include "EfiDriverLib.h"
#include "SolarConsoleLib.h" //
#include "SolarGraphicsLib.h" //
#include "SolarCppLib.h" //

EFI_APPLICATION_ENTRY_POINT (Hello123)

EFI_STATUS
Hello123 (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  //EFI_GRAPHICS_OUTPUT_PROTOCOL 	           **pGraph, //
  //SOLAR_IMAGE_INPUT	                       *Image //
  )
{
  EFI_SIMPLE_TEXT_IN_PROTOCOL     *cIn;
  EFI_SIMPLE_TEXT_OUT_PROTOCOL    *cOut;
///
  EFI_INPUT_KEY 				  KeyCode;
  CHAR16                          Unum = 0; ///
//  EFI_STATUS                      Status;
	

  EfiInitializeDriverLib (ImageHandle, SystemTable);

  //
  // Console initial.
  //
  cOut = gST->ConOut;
  cIn  = gST->ConIn;

/// 
  cOut->OutputString (cOut, L"Hello EFI123 ! \t"); //add "\n" show enter&tab 
  cOut->OutputString (cOut, L"Please entry number:"); 

  getch(&KeyCode);
  wprintf(L"%c", (CHAR16) KeyCode.UnicodeChar); //wprintf(L"%c \n", (CHAR16) KeyCode.UnicodeChar);
  Unum=(CHAR16) KeyCode.UnicodeChar + 1; // wprintf(L"%c",Unum); //

 
//  DrawHorizontalLine (*pGraph, Image, 355, 50 , 90, SOLAR_WHITE, FullLine);
 

///  
  return EFI_SUCCESS;
}
