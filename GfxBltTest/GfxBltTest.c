/*++ 

Copyright (c) 2010, Inventec Corporation
         
Module Name:

  GfxBltTest.c

Abstract:

  A graphics BLT test program for EFI shell.

--*/

#include "Efi.h"
#include "EfiCommon.h"
#include "EfiDriverLib.h"
#include "EfiPrintLib.h"
#include "GraphicsLib.h"
#include EFI_PROTOCOL_DEFINITION (GraphicsOutput)

typedef enum {
  COLOR8,
  COLOR16,
  COLOR32
} COLOR_TYPE;

typedef enum {
  RGB,
  BGR
} RGB_TYPE;

UINTN
kbhit (
  EFI_INPUT_KEY *KeyCode
  )
{
    gST->ConIn->ReadKeyStroke(gST->ConIn, KeyCode);

    if (KeyCode->ScanCode == SCAN_NULL && KeyCode->UnicodeChar == CHAR_NULL) {
      return (0);
    }

    return (1);
}

void
getch (
  EFI_INPUT_KEY *KeyCode
  )
{
    do {
        gST->ConIn->ReadKeyStroke(gST->ConIn, KeyCode);
    } while(KeyCode->ScanCode == SCAN_NULL && KeyCode->UnicodeChar == CHAR_NULL);
}

EFI_STATUS
QueryRequestMode (
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL     *pGraph,
  IN  UINT32                           HorizontalResolution,
  IN  UINT32                           VerticalResolution,
  OUT UINT32                           *ModeNum
  )
{
  EFI_STATUS                           Status;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *ModeInfo;
  UINTN                                SizeOfInfo;
  UINT32                               i;

  Status   = EFI_SUCCESS;
  *ModeNum = 0xffffffff;
  for (i = 0; i < (1 << sizeof(UINT32)) ;i++) {
    Status = pGraph->QueryMode (
                     pGraph,
                     i,
                     &SizeOfInfo,
                     &ModeInfo
                     );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    if (ModeInfo->HorizontalResolution == HorizontalResolution &&
        ModeInfo->VerticalResolution == VerticalResolution) {
      *ModeNum = i;
      pGraph->SetMode (pGraph, *ModeNum);
      return Status;
    }
  }

  return Status;
}

void
ReturnToConsoleTextMode (
  EFI_GRAPHICS_OUTPUT_PROTOCOL *pGraph
  ) 
{
  UINT32                       ModeNum;
  
  QueryRequestMode (pGraph, 640, 480, &ModeNum); 
  pGraph->SetMode(pGraph, ModeNum);
}

EFI_STATUS
EFIAPI
GfxBltTest (
  IN EFI_HANDLE                        ImageHandle,
  IN EFI_SYSTEM_TABLE                  *SystemTable
  )
{
  EFI_STATUS                           Status;
  EFI_GRAPHICS_OUTPUT_PROTOCOL         *pGraph;
  EFI_CONSOLE_CONTROL_PROTOCOL         *pConsole;
  UINT32                               i;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *ModeInfo;
  UINTN                                SizeOfInfo;
  UINT32                               ModeNum;
  UINT32                               HorizontalResolution;
  UINT32                               VerticalResolution;
  UINT32                               RedMask;
  UINT32                               GreenMask;
  UINT32                               BlueMask;
  UINT8                                RedShift;
  UINT8                                GreenShift;
  UINT8                                BlueShift;
  UINT8                                RgbType;
  UINT8                                ColorType;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL        Blt;
  EFI_INPUT_KEY                        KeyCode;
  
  // Initial the variables.
  Status  = EFI_SUCCESS;
  ModeNum = HorizontalResolution = VerticalResolution = RedMask = GreenMask = BlueMask = RedShift = GreenShift = BlueShift = ColorType = 0;
  
  EfiInitializeDriverLib (ImageHandle, SystemTable);  // Check this solution in the EDK2 !!!!!
  gST->ConOut->OutputString (gST->ConOut, L"\nGfxBltTest Starts....");
  
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
  pConsole->SetMode(pConsole, EfiConsoleControlScreenGraphics);
  
  // Get GOP handle.
  Status = gBS->HandleProtocol (
                gST->ConsoleOutHandle, 
                &gEfiGraphicsOutputProtocolGuid, 
                (void **) &pGraph
                );
   
  // Get all graphics mode then set the maximum in the platform.
  for (i = 0; i < (1 << sizeof(UINT32)); i++) {
    
    // Get GOP mode information.
    Status = pGraph->QueryMode (
                     pGraph,
                     i,
                     &SizeOfInfo,
                     &ModeInfo
                     );
    if (EFI_ERROR (Status)) {
      if (i < 1) {
        // If there is not any mode in the platform, exit the program.
        gST->ConOut->OutputString (gST->ConOut, L"No mode in the platform.\n");
        return Status;
      } else {
        // Set the maximum mode for the platform and break this loop.
        pGraph->SetMode (pGraph, ModeNum);
        break;
      }
    }
    
    // Set the mode data in to the variable if the Horizontal/Vertical Resolution more than old.
    if ((ModeInfo->HorizontalResolution > HorizontalResolution) || 
        (ModeInfo->HorizontalResolution == HorizontalResolution && ModeInfo->PixelInformation.GreenMask > GreenMask)
       ) {
      HorizontalResolution = ModeInfo->HorizontalResolution;
      VerticalResolution = ModeInfo->VerticalResolution;
      RedMask = ModeInfo->PixelInformation.RedMask;
      GreenMask = ModeInfo->PixelInformation.GreenMask;
      BlueMask = ModeInfo->PixelInformation.BlueMask;
      
      // Set color shift count.
      if (RedMask >= BlueMask) {
        RgbType = BGR;
        if (RedMask >= 0x10000) {
          RedShift = 0x10;
          GreenShift = 0x08;
          BlueShift = 0x00;
          ColorType = COLOR32;
        } else if (RedMask >= 0x100) {
          RedShift = 0x08;
          GreenShift = 0x04;
          BlueShift = 0x00;
          ColorType = COLOR16;
        } else {
          RedShift = 0x05;
          GreenShift = 0x02;
          BlueShift = 0x00;
          ColorType = COLOR8;
        }
      } else {
        RgbType = RGB;
        if (BlueMask >= 0x10000) {
          RedShift = 0x00;
          GreenShift = 0x08;
          BlueShift = 0x10;
          ColorType = COLOR32;
        } else if (BlueMask >= 0x100) {
          RedShift = 0x08;
          GreenShift = 0x04;
          BlueShift = 0x08;
          ColorType = COLOR16;
        } else {
          RedShift = 0x00;
          GreenShift = 0x03;
          BlueShift = 0x06;
          ColorType = COLOR8;
        }
      }
      
      ModeNum = i;
    }
  }
   
  Blt.Red   = 0xff;
  Blt.Green = 0x7f;
  Blt.Blue  = 0x00;
  
  Status = pGraph->Blt (
           pGraph,
           &Blt,
           EfiBltVideoFill,
           0,
           0,
           0,
           0,
           HorizontalResolution,
           VerticalResolution,
           0
           );
  if (EFI_ERROR (Status)) {
    gST->ConOut->OutputString (gST->ConOut, L"BLT 1024x768 output fail !!\n");
    return Status;
  }
  
  getch (&KeyCode);
  
  Blt.Red   = 0x00;
  Blt.Green = 0x7f;
  Blt.Blue  = 0xff;
  
  Status = pGraph->Blt (
           pGraph,
           &Blt,
           EfiBltVideoFill,
           0,
           0,
           0,
           0,
           HorizontalResolution,
           VerticalResolution,
           0
           );
  if (EFI_ERROR (Status)) {
    gST->ConOut->OutputString (gST->ConOut, L"BLT 1024x768 output fail !!\n");
    return Status;
  }
  
  getch (&KeyCode);
  
  ReturnToConsoleTextMode (pGraph);
  pConsole->SetMode(pConsole, EfiConsoleControlScreenText);
  
  gST->ConOut->OutputString (gST->ConOut, L"Done !!\n");
  return EFI_SUCCESS;
}
