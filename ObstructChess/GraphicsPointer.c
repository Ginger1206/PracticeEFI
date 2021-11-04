/*++ 

Copyright (c) 2008 - 2009, Solar Studio                                                         
         
Module Name:

  GraphicsPointer.c

Abstract:

  Pointer for graphics interface.

--*/

extern "C" {
  #include "Efi.h"
  #include "EfiDriverLib.h"
  #include "Solar.h"
  #include "SolarGraphicsLib.h"
  #include EFI_PROTOCOL_DEFINITION (SimplePointer)
  #include EFI_GUID_DEFINITION (PrimaryConsoleInDevice)
}

#include "GraphicsPointer.h"

#define SIMPLE_POINTER_BIT_MAP_WIDTH       16
#define SIMPLE_POINTER_BIT_MAP_HEIGHT      16

// Mouse cursor types
#define GRAPHICS_POINTER_ARROW             0
#define GRAPHICS_POINTER_HOURGLASS         1
#define GRAPHICS_POINTER_ADJUST_VERTICAL   2
#define GRAPHICS_POINTER_ADJUST_SLASH      3
#define GRAPHICS_POINTER_ADJUST_HORIZONTAL 4
#define GRAPHICS_POINTER_ADJUST_BACKSLASH  5
#define GRAPHICS_POINTER_EDIT              6
#define GRAPHICS_POINTER_END               7

// Global variables
static EFI_SIMPLE_POINTER_PROTOCOL   *pSimplePointerInterface;
static EFI_GRAPHICS_OUTPUT_PROTOCOL  *pGraphics;
#ifdef EFI_NT_EMULATOR
static EFI_GRAPHICS_OUTPUT_BLT_PIXEL PointerImage[SIMPLE_POINTER_BIT_MAP_HEIGHT][SIMPLE_POINTER_BIT_MAP_WIDTH];
static EFI_GRAPHICS_OUTPUT_BLT_PIXEL SimplePointerBackgroundBackupBuffer[SIMPLE_POINTER_BIT_MAP_HEIGHT][SIMPLE_POINTER_BIT_MAP_WIDTH];
#else   // EFI_NT_EMULATOR
static UINT32                        *VideoBuffer;
static UINT32                        *SimplePointerBackgroundBackupBuffer = (UINT32 *) NULL;
#endif  // EFI_NT_EMULATOR
static UINT32                        ScreenWidth, ScreenHeight;
static INT32                         PointerX, PointerY;  // Simple pointer's location
static UINT8                         PointerType;

// Mouse cursor (regular shape)
static
UINT8
imgPointerArrow [SIMPLE_POINTER_BIT_MAP_HEIGHT][SIMPLE_POINTER_BIT_MAP_WIDTH] = {
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 2, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
  { 1, 2, 2, 1, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 2, 1, 1, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 1, 0, 0, 1, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 1, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 }
};

