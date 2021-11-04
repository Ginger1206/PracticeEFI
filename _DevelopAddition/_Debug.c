/*++ 

Copyright (c) 2008 - 2009, Solar Studio                                                         
         
Module Name:

  _Debug.c

Abstract:

  This is a debug solution for Solar System.

--*/

#include "_DevelopAdditionSetup.h"

#ifdef __DEBUG__

#ifdef __CPP__
extern "C" {
#endif  //__CPP__

  #include "Efi.h"
  #include "EfiDriverLib.h"
  #include "Solar.h"
  #include "SolarConsoleLib.h"	// Library : SolarConsoleLib

#ifdef __CPP__
}
#endif  //__CPP__

EFI_INPUT_KEY DebugKeyCode;

SOLAR_STATUS
SOLAR_API
_DebugWaitKey (void)
/*
  Function: _DebugWaitKey

  Discription:
    Wait any key to continu.

  return:
    SOLAR_STATUS = SOLAR_SUCCESS
*/
{
  getch(&DebugKeyCode);

  return SOLAR_SUCCESS;
}

SOLAR_STATUS
SOLAR_API
_DebugPrintf (
  UINT16       X, 
  UINT16       Y, 
  UINT8        FgColor, 
  UINT8        BgColor, 
  CONST CHAR16 *Format,
  UINT64       DebugValue
  )
/*
  Function: _DebugPrintf

  Discription:
    Print a debug string and wait any key to continu.

  return:
    SOLAR_STATUS = SOLAR_SUCCESS
*/
{
  PAWPrintf (X, Y, FgColor, BgColor, Format, DebugValue);
  getch(&DebugKeyCode);

  return SOLAR_SUCCESS;
}

#else   //__DEBUG__
void empty (void) {}
#endif  //__DEBUG__