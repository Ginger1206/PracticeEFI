/*++ 

Copyright (c) 2008 - 2009, Solar Studio                                                         
         
Module Name:

  TestSolarLib.c

Abstract:

  This is a test environment of Solar libraries.

--*/

#include "Efi.h"
#include "EfiDriverLib.h"
#include "Solar.h"
#include "SolarConsoleLib.h"	// Library : SolarConsoleLib

EFI_APPLICATION_ENTRY_POINT (TestSolarLib)

EFI_STATUS
TestSolarLib (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  EFI_SIMPLE_TEXT_IN_PROTOCOL     *cIn;
  EFI_SIMPLE_TEXT_OUT_PROTOCOL    *cOut;

  EfiInitializeDriverLib (ImageHandle, SystemTable);

  //
  // Console initial.
  //
  cOut = gST->ConOut;
  cIn  = gST->ConIn;
  
  cOut->OutputString (cOut, L"This is a test program for Solar libraries!");
  
  if (InitiateSolarConsoleLib () != SOLAR_SUCCESS) {
    cOut->OutputString (cOut, L"\n>> SolarConsleLib isn't ready.");
  } else {
    wprintf (L"\n>> SolarConsleLib is ready.");
  }

  
  return EFI_SUCCESS;
}
