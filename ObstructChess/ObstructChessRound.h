/*++ 

Copyright (c) 2008 - 2010, Solar Studio                                                         
         
Module Name:

  ObstructChessRound.h

Abstract:

  This is an EFI-shell game program that calls ObstructChess.

--*/

#ifndef _OBSTRUCT_CHESS_ROUND_H_
#define _OBSTRUCT_CHESS_ROUND_H_

// Chess related definition.
#define CHESS_IMAGE_WIDTH               15
#define CHESS_IMAGE_HEIGHT              15
#define CHESS_IMAGE_TRANSPARENT         0x80
#define BLACK_CHESS_IMAGE_FILENAME      L"BlackChess.bmp"
#define WHITE_CHESS_IMAGE_FILENAME      L"WhiteChess.bmp"
#define BLACK_LOCATION_IMAGE_FILENAME   L"BlackLocation.bmp"
#define WHITE_LOCATION_IMAGE_FILENAME   L"WhiteLocation.bmp"

#define CHESS_INVALID_NEIGHBOR          0xFFFF

// Player related definition.
#define ENEMY(self)                     1 - self
#define SELF(self)                      self

#define DEFAULT_INFO_BLOCK_WIDTH        200
#define DEFAULT_INFO_BLOCK_HEIGHT       150
#define INFO_BLOCK_01_IMAGE_FILENAME    L"InfoBlock1.bmp"
#define WHITE_INFO_BLOCK_IMAGE_FILENAME L"WhiteInfoBlock.bmp"
#define BLACK_INFO_BLOCK_IMAGE_FILENAME L"BlackInfoBlock.bmp"
#define THINK_INFO_BLOCK_IMAGE_FILENAME L"Think.bmp"
#define RED_INFO_BLOCK_IMAGE_FILENAME   L"RedInfoBlock.bmp"
#define ABANDON_ROUND_MSG_WIN_FILENAME  L"AbandonRoundMsgWin.bmp"
#define EXIT_GAME_MSG_WIN_FILENAME      L"ExitGameMsgWin.bmp"
#define WIN_MSG_WIN_FILENAME            L"WinMsgWin.bmp"
#define DRAW_MSG_WIN_FILENAME           L"DrawMsgWin.bmp"
#define LOSE_MSG_WIN_FILENAME           L"LoseMsgWin.bmp"
#define PLAYER_NAME_STRING_LENGTH       0x100

// Macro definition.
#define PUT_BLACK_CHECK(Sn)             BlockActive (pGraph, DesktopImage, WhiteChessImage, BlackChessImage, ChessBlock, Sn, BlackCheck)
#define PUT_WHITE_CHECK(Sn)             BlockActive (pGraph, DesktopImage, WhiteChessImage, BlackChessImage, ChessBlock, Sn, WhiteCheck)
#define PUT_EMPTY_CHECK(Sn)             BlockActive (pGraph, DesktopImage, WhiteChessImage, BlackChessImage, ChessBlock, Sn, Empty)

// Others
#define CHECK_DANGER                    true
#define NO_CHECK_DANGER                 false
#define BLACK_SITE                      0
#define WHITE_SITE                      1

SOLAR_STATUS
SOLAR_API
Round (
  EFI_GRAPHICS_OUTPUT_PROTOCOL *pGraph,
  SOLAR_IMAGE_INPUT            DesktopImage,
  CHESS_BLOCK_INFO             *ChessBlock,
  BOOLEAN                      isSinglePlayer
  );

#endif //_OBSTRUCT_CHESS_ROUND_H_