/*++ 

Copyright (c) 2008 - 2010, Solar Studio                                                         
         
Module Name:

  ObstructChessTestFunction.h

Abstract:

  This is an EFI-shell game program that calls ObstructChess.

--*/

SOLAR_STATUS
SOLAR_API
TestChessFullOnBoard (
  EFI_GRAPHICS_OUTPUT_PROTOCOL *pGraph,
  SOLAR_IMAGE_INPUT             DesktopImage,
  SOLAR_IMAGE_INPUT             WhiteChessImage,
  SOLAR_IMAGE_INPUT             BlackChessImage,
  CHESS_BLOCK_INFO              *ChessBlock
  );

SOLAR_STATUS
SOLAR_API
TestNeighbor (
  EFI_GRAPHICS_OUTPUT_PROTOCOL *pGraph,
  SOLAR_IMAGE_INPUT             DesktopImage,
  SOLAR_IMAGE_INPUT             WhiteChessImage,
  SOLAR_IMAGE_INPUT             BlackChessImage,
  CHESS_BLOCK_INFO              *ChessBlock,
  UINT16                        Sn
  );