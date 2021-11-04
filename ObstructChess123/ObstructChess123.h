/*++ 

Copyright (c) 2008 - 2010, Solar Studio                                                         
         
Module Name:

  ObstructChess.h

Abstract:

  This is an EFI-shell game program that calls ObstructChess.

--*/

#ifndef _OBSTRUCT_CHESS_H_
#define _OBSTRUCT_CHESS_H_

#include "_DevelopAdditionSetup.h"

#define SPACE_LINE                           L"                                                                                "

#define HORIZONTAL_RESOLUTION                800
#define VERTICAL_RESOLUTION                  600

// Chess board related definition.
#define CHESS_BOARD_WIDTH                    481
#define CHESS_BOARD_HEIGHT                   CHESS_BOARD_WIDTH

#define CHESS_BOARD_CHECK_BLOCK_ONE_SIDE_MAX 11
#define CHESS_BOARD_CHECK_BLOCK_ROW_MAX_NUM  (CHESS_BOARD_CHECK_BLOCK_ONE_SIDE_MAX - 1) * 2 + 1

#define CHESS_BOARD_X1                       160
#define CHESS_BOARD_Y1                       60

#define INIT_BACKGROUND_FILE                 L"InitBackground.bmp"
#define CHESS_BOARD_BACKGROUND_FILE1         L"ObstructChessBackground1.bmp"
#define CHESS_BOARD_BACKGROUND_FILE2         L"ObstructChessBackground2.bmp"

// Key related definition.
#define CHAR_SPACE                           0x0020

typedef struct _CHESS_BLOCK_POSITION {
  UINT16 x;
  UINT16 y;
} CHESS_BLOCK_POSITION;

typedef struct _CHESS_BLOCK_INFO {
  CHESS_BLOCK_POSITION Position;
  UINT8                Status;
  UINT16               Row;
  UINT16               Col;
  UINT16               TopLeft;
  UINT16               TopRight;
  UINT16               Right;
  UINT16               LowerRight;
  UINT16               LowerLeft;
  UINT16               Left;
  SOLAR_IMAGE_INPUT    BackgroundImage;
} CHESS_BLOCK_INFO;

enum ChessBlockStatus {
  BlackCheck,                           // 0
  WhiteCheck,                           // 1
  Empty,                                // 2
  Invalid                               // 3
};

#endif //_OBSTRUCT_CHESS_H_

