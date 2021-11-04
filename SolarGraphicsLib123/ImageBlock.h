/*++ 

Copyright (c) 2008 - 2010, Solar Studio                                                         
         
Module Name:

  ImageBlock.h

Abstract:

  This is an EFI-shell game program that calls ObstructChess.

--*/

#ifndef _IMAGE_BLOCK_H_
#define _IMAGE_BLOCK_H_

#define BLOCK_SET_TO_CENTER_X(width)  (HORIZONTAL_RESOLUTION / 2) - (width / 2)
#define BLOCK_SET_TO_CENTER_Y(height) (VERTICAL_RESOLUTION / 2) - (height / 2)

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Clasee Definition - STARTS ////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/////////////////////////
// class : IMAGE_BLOCK //
/////////////////////////

class IMAGE_BLOCK {
  // Static Property
public:
  SOLAR_IMAGE_INPUT             Foreground;
  SOLAR_IMAGE_INPUT             Background;
  UINT32                        BlockTextColor;
  UINT32                        BlockColor;
  EFI_GRAPHICS_OUTPUT_PROTOCOL  *pGraph;

protected:
  UINT16                        PresentlyX;
  UINT16                        PresentlyY;

  // Dynamic Property
public:
  IMAGE_BLOCK ();
  ~IMAGE_BLOCK ();
  SOLAR_STATUS SOLAR_API BlockInit (EFI_GRAPHICS_OUTPUT_PROTOCOL *pG, UINT16 Width, UINT16 Height, UINT32 DefaultBlockTextColor, UINT32 DefaultBlockColor);
  SOLAR_STATUS SOLAR_API Display (SOLAR_IMAGE_INPUT BackgroundSourceImage, UINT16 X, UINT16 Y, BOOLEAN SaveBackground);
  SOLAR_STATUS SOLAR_API Hidden (VOID);
};

//////////////////////////
// class : SELECT_BLOCK //
//////////////////////////
#define ITEM_HEIGHT 15                  // For select item partitions.
#define ITEM_MARGIN 2

class SELECT_BLOCK: public IMAGE_BLOCK {
  // Static Property
public:

protected:
  CHAR16 **ItemContent;
  UINT16 ItemNum;
  UINT16 ItemPtr;

  // Dynamic Property
public:
  SELECT_BLOCK ();
  ~SELECT_BLOCK ();
  SOLAR_STATUS SOLAR_API SetItemNum (UINT16 Num);
  SOLAR_STATUS SOLAR_API SetItemContent (UINT16 ItemNo, CHAR16 *Content);
  SOLAR_STATUS SOLAR_API FocuseItem (UINT16 ItemNo, UINT32 FocuseItemTextColor, UINT32 FocuseItemBlockColor);
};

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
  );

#endif //_IMAGE_BLOCK_H_