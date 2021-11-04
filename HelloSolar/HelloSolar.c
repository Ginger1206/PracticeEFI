/*++ 

Copyright (c) 2008 - 2009, Solar Studio                                                         
         
Module Name:

  HelloSolar.c

Abstract:

  A hello demo module for EFI shell.

--*/

#include "Efi.h"
#include "EfiDriverLib.h"
#include "Solar.h"
#include "SolarConsoleLib.h"

EFI_APPLICATION_ENTRY_POINT (HelloSolar)

EFI_STATUS
HelloSolar (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  EFI_SIMPLE_TEXT_IN_PROTOCOL     *cIn;
  EFI_SIMPLE_TEXT_OUT_PROTOCOL    *cOut;
  //EFI_INPUT_KEY					  KeyCode;

  EfiInitializeDriverLib (ImageHandle, SystemTable); //SEC setup
  //getch(&KeyCode); //for test 210323
  //
  // Console initial.
  //
  cOut = gST->ConOut;
  cIn  = gST->ConIn;
  
  cOut->OutputString (cOut, L"Hello Solar !");
  
  return EFI_SUCCESS;
}