// Mouse cursor (hourglass)
static
UINT8
imgPointerHourglass [SIMPLE_POINTER_BIT_MAP_HEIGHT][SIMPLE_POINTER_BIT_MAP_WIDTH] = {
  { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
  { 0, 0, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 0, 0, 0 },
  { 0, 0, 1, 2, 2, 1, 2, 1, 2, 1, 2, 2, 1, 0, 0, 0 },
  { 0, 0, 1, 2, 2, 2, 1, 2, 1, 2, 2, 2, 1, 0, 0, 0 },
  { 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 2, 2, 1, 2, 2, 1, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 1, 1, 2, 2, 2, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 1, 1, 2, 1, 2, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0, 0 },
  { 0, 0, 1, 1, 2, 2, 2, 1, 2, 2, 2, 1, 1, 0, 0, 0 },
  { 0, 0, 1, 2, 2, 2, 1, 2, 1, 2, 2, 2, 1, 0, 0, 0 },
  { 0, 0, 1, 2, 2, 1, 2, 1, 2, 1, 2, 2, 1, 0, 0, 0 },
  { 0, 0, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 0, 0, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 }
};

// Mouse cursor at 0 degree
static
UINT8
imgPointerVertical [SIMPLE_POINTER_BIT_MAP_HEIGHT][SIMPLE_POINTER_BIT_MAP_WIDTH] = {
  { 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 2, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 2, 2, 2, 1, 2, 2, 2, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 2, 2, 2, 1, 2, 2, 2, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 2, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0 }
};

// Mouse cursor at 45 degree
static
UINT8
imgPointerSlash [SIMPLE_POINTER_BIT_MAP_HEIGHT][SIMPLE_POINTER_BIT_MAP_WIDTH] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 1, 2 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 2 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 1, 2 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 2, 2 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 2 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 2, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 2, 2, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 2, 1, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 2, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

// Mouse cursor at 90 degree
static
UINT8
imgPointerHorizontal [SIMPLE_POINTER_BIT_MAP_HEIGHT][SIMPLE_POINTER_BIT_MAP_WIDTH] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0 },
  { 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0 },
  { 0, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 0 },
  { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2 },
  { 0, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 0 },
  { 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0 },
  { 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

// Mouse cursor at 315 degree
static
UINT8
imgPointerBackslash [SIMPLE_POINTER_BIT_MAP_HEIGHT][SIMPLE_POINTER_BIT_MAP_WIDTH] = {
  { 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 2, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 2, 1, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 2, 2, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 2, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 2 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 2, 2 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 1, 2 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 2 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 1, 2 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2 }
};

static
EFI_STATUS
SavePointerBackground (
  void
  )
{
#ifdef EFI_NT_EMULATOR
  pGraphics->Blt (
    pGraphics,
    (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) &SimplePointerBackgroundBackupBuffer,
    EfiBltVideoToBltBuffer,
    PointerX,
    PointerY,
    0,
    0,
    SIMPLE_POINTER_BIT_MAP_WIDTH,
    SIMPLE_POINTER_BIT_MAP_HEIGHT,
    0
    );
  EfiCopyMem (&PointerImage, SimplePointerBackgroundBackupBuffer, SIMPLE_POINTER_BIT_MAP_HEIGHT * SIMPLE_POINTER_BIT_MAP_WIDTH * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
#else   // EFI_NT_EMULATOR
  UINT32 *VideoPtr, *BufferPtr;

  VideoPtr = VideoBuffer + (PointerY * ScreenWidth) + PointerX;
  BufferPtr = SimplePointerBackgroundBackupBuffer;

  for (int i = 0; i < SIMPLE_POINTER_BIT_MAP_HEIGHT; i++) {
    // Copy video to buffer
    EfiCopyMem (BufferPtr, VideoPtr, SIMPLE_POINTER_BIT_MAP_WIDTH * sizeof (UINT32));
    // Forward to next scanline
    BufferPtr += SIMPLE_POINTER_BIT_MAP_WIDTH;
    VideoPtr  += ScreenWidth;
  }
#endif  // EFI_NT_EMULATOR

  return EFI_SUCCESS;
}

EFI_STATUS
RestorePointerBackground (
  VOID
  )
{
#ifdef EFI_NT_EMULATOR
  pGraphics->Blt (
    pGraphics,
    (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) &SimplePointerBackgroundBackupBuffer,
    EfiBltBufferToVideo,
    0,
    0,
    PointerX,
    PointerY,
    SIMPLE_POINTER_BIT_MAP_WIDTH,
    SIMPLE_POINTER_BIT_MAP_HEIGHT,
    0
    );
#else   // EFI_NT_EMULATOR
  UINT32 *VideoPtr, *BufferPtr;

  VideoPtr = VideoBuffer + (PointerY * ScreenWidth) + PointerX;
  BufferPtr = SimplePointerBackgroundBackupBuffer;

  for (int i = 0; i < SIMPLE_POINTER_BIT_MAP_HEIGHT; i++) {
    // Copy video to buffer
    EfiCopyMem (VideoPtr, BufferPtr, SIMPLE_POINTER_BIT_MAP_WIDTH * sizeof (UINT32));
    // Forward to next scanline
    BufferPtr += SIMPLE_POINTER_BIT_MAP_WIDTH;
    VideoPtr  += ScreenWidth;
  }
#endif  // EFI_NT_EMULATOR

  return EFI_SUCCESS;
}

VOID
DrawCursor (
  VOID
  )
{
  UINT8  Point, *PtrImage;
#ifndef EFI_NT_EMULATOR
  UINT32 *VideoPtr;
#endif  // EFI_NT_EMULATOR

  // Get the first backup image for SimplePointer process.
  SavePointerBackground ();

  switch (PointerType) {
  case GRAPHICS_POINTER_HOURGLASS:         PtrImage = (UINT8 *) imgPointerHourglass;  break;
  case GRAPHICS_POINTER_ADJUST_VERTICAL:   PtrImage = (UINT8 *) imgPointerVertical;   break;
  case GRAPHICS_POINTER_ADJUST_SLASH:      PtrImage = (UINT8 *) imgPointerSlash;      break;
  case GRAPHICS_POINTER_ADJUST_HORIZONTAL: PtrImage = (UINT8 *) imgPointerHorizontal; break;
  case GRAPHICS_POINTER_ADJUST_BACKSLASH:  PtrImage = (UINT8 *) imgPointerBackslash;  break;
  default:                                 PtrImage = (UINT8 *) imgPointerArrow;
  }

#ifndef EFI_NT_EMULATOR
  VideoPtr = VideoBuffer + (PointerY * ScreenWidth) + PointerX;
#endif  // EFI_NT_EMULATOR

  for (int i = 0; i < SIMPLE_POINTER_BIT_MAP_HEIGHT; i++) {
    for (int j = 0; j < SIMPLE_POINTER_BIT_MAP_WIDTH; j++) {
      if ((Point = *PtrImage++) != 0) {
#ifdef EFI_NT_EMULATOR
        PointerImage[i][j].Blue  = (Point == 1) ? 0x00 : 0xff;
        PointerImage[i][j].Green = (Point == 1) ? 0x00 : 0xff;
        PointerImage[i][j].Red   = (Point == 1) ? 0x00 : 0xff;
#else   // EFI_NT_EMULATOR
        *(VideoPtr + j) = (Point == 1) ? 0x000000 : 0xffffff;
#endif  // EFI_NT_EMULATOR
      }
    }
#ifndef EFI_NT_EMULATOR
    // Forward to next scanline
    VideoPtr += ScreenWidth;
#endif  // EFI_NT_EMULATOR
  }
#ifdef EFI_NT_EMULATOR
  pGraphics->Blt (
    pGraphics,
    (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) &PointerImage,
    EfiBltBufferToVideo,
    0,
    0,
    PointerX,
    PointerY,
    SIMPLE_POINTER_BIT_MAP_WIDTH,
    SIMPLE_POINTER_BIT_MAP_HEIGHT,
    0
    );   
#endif  // EFI_NT_EMULATOR
}

SOLAR_STATUS
EnableSimplePointer (
  VOID
  )
{
  SOLAR_STATUS Status = SOLAR_SUCCESS;

  // Check the device status.
  if ((Status = pSimplePointerInterface->Reset (pSimplePointerInterface, TRUE)) != EFI_SUCCESS) {
    return Status;
  }

  // Set simple pointer initial state.
  pSimplePointerInterface->Mode->LeftButton  = true;
  pSimplePointerInterface->Mode->RightButton = true;
  pSimplePointerInterface->Mode->ResolutionX = 3;
  pSimplePointerInterface->Mode->ResolutionY = 3;
  pSimplePointerInterface->Mode->ResolutionZ = 0;

  // Allocate a memory area for |SimplePointerBackgroundBackupImage|.
#ifndef EFI_NT_EMULATOR
  SimplePointerBackgroundBackupBuffer = (UINT32 *) EfiLibAllocatePool (SIMPLE_POINTER_BIT_MAP_HEIGHT * SIMPLE_POINTER_BIT_MAP_WIDTH * sizeof (UINT32));
  if (SimplePointerBackgroundBackupBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
#endif  // EFI_NT_EMULATOR
  // Set initial mouse cursor to be central of screen
  PointerX = ScreenWidth  / 2;
  PointerY = ScreenHeight / 2;
  DrawCursor ();

  return Status;
}

VOID
DisableSimplePointer (
  VOID
  )
{
  RestorePointerBackground ();
  if (SimplePointerBackgroundBackupBuffer != NULL) {
    gBS->FreePool (SimplePointerBackgroundBackupBuffer);
  }
}

SOLAR_STATUS
GetPointerState (
  OUT POINTER_STATE *PtrState
  )
{
  EFI_SIMPLE_POINTER_STATE PointerState;
  EFI_STATUS               Status;

  // Get mouse state
  //while ((Status = pSimplePointerInterface->GetState (pSimplePointerInterface, &PointerState)) != EFI_SUCCESS);
  Status = pSimplePointerInterface->GetState (pSimplePointerInterface, &PointerState);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  // Proccess mouse movement
  if (PointerState.RelativeMovementX || PointerState.RelativeMovementY) {
    RestorePointerBackground ();

    // Get new mouse position
    PointerX += PointerState.RelativeMovementX;
    PointerY += PointerState.RelativeMovementY;
    if (PointerX < 0) {
      PointerX = 0;
    } else if (PointerX >= (INT32) ScreenWidth) {
      PointerX = ScreenWidth - 1;
    }
    if (PointerY < 0) {
      PointerY = 0;
    } else if (PointerY >= (INT32) ScreenHeight) {
      PointerY = ScreenHeight - 1;
    }
    DrawCursor ();
  }

  PtrState->X       = PointerX;
  PtrState->Y       = PointerY;
  PtrState->LButton = PointerState.LeftButton;
  PtrState->RButton = PointerState.RightButton;

  return EFI_SUCCESS;
}

SOLAR_STATUS
InitiateGraphicsPointer (
  EFI_GRAPHICS_OUTPUT_PROTOCOL **pGraph,
  UINT32                       Width,
  UINT32                       Height
  )
{
  SOLAR_STATUS Status = SOLAR_SUCCESS;
  EFI_GUID     gEfiSimplePointerProtocolGuid = EFI_SIMPLE_POINTER_PROTOCOL_GUID;
  EFI_HANDLE   *HandleBuffer;
  UINTN        HandleCount;

  // Initiate the global variables.
  pGraphics    = *pGraph;
#ifndef EFI_NT_EMULATOR
  VideoBuffer  = (UINT32 *) (*pGraph)->Mode->FrameBufferBase;
#endif  // EFI_NT_EMULATOR
  PointerType  = GRAPHICS_POINTER_ARROW;
  ScreenWidth  = Width;
  ScreenHeight = Height;

  // Locate Simple Pointer Device Protocol.
  /*
  Status = gBS->LocateProtocol (
                  &gEfiSimplePointerProtocolGuid,
                  NULL,
                  (VOID **) &pSimplePointerInterface
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }
  */
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,   
                  &gEfiPrimaryConsoleInDeviceGuid,
                  NULL,
                  &HandleCount, 
                  &HandleBuffer
                  );
  if (!EFI_ERROR(Status)) {
    Status = gBS->HandleProtocol (HandleBuffer[0], 
                    &gEfiSimplePointerProtocolGuid, 
                    (VOID **) &pSimplePointerInterface
                    );
  }

  return Status;
}