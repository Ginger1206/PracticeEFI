/*++ 

Copyright (c) 2008 - 2010, Solar Studio                                                         
         
Module Name:

  DrawGame.h

Abstract:

  This is an EFI-shell game program that calls DrawGame.

--*/

#ifndef _DRAW_FUNC_H_
#define _DRAW_FUNC_H_

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
#define SELECT_X_COORDINATE                  30

// Color Block related definition
#define COLOR_BLOCK_EDGE                     20 // = COLOR_BLOCK_WIDTH = COLOR_BLOCK_HEIGHT 此非固定的應該要COLOR_BOARD_WIDTH除畫的行數
#define SELECT_STARTX_WIDTH                  30

//Macro definition
#define PUT_COLOR_CHECK(Sn)                  BlockActive(pGraph, DesktopImage, ColorBlockImage, ColorBlock, Sn, ColorCheck);    

// Key related definition.
#define CHAR_BackSpace                       0x0008
#define CHAR_HTab                            0x0009
//#define CHAR_ENTER                           0x0013
#define CHAR_SPACE                           0x0020
#define CHAR_A                               0x0041
#define CHAR_a                               0x0061
#define CHAR_B                               0x0042
#define CHAR_b                               0x0062
#define CHAR_C                               0x0043
#define CHAR_c                               0x0063
#define CHAR_D                               0x0044
#define CHAR_d                               0x0064
#define CHAR_E                               0x0045
#define CHAR_e                               0x0065
#define CHAR_F                               0x0046
#define CHAR_f                               0x0066
#define CHAR_Q                               0x0051
#define CHAR_q                               0x0071
#define CHAR_R                               0x0052
#define CHAR_r                               0x0072
#define CHAR_T                               0x0054
#define CHAR_t                               0x0074

#define CHAR_0                               0x0030
#define CHAR_1                               0x0031
#define CHAR_2                               0x0032
#define CHAR_3                               0x0033
#define CHAR_4                               0x0034
#define CHAR_5                               0X0035
#define CHAR_6                               0x0036
#define CHAR_7                               0x0037
#define CHAR_8                               0x0038
#define CHAR_9                               0x0039

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
  //UINT32 color;
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

#endif //_DRAW_FUNC_H_

