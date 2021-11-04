/*++ 

Copyright (c) 2008 - 2009, Solar Studio                                                         
         
Module Name:

  GraphicsPointer.h

Abstract:

  Pointer for graphics interface.

--*/

#ifndef _GRAPHICS_POINTER_H_
#define _GRAPHICS_POINTER_H_

typedef struct _POINTER_STATE {
  INT32   X;
  INT32   Y;
  BOOLEAN LButton;
  BOOLEAN RButton;
} POINTER_STATE;

SOLAR_STATUS
InitiateGraphicsPointer (
  EFI_GRAPHICS_OUTPUT_PROTOCOL **pGraph,
  UINT32                       Width,
  UINT32                       Height
  );

SOLAR_STATUS
EnableSimplePointer (
  VOID
  );

VOID
DisableSimplePointer (
  VOID
  );

SOLAR_STATUS
GetPointerState (
  OUT POINTER_STATE *PtrState
  );

#endif //_GRAPHICS_POINTER_H_

