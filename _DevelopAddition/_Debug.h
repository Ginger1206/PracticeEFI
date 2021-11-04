/*++ 

Copyright (c) 2008 - 2009, Solar Studio                                                         
         
Module Name:

  _Debug.h

Abstract:

  This is a debug solution for Solar System.

--*/

#ifdef __DEBUG__
#ifndef __DEBUG_H_
#define __DEBUG_H_

#define __INT3    _asm { int 3 }

extern EFI_INPUT_KEY DebugKeyCode;

SOLAR_STATUS
SOLAR_API
_DebugWaitKey (void);

SOLAR_STATUS
SOLAR_API
_DebugPrintf (
  UINT16       X, 
  UINT16       Y, 
  UINT8        FgColor, 
  UINT8        BgColor, 
  CONST CHAR16 *Format,
  UINT64       DebugValue
  );

#endif  //__DEBUG_H_
#endif  //__DEBUG__