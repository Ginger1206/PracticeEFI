/*++ 

Copyright (c) 2008 - 2010, Solar Studio                                                         
         
Module Name:

  DrawGame.h

Abstract:

  This is an EFI-shell game program that calls DrawGame.

--*/

#ifndef _OBSTRUCT_CHESS_H_
#define _OBSTRUCT_CHESS_H_

#include "_DevelopAdditionSetup.h"

#define SPACE_LINE                           L"                                                                                "

#define HORIZONTAL_RESOLUTION                800
#define VERTICAL_RESOLUTION                  600

// Chess board related definition.
#define CHESS_BOARD_CHECK_BLOCK_ONE_SIDE_MAX 11
#define CHESS_BOARD_CHECK_BLOCK_ROW_MAX_NUM  (CHESS_BOARD_CHECK_BLOCK_ONE_SIDE_MAX - 1) * 2 + 1

#define CHESS_INVALID_NEIGHBOR               0xFFFF

#define INIT_BACKGROUND_FILE                 L"InitBackground.bmp"
#define CHESS_BOARD_BACKGROUND_FILE1         L"ObstructChessBackground1.bmp"
#define CHESS_BOARD_BACKGROUND_FILE2         L"ObstructChessBackground2.bmp"

// Chess related definition. 210326G
#define CHESS_IMAGE_WIDTH                    15
#define CHESS_IMAGE_HEIGHT                   15
#define CHESS_IMAGE_TRANSPARENT              0x80
#define WHITE_CHESS_IMAGE_FILENAME           L"WhiteChess.bmp"
#define WHITE_LOCATION_IMAGE_FILENAME        L"WhiteLocation.bmp"
#define BLACK_CHESS_IMAGE_FILENAME           L"BlackChess.bmp"

//Color Board related definition
#define COLOR_BOARD_WIDTH                    500
#define COLOR_BOARD_HEIGH                    COLOR_BOARD_WIDTH
#define COLOR_BOARD_X1                       50
#define COLOR_BOARD_Y1                       25

// Color Block related definition
#define COLOR_BLOCK_EDGE                     20 // = COLOR_BLOCK_WIDTH = COLOR_BLOCK_HEIGHT 此非固定的應該要COLOR_BOARD_WIDTH除畫的行數

//Macro definition
#define PUT_COLOR_CHECK(Sn)                  BlockActive(pGraph, DesktopImage, ColorBlockImage, ColorBlock, Sn, ColorCheck);    

// Key related definition.
#define CHAR_BackSpace                       0x0008
#define CHAR_SPACE                           0x0020
#define CHAR_D                               0x0064
#define CHAR_F                               0x0066
#define CHAR_R                               0x0072

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
/*
enum ChessBlockStatus {
  BlackCheck,                           // 0
  WhiteCheck,                           // 1
  Empty,                                // 2
  Invalid                               // 3
};
*/
//先寫有誤就換CHESS_BLOCK_INFO
typedef struct _COLOR_BLOCK_POSITION{
  UINT16 x;
  UINT16 y;
}COLOR_BLOCK_POSITION;

typedef struct _COLOR_BLOCK_INFO{
  COLOR_BLOCK_POSITION  Position;
  UINT8                 Status;
  UINT16                Row;
  UINT16                Col;
  UINT16                TopRight;
  UINT16                TopLeft;
  UINT16                LowerRight;
  UINT16                LowerLeft;
  SOLAR_IMAGE_INPUT     BackgroundImage;
}COLOR_BLOCK_INFO;

typedef struct _BLOCK_COORDINATE{
  UINT16 x;
  UINT16 y;
}BLOCK_COORDINATE;

enum ColorBlockStatus{ //未寫完
  ColorCheck, //要改成color
  Empty,
  Invalid
};

#endif //_OBSTRUCT_CHESS_H_

