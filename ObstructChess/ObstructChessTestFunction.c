/*++ 

Copyright (c) 2008 - 2010, Solar Studio                                                         
         
Module Name:

  ObstructChessTestFunction.c

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
#include "ObstructChess.h"
#include "ObstructChessRound.h"
#include "GraphicsPointer.h"
#include "ImageBlock.h"
#ifdef __DEBUG__
  #include "../_DevelopAddition/_Debug.h"
#endif

extern UINT16                        ChessBlockEndColNum[CHESS_BOARD_CHECK_BLOCK_ROW_MAX_NUM];
extern UINT16                        ChessBlockAmount;
extern EFI_GRAPHICS_OUTPUT_BLT_PIXEL TransparentColor;

SOLAR_STATUS
SOLAR_API
TestChessFullOnBoard (
  EFI_GRAPHICS_OUTPUT_PROTOCOL *pGraph,
  SOLAR_IMAGE_INPUT             DesktopImage,
  SOLAR_IMAGE_INPUT             WhiteChessImage,
  SOLAR_IMAGE_INPUT             BlackChessImage,
  CHESS_BLOCK_INFO              *ChessBlock
  )
{
  for (UINT16 i = 0; i < ChessBlockAmount; i ++) {
    TransparentBltBufferToVideo (
      pGraph,
      ((i % 2) ? &BlackChessImage : &WhiteChessImage),
      &DesktopImage,
      ChessBlock[i].Position.x - (CHESS_IMAGE_WIDTH / 2),
      ChessBlock[i].Position.y - (CHESS_IMAGE_HEIGHT / 2),
      TransparentColor
      );
  }

  return SOLAR_SUCCESS;
}

SOLAR_STATUS
SOLAR_API
TestNeighbor (
  EFI_GRAPHICS_OUTPUT_PROTOCOL *pGraph,
  SOLAR_IMAGE_INPUT             DesktopImage,
  SOLAR_IMAGE_INPUT             WhiteChessImage,
  SOLAR_IMAGE_INPUT             BlackChessImage,
  CHESS_BLOCK_INFO              *ChessBlock,
  UINT16                        Sn
  )
{
  UINT16 TmpSn;

  TransparentBltBufferToVideo (
    pGraph,
    &BlackChessImage,
    &DesktopImage,
    ChessBlock[Sn].Position.x - (CHESS_IMAGE_WIDTH / 2),
    ChessBlock[Sn].Position.y - (CHESS_IMAGE_HEIGHT / 2),
    TransparentColor
    );

  if ((TmpSn = ChessBlock[Sn].TopLeft) != CHESS_INVALID_NEIGHBOR) {
    TransparentBltBufferToVideo (
      pGraph,
      &BlackChessImage,
      &DesktopImage,
      ChessBlock[TmpSn].Position.x - (CHESS_IMAGE_WIDTH / 2),
      ChessBlock[TmpSn].Position.y - (CHESS_IMAGE_HEIGHT / 2),
      TransparentColor
      );
  }

  if ((TmpSn = ChessBlock[Sn].TopRight) != CHESS_INVALID_NEIGHBOR) {
    TransparentBltBufferToVideo (
      pGraph,
      &WhiteChessImage,
      &DesktopImage,
      ChessBlock[TmpSn].Position.x - (CHESS_IMAGE_WIDTH / 2),
      ChessBlock[TmpSn].Position.y - (CHESS_IMAGE_HEIGHT / 2),
      TransparentColor
      );
  }

  if ((TmpSn = ChessBlock[Sn].Right) != CHESS_INVALID_NEIGHBOR) {
    TransparentBltBufferToVideo (
      pGraph,
      &BlackChessImage,
      &DesktopImage,
      ChessBlock[TmpSn].Position.x - (CHESS_IMAGE_WIDTH / 2),
      ChessBlock[TmpSn].Position.y - (CHESS_IMAGE_HEIGHT / 2),
      TransparentColor
      );
  }

  if ((TmpSn = ChessBlock[Sn].LowerRight) != CHESS_INVALID_NEIGHBOR) {
    TransparentBltBufferToVideo (
      pGraph,
      &WhiteChessImage,
      &DesktopImage,
      ChessBlock[TmpSn].Position.x - (CHESS_IMAGE_WIDTH / 2),
      ChessBlock[TmpSn].Position.y - (CHESS_IMAGE_HEIGHT / 2),
      TransparentColor
      );
  }

  if ((TmpSn = ChessBlock[Sn].LowerLeft) != CHESS_INVALID_NEIGHBOR) {
    TransparentBltBufferToVideo (
      pGraph,
      &BlackChessImage,
      &DesktopImage,
      ChessBlock[TmpSn].Position.x - (CHESS_IMAGE_WIDTH / 2),
      ChessBlock[TmpSn].Position.y - (CHESS_IMAGE_HEIGHT / 2),
      TransparentColor
      );
  }

  if ((TmpSn = ChessBlock[Sn].Left) != CHESS_INVALID_NEIGHBOR) {
    TransparentBltBufferToVideo (
      pGraph,
      &WhiteChessImage,
      &DesktopImage,
      ChessBlock[TmpSn].Position.x - (CHESS_IMAGE_WIDTH / 2),
      ChessBlock[TmpSn].Position.y - (CHESS_IMAGE_HEIGHT / 2),
      TransparentColor
      );
  }

  return SOLAR_SUCCESS;
}