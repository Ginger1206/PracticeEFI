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
#include "ImageBlock.h"
#ifdef __DEBUG__
  #include "../_DevelopAddition/_Debug.h"
 // #include "ObstructChessTestFunction.h"
#endif

extern EFI_GRAPHICS_OUTPUT_BLT_PIXEL TransparentColor;

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Clasee Definition - STARTS ////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/////////////////////////
// class : IMAGE_BLOCK //
/////////////////////////

// IMAGE_BLOCK construction function
IMAGE_BLOCK::IMAGE_BLOCK () {
  Foreground.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) NULL;
  Background.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) NULL;
}

// IMAGE_BLOCK deconstruction function
IMAGE_BLOCK::~IMAGE_BLOCK () 
{
  if (Foreground.BltBuffer) {
    gBS->FreePool (Foreground.BltBuffer);
  }

  if (Background.BltBuffer) {
    gBS->FreePool (Background.BltBuffer);
  }
}

// IMAGE_BLOCK send a graphics protocol and set block width / height size then allocate the memory for object.
SOLAR_STATUS SOLAR_API IMAGE_BLOCK::BlockInit (EFI_GRAPHICS_OUTPUT_PROTOCOL *pG, UINT16 Width, UINT16 Height, UINT32 DefaultBlockTextColor, UINT32 DefaultBlockColor)
{
  pGraph            = pG;
  Foreground.Width  = Background.Width  = Width;
  Foreground.Height = Background.Height = Height;
  BlockColor        = DefaultBlockColor;
  BlockTextColor    = DefaultBlockTextColor;


  if ((Foreground.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool (Foreground.Width * Foreground.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL))) == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  if ((Background.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool (Background.Width * Background.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL))) == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  DrawRectangle (pGraph, &Foreground, 0, 0, Width - 1, Height - 1, BlockColor, FullLine, Fill, BlockColor, 0);

  return SOLAR_SUCCESS;
}

// IMAGE_BLOCK display the block.
SOLAR_STATUS SOLAR_API IMAGE_BLOCK::Display (SOLAR_IMAGE_INPUT BackgroundSourceImage, UINT16 X, UINT16 Y, BOOLEAN SaveBackground) 
{
  SOLAR_STATUS Status;

  if (SaveBackground) {
    // Save the background image.
    Status = pGraph->Blt (
               pGraph,
               Background.BltBuffer,
               EfiBltVideoToBltBuffer,
               X,
               Y,
               0,
               0,
               Background.Width,
               Background.Height,
               0
               );
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }
  }

  // Put the foreground image on the screen.
  Status = TransparentBltBufferToVideo (
             pGraph,
             &Foreground,
             &BackgroundSourceImage,
             X,
             Y,
             TransparentColor
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  // Save the image position of top-left.
  PresentlyX = X;
  PresentlyY = Y;

  return SOLAR_SUCCESS;
}

// IMAGE_BLOCK hidden the block.
SOLAR_STATUS SOLAR_API IMAGE_BLOCK::Hidden (VOID) 
{
  SOLAR_STATUS Status;

  // Put back the background image on the screen.
  Status = pGraph->Blt (
             pGraph,
             Background.BltBuffer,
             EfiBltBufferToVideo,
             0,
             0,
             PresentlyX,
             PresentlyY,
             Background.Width,
             Background.Height,
             0
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return SOLAR_SUCCESS;
}

//////////////////////////
// class : SELECT_BLOCK //
//////////////////////////

// IMAGE_BLOCK construction function
SELECT_BLOCK::SELECT_BLOCK () 
{
  ItemContent = (CHAR16 **) NULL;
  ItemNum = 0;
  ItemPtr = 0;
}

// IMAGE_BLOCK deconstruction function
SELECT_BLOCK::~SELECT_BLOCK () 
{
  if (ItemNum && ItemContent) {
    for (UINT16 i = 0; i < ItemNum; i ++) {
      if (*(ItemContent + i)) {
        gBS->FreePool (*(ItemContent + i));
      }
    }
    gBS->FreePool (ItemContent);
  }
}

// SELECT_BLOCK set item number
SOLAR_STATUS SOLAR_API SELECT_BLOCK::SetItemNum (UINT16 Num) 
{
  ItemNum = Num;
  
  if ((ItemContent = (CHAR16 **) EfiLibAllocatePool (ItemNum * sizeof(UINTN))) == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  for (UINT16 i = 0; i < ItemNum; i ++) {
    *(ItemContent + i) = (CHAR16 *) NULL;
  }

  return SOLAR_SUCCESS;
}

// SELECT_BLOCK set one item content.
SOLAR_STATUS SOLAR_API SELECT_BLOCK::SetItemContent (UINT16 ItemNo, CHAR16 *Content) 
{
  if (ItemNo < ItemNum) {
    if (*(ItemContent + ItemNo)) {
      gBS->FreePool (*(ItemContent + ItemNo));
    }

    if ((*(ItemContent + ItemNo) = (CHAR16 *) EfiLibAllocatePool ((EfiStrLen(Content) + 1) * sizeof(CHAR16))) == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
  } else {
    return EFI_OUT_OF_RESOURCES;
  }

  EfiStrCpy (*(ItemContent + ItemNo), Content);
  PrintString (pGraph, &Foreground, ITEM_MARGIN, ITEM_MARGIN + ItemNo * ITEM_HEIGHT, BlockTextColor, *(ItemContent + ItemNo));

  return SOLAR_SUCCESS;
}

// SELECT_BLOCK focuse an item
SOLAR_STATUS SOLAR_API SELECT_BLOCK::FocuseItem (UINT16 ItemNo, UINT32 FocuseItemTextColor, UINT32 FocuseItemBlockColor) 
{
  if (ItemNo >= ItemNum) {
    return EFI_OUT_OF_RESOURCES;
  }

  if (ItemNo != ItemPtr) {
    DrawRectangle (pGraph, &Foreground, 0, (ItemPtr * ITEM_HEIGHT), Foreground.Width - 1, ((ItemPtr + 1) * ITEM_HEIGHT - 1), BlockColor, FullLine, Fill, BlockColor, 0);
    PrintString (pGraph, &Foreground, ITEM_MARGIN, (ITEM_MARGIN + ItemPtr * ITEM_HEIGHT), BlockTextColor, *(ItemContent + ItemPtr));
  }

  ItemPtr = ItemNo;

  DrawRectangle (pGraph, &Foreground, 0, (ItemPtr * ITEM_HEIGHT), Foreground.Width - 1, ((ItemPtr + 1) * ITEM_HEIGHT - 1), FocuseItemBlockColor, FullLine, Fill, FocuseItemBlockColor, 0);
  PrintString (pGraph, &Foreground, ITEM_MARGIN, (ITEM_MARGIN + ItemPtr * ITEM_HEIGHT), FocuseItemTextColor, *(ItemContent + ItemPtr));

  return SOLAR_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Function Definition - STARTS //////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////
// Function : SelectItem () //
//////////////////////////////

UINT16 
SelectItem (
  EFI_GRAPHICS_OUTPUT_PROTOCOL *pG, 
  SOLAR_IMAGE_INPUT            BackgroundSourceImage, 
  UINT16                       Width, 
  UINT16                       Height, 
  UINT16                       BlockTLX, 
  UINT16                       BlockTLY, 
  UINT32                       DefaultBlockTextColor, 
  UINT32                       DefaultBlockColor, 
  UINT32                       FocuseItemTextColor, 
  UINT32                       FocuseItemBlockColor, 
  UINT16                       ItemNum, 
  CHAR16                       *ItemContent
  )
{
  SELECT_BLOCK  SelectBlock;
  CHAR16        TmpStr[0x100];
  UINT16        ItemPtr  = 0;
  BOOLEAN       isSelect = false;
  EFI_INPUT_KEY KeyCode;
  
  SelectBlock.BlockInit (pG, Width, Height, DefaultBlockTextColor, DefaultBlockColor);
  SelectBlock.SetItemNum (ItemNum);

  for (UINT16 i = 0, j = 0; i < ItemNum; i ++) {
    UINT16 k = 0;
    while ((TmpStr[k ++] = *(ItemContent + j ++)) != '\\');
    TmpStr[k - 1] = (CHAR16) NULL;
    SelectBlock.SetItemContent (i, (CHAR16 *) &TmpStr);
  }
  SelectBlock.FocuseItem (ItemPtr, FocuseItemTextColor, FocuseItemBlockColor);
  SelectBlock.Display (BackgroundSourceImage, BlockTLX, BlockTLY, true);
  while (!isSelect) {
    getch (&KeyCode);
    switch (KeyCode.ScanCode) {
      case SCAN_UP:
        ItemPtr ? ItemPtr -- : ItemPtr = ItemNum - 1;
        break;
      case SCAN_DOWN :
        (ItemPtr < ItemNum - 1) ? ItemPtr ++ : ItemPtr = 0;
        break;
      case SCAN_NULL :
        if (KeyCode.UnicodeChar == CHAR_CARRIAGE_RETURN) {
          isSelect = true;
          break;
        }
      case SCAN_ESC:
        SelectBlock.Hidden ();
        return 0xFF;
    }
    SelectBlock.FocuseItem (ItemPtr, FocuseItemTextColor, FocuseItemBlockColor);
    SelectBlock.Display (BackgroundSourceImage, BlockTLX, BlockTLY, false);
  }
  SelectBlock.Hidden ();

  return ItemPtr;
}