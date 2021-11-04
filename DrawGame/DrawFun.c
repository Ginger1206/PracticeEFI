/*++ 

Copyright (c) 2008 - 2010, Solar Studio                                                         
         
Module Name:

  ImageBlock.c

Abstract:

  This is an EFI-shell game program that calls ObstructChess.

--*/

extern "C" {
  #include "Efi.h"
  #include "EfiDriverLib.h"
  #include "Solar.h"  
  #include "SolarConsoleLib.h"  // Library : SolarConsoleLib
  #include "SolarGraphicsLib.h" // Library : SolarGraphicsLib
  #include EFI_PROTOCOL_DEFINITION (ConsoleControl)
}
#include "SolarCppLib.h"        // Library : SolarCppLib (Stardart C++ module implement)
#include "DrawGame.h"
#ifdef __DEBUG__
  #include "../_DevelopAddition/_Debug.h"
#endif

VOID *
memcpy (
  OUT VOID        *Dest,
  IN  const VOID  *Src,
  IN  UINTN       Count
  )
{
  volatile UINT8  *Ptr;
  const    UINT8  *Source;
  
  for (Ptr = Dest, Source = Src; Count > 0; Count--, Source++, Ptr++) {
    *Ptr = *Source;
  }

  return Dest;
}
