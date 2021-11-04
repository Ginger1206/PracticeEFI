/*++ 

Copyright (c) 2010, Inventec Corporation
         
Module Name:

  ConsoleControl.c

Abstract:

  This is the ConsoleControl switch utility.

--*/

#include "Efi.h"
#include "EfiCommon.h"
#include "EfiDriverLib.h"
#include "EfiPrintLib.h"
#include "GraphicsLib.h"
#include EFI_PROTOCOL_DEFINITION (GraphicsOutput)

EFI_STATUS
EFIAPI
ConsoleControl (
  IN EFI_HANDLE                        ImageHandle,
  IN EFI_SYSTEM_TABLE                  *SystemTable
  )
{
  EFI_STATUS                           Status;
  EFI_CONSOLE_CONTROL_PROTOCOL         *pConsole;

  
  // Initial the variables.
  Status  = EFI_SUCCESS;
  
  EfiInitializeDriverLib (ImageHandle, SystemTable);
  
  // Locate CCP.
  Status = gBS->LocateProtocol (
                &gEfiConsoleControlProtocolGuid,
                NULL,
                (void **) &pConsole
                );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  
  // Set console control mode for graphics using.
  //pConsole->SetMode(pConsole, EfiConsoleControlScreenGraphics);
  
  // Set console control mode for text using.
  pConsole->SetMode(pConsole, EfiConsoleControlScreenText);

  return EFI_SUCCESS;
}
