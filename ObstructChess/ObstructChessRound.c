/*++ 

Copyright (c) 2008 - 2010, Solar Studio                                                         
         
Module Name:

  ObstructChessRound.c

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
  #include "ObstructChessTestFunction.h"
#endif

extern UINT16                        ChessBlockEndColNum[CHESS_BOARD_CHECK_BLOCK_ROW_MAX_NUM];
extern UINT16                        ChessBlockAmount;
extern UINT16                        CheckBlockRowNum;
extern EFI_GRAPHICS_OUTPUT_BLT_PIXEL TransparentColor;

SOLAR_STATUS
SOLAR_API
BlockActive (
  EFI_GRAPHICS_OUTPUT_PROTOCOL *pGraph,
  SOLAR_IMAGE_INPUT            DesktopImage,
  SOLAR_IMAGE_INPUT            WhiteChessImage,
  SOLAR_IMAGE_INPUT            BlackChessImage,
  CHESS_BLOCK_INFO             *ChessBlock,
  UINT16                       Sn,
  UINT8                        ChessBlockStatus
  )
{
  (ChessBlock + Sn)->Status = ChessBlockStatus;

  switch (ChessBlockStatus) {
    case Empty:
      TransparentBltBufferToVideo (
        pGraph,
        &(ChessBlock + Sn)->BackgroundImage,
        &DesktopImage,
        (ChessBlock + Sn)->Position.x - (CHESS_IMAGE_WIDTH / 2),
        (ChessBlock + Sn)->Position.y - (CHESS_IMAGE_HEIGHT / 2),
        TransparentColor
        );
      break;
    case BlackCheck:
      TransparentBltBufferToVideo (
        pGraph,
        &BlackChessImage,
        &DesktopImage,
        (ChessBlock + Sn)->Position.x - (CHESS_IMAGE_WIDTH / 2),
        (ChessBlock + Sn)->Position.y - (CHESS_IMAGE_HEIGHT / 2),
        TransparentColor
        );
      break;
    case WhiteCheck:
      TransparentBltBufferToVideo (
        pGraph,
        &WhiteChessImage,
        &DesktopImage,
        (ChessBlock + Sn)->Position.x - (CHESS_IMAGE_WIDTH / 2),
        (ChessBlock + Sn)->Position.y - (CHESS_IMAGE_HEIGHT / 2),
        TransparentColor
        );
      break;
    case Invalid:
      break;
  }
  
  return SOLAR_SUCCESS;
}

BOOLEAN
CheckExpelEnemyCheck (
  CHESS_BLOCK_INFO             *ChessBlock,
  UINT16                       Sn,
  BOOLEAN                      Who
  )
{
  UINT8 FriendlyNeighborCount = 0;

  if (ChessBlock[Sn].Status == ENEMY(Who)) {
    if (ChessBlock[Sn].TopLeft != CHESS_INVALID_NEIGHBOR) {
      (ChessBlock[ChessBlock[Sn].TopLeft].Status    == SELF(Who)) ? FriendlyNeighborCount ++ : FriendlyNeighborCount;
    }
    if (ChessBlock[Sn].TopRight != CHESS_INVALID_NEIGHBOR) {
      (ChessBlock[ChessBlock[Sn].TopRight].Status   == SELF(Who)) ? FriendlyNeighborCount ++ : FriendlyNeighborCount;
    }
    if (ChessBlock[Sn].Right != CHESS_INVALID_NEIGHBOR) {
      (ChessBlock[ChessBlock[Sn].Right].Status      == SELF(Who)) ? FriendlyNeighborCount ++ : FriendlyNeighborCount;
    }
    if (ChessBlock[Sn].LowerRight != CHESS_INVALID_NEIGHBOR) {
      (ChessBlock[ChessBlock[Sn].LowerRight].Status == SELF(Who)) ? FriendlyNeighborCount ++ : FriendlyNeighborCount;
    }
    if (ChessBlock[Sn].LowerLeft != CHESS_INVALID_NEIGHBOR) {
      (ChessBlock[ChessBlock[Sn].LowerLeft].Status  == SELF(Who)) ? FriendlyNeighborCount ++ : FriendlyNeighborCount;
    }
    if (ChessBlock[Sn].Left != CHESS_INVALID_NEIGHBOR) {
      (ChessBlock[ChessBlock[Sn].Left].Status       == SELF(Who)) ? FriendlyNeighborCount ++ : FriendlyNeighborCount;
    }
    if (FriendlyNeighborCount >= 3) {
      return true;
    } else {
      return false;
    }
  }

  return false; 
}

VOID
ExpelEnemy (
  EFI_GRAPHICS_OUTPUT_PROTOCOL *pGraph,
  SOLAR_IMAGE_INPUT            DesktopImage,
  SOLAR_IMAGE_INPUT            WhiteChessImage,
  SOLAR_IMAGE_INPUT            BlackChessImage,
  CHESS_BLOCK_INFO             *ChessBlock,
  UINT16                       Sn,
  BOOLEAN                      Who,
  UINT16                       *BlackScore,
  UINT16                       *WhiteScore
  )
{
  // Check expel enemy when player put their check. 
  if (ChessBlock[Sn].TopLeft != CHESS_INVALID_NEIGHBOR) {
    if (CheckExpelEnemyCheck (ChessBlock, ChessBlock[Sn].TopLeft, Who)) {
      PUT_EMPTY_CHECK (ChessBlock[Sn].TopLeft);
      Who ? (*BlackScore) -- : (*WhiteScore) --;
    }
  }
  if (ChessBlock[Sn].TopRight != CHESS_INVALID_NEIGHBOR) {
    if (CheckExpelEnemyCheck (ChessBlock, ChessBlock[Sn].TopRight, Who)) {
      PUT_EMPTY_CHECK (ChessBlock[Sn].TopRight);
      Who ? (*BlackScore) -- : (*WhiteScore) --;
    }
  }
  if (ChessBlock[Sn].Right != CHESS_INVALID_NEIGHBOR) {
    if (CheckExpelEnemyCheck (ChessBlock, ChessBlock[Sn].Right, Who)) {
      PUT_EMPTY_CHECK (ChessBlock[Sn].Right);
      Who ? (*BlackScore) -- : (*WhiteScore) --;
    }
  }
  if (ChessBlock[Sn].LowerRight != CHESS_INVALID_NEIGHBOR) {
    if (CheckExpelEnemyCheck (ChessBlock, ChessBlock[Sn].LowerRight, Who)) {
      PUT_EMPTY_CHECK (ChessBlock[Sn].LowerRight);
      Who ? (*BlackScore) -- : (*WhiteScore) --;
    }
  }
  if (ChessBlock[Sn].LowerLeft != CHESS_INVALID_NEIGHBOR) {
    if (CheckExpelEnemyCheck (ChessBlock, ChessBlock[Sn].LowerLeft, Who)) {
      PUT_EMPTY_CHECK (ChessBlock[Sn].LowerLeft);
      Who ? (*BlackScore) -- : (*WhiteScore) --;
    }
  }
  if (ChessBlock[Sn].Left != CHESS_INVALID_NEIGHBOR) {
    if (CheckExpelEnemyCheck (ChessBlock, ChessBlock[Sn].Left, Who)) {
      PUT_EMPTY_CHECK (ChessBlock[Sn].Left);
      Who ? (*BlackScore) -- : (*WhiteScore) --;
    }
  }
}

SOLAR_STATUS
SOLAR_API
LocationActive (
  EFI_GRAPHICS_OUTPUT_PROTOCOL *pGraph,
  SOLAR_IMAGE_INPUT            DesktopImage,
  SOLAR_IMAGE_INPUT            LocationImage,
  SOLAR_IMAGE_INPUT            LocationBackgroundImage,
  CHESS_BLOCK_INFO             *ChessBlock,
  UINT16                       Sn
  )
{
  EFI_STATUS                   Status;

  // Save the background for put the location image.
  Status = pGraph->Blt (
             pGraph,
             LocationBackgroundImage.BltBuffer,
             EfiBltVideoToBltBuffer,
             ChessBlock[Sn].Position.x - (CHESS_IMAGE_WIDTH / 2),
             ChessBlock[Sn].Position.y - (CHESS_IMAGE_HEIGHT / 2),
             0,
             0,
             LocationBackgroundImage.Width,
             LocationBackgroundImage.Height,
             0
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  // Put the location image.
  TransparentBltBufferToVideo (
    pGraph,
    &LocationImage,
    &DesktopImage,
    ChessBlock[Sn].Position.x - (CHESS_IMAGE_WIDTH / 2),
    ChessBlock[Sn].Position.y - (CHESS_IMAGE_HEIGHT / 2),
    TransparentColor
    );

  return SOLAR_SUCCESS;
}

UINT16 CalculateLocationWhenDirectionKeyEvent (
  CHESS_BLOCK_INFO *ChessBlock,
  UINT16           Sn,
  UINT16           ScanCode
  )
{
  switch (ScanCode) {
    case SCAN_UP:
      if ((ChessBlock + Sn)->Row > 0) {
        if ((ChessBlock + Sn)->Row % 2) {
          if ((ChessBlock + Sn)->TopLeft == CHESS_INVALID_NEIGHBOR) {
            return (ChessBlock + Sn)->TopRight;
          } else {
            return (ChessBlock + Sn)->TopLeft;
          }
        } else {
          if ((ChessBlock + Sn)->TopRight == CHESS_INVALID_NEIGHBOR) {
            return (ChessBlock + Sn)->TopLeft;
          } else {
            return (ChessBlock + Sn)->TopRight;
          }
        }
      } else {
        return ChessBlockAmount - ChessBlockEndColNum[ CheckBlockRowNum - 1] + Sn;
      }
      break;
    case SCAN_DOWN:
      if ((ChessBlock + Sn)->Row <  CheckBlockRowNum - 1) {
        if ((ChessBlock + Sn)->Row % 2) {
          if ((ChessBlock + Sn)->LowerRight == CHESS_INVALID_NEIGHBOR) {
            return (ChessBlock + Sn)->LowerLeft;
          } else {
            return (ChessBlock + Sn)->LowerRight;
          }
        } else {
          if ((ChessBlock + Sn)->LowerLeft == CHESS_INVALID_NEIGHBOR) {
            return (ChessBlock + Sn)->LowerRight;
          } else {
            return (ChessBlock + Sn)->LowerLeft;
          }
        }
      } else {
        return (ChessBlock + Sn)->Col;
      }
      break;
    case SCAN_LEFT:
      if ((ChessBlock + Sn)->Left == CHESS_INVALID_NEIGHBOR) {
        return Sn + ChessBlockEndColNum[(ChessBlock + Sn)->Row] - 1;
      } else {
        return (ChessBlock + Sn)->Left;
      }
      break;
    case SCAN_RIGHT:
      if ((ChessBlock + Sn)->Right == CHESS_INVALID_NEIGHBOR) {
        return Sn - ChessBlockEndColNum[(ChessBlock + Sn)->Row] + 1;
      } else {
        return (ChessBlock + Sn)->Right;
      }
      break;
  }

  return 0;
}

BOOLEAN
CheckPutCheck (
  CHESS_BLOCK_INFO *ChessBlock,
  UINT16           Sn,
  BOOLEAN          Who
  )
{
  if (ChessBlock[Sn].Status == Empty) {
    UINT8 EnemyNeighborCount = 0;
    if (ChessBlock[Sn].TopLeft != CHESS_INVALID_NEIGHBOR) {
      (ChessBlock[ChessBlock[Sn].TopLeft].Status    == ENEMY(Who)) ? EnemyNeighborCount ++ : EnemyNeighborCount;
    }
    if (ChessBlock[Sn].TopRight != CHESS_INVALID_NEIGHBOR) {
      (ChessBlock[ChessBlock[Sn].TopRight].Status   == ENEMY(Who)) ? EnemyNeighborCount ++ : EnemyNeighborCount;
    }
    if (ChessBlock[Sn].Right != CHESS_INVALID_NEIGHBOR) {
      (ChessBlock[ChessBlock[Sn].Right].Status      == ENEMY(Who)) ? EnemyNeighborCount ++ : EnemyNeighborCount;
    }
    if (ChessBlock[Sn].LowerRight != CHESS_INVALID_NEIGHBOR) {
      (ChessBlock[ChessBlock[Sn].LowerRight].Status == ENEMY(Who)) ? EnemyNeighborCount ++ : EnemyNeighborCount;
    }
    if (ChessBlock[Sn].LowerLeft != CHESS_INVALID_NEIGHBOR) {
      (ChessBlock[ChessBlock[Sn].LowerLeft].Status  == ENEMY(Who)) ? EnemyNeighborCount ++ : EnemyNeighborCount;
    }
    if (ChessBlock[Sn].Left != CHESS_INVALID_NEIGHBOR) {
      (ChessBlock[ChessBlock[Sn].Left].Status       == ENEMY(Who)) ? EnemyNeighborCount ++ : EnemyNeighborCount;
    }
    if (EnemyNeighborCount < 3) {
      return true;
    } else {
      return false;
    }
  }

  return false;
}

UINT32
RandomInitSeed (
  VOID
  )
{
  EFI_TIME                  Time;
  UINT32                    Seed;

  gRT->GetTime (&Time, (EFI_TIME_CAPABILITIES *) NULL);
  Seed = (~Time.Hour << 24 | Time.Day << 16 | Time.Minute << 8 | Time.Second);
  Seed ^= Time.Nanosecond;
  Seed ^= Time.Year << 7;

  return Seed;
}

UINT8
GetEnemyNeighborNum (
  CHESS_BLOCK_INFO *ChessBlock,
  UINT16           Sn,
  BOOLEAN          Who
  )
{
  UINT8            EnemyNeighborCount;

  EnemyNeighborCount = 0;

  if (ChessBlock[Sn].TopLeft != CHESS_INVALID_NEIGHBOR) {
    if (ChessBlock[ChessBlock[Sn].TopLeft].Status == ENEMY(Who)) {
      EnemyNeighborCount ++;
    }
  }
  if (ChessBlock[Sn].TopRight != CHESS_INVALID_NEIGHBOR) {
    if (ChessBlock[ChessBlock[Sn].TopRight].Status == ENEMY(Who)) {
      EnemyNeighborCount ++;
    }
  }
  if (ChessBlock[Sn].Right != CHESS_INVALID_NEIGHBOR) {
    if (ChessBlock[ChessBlock[Sn].Right].Status == ENEMY(Who)) {
      EnemyNeighborCount ++;
    }
  }
  if (ChessBlock[Sn].LowerRight != CHESS_INVALID_NEIGHBOR) {
    if (ChessBlock[ChessBlock[Sn].LowerRight].Status == ENEMY(Who)) {
      EnemyNeighborCount ++;
    }
  }
  if (ChessBlock[Sn].LowerLeft != CHESS_INVALID_NEIGHBOR) {
    if (ChessBlock[ChessBlock[Sn].LowerLeft].Status == ENEMY(Who)) {
      EnemyNeighborCount ++;
    }
  }
  if (ChessBlock[Sn].Left != CHESS_INVALID_NEIGHBOR) {
    if (ChessBlock[ChessBlock[Sn].Left].Status == ENEMY(Who)) {
      EnemyNeighborCount ++;
    }
  }

  return EnemyNeighborCount;
}

INT16
GetBlockSnForPutAppropriateSelfNeighborAi (
  CHESS_BLOCK_INFO *ChessBlock,
  UINT16           Sn,
  BOOLEAN          Who,
  BOOLEAN          CheckDanger
  )
{
  UINT32 RanVal;
  UINT16 NeighborSn[6] = {0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000};
  UINT8  Count = 0;

  do {
    RanVal = RandomInitSeed () % 6;
    if (NeighborSn[RanVal] == 0x8000) {
      switch (Count) {
        case 0:
          NeighborSn[RanVal] = ChessBlock[Sn].TopLeft;
          break;
        case 1:
          NeighborSn[RanVal] = ChessBlock[Sn].TopRight;
          break;
        case 2:
          NeighborSn[RanVal] = ChessBlock[Sn].Right;
          break;
        case 3:
          NeighborSn[RanVal] = ChessBlock[Sn].LowerRight;
          break;
        case 4:
          NeighborSn[RanVal] = ChessBlock[Sn].LowerLeft;
          break;
        case 5:
          NeighborSn[RanVal] = ChessBlock[Sn].Left;
          break;
      }
      Count ++;
    }
  } while (NeighborSn[0] == 0x8000 || NeighborSn[1] == 0x8000 || NeighborSn[2] == 0x8000 || NeighborSn[3] == 0x8000 || NeighborSn[4] == 0x8000 || NeighborSn[5] == 0x8000);

  if (NeighborSn[0] != CHESS_INVALID_NEIGHBOR) {
    if (ChessBlock[NeighborSn[0]].Status == Empty && (!CheckDanger || GetEnemyNeighborNum (ChessBlock, NeighborSn[0], Who) < 2)) {
      if (CheckPutCheck (ChessBlock, NeighborSn[0], Who)) {
        return NeighborSn[0];
      }
    }
  }
  if (NeighborSn[1] != CHESS_INVALID_NEIGHBOR) {
    if (ChessBlock[NeighborSn[1]].Status == Empty && (!CheckDanger || GetEnemyNeighborNum (ChessBlock, NeighborSn[1], Who) < 2)) {
      if (CheckPutCheck (ChessBlock, NeighborSn[1], Who)) {
        return NeighborSn[1];
      }
    }
  }
  if (NeighborSn[2] != CHESS_INVALID_NEIGHBOR) {
    if (ChessBlock[NeighborSn[2]].Status == Empty && (!CheckDanger || GetEnemyNeighborNum (ChessBlock, NeighborSn[2], Who) < 2)) {
      if (CheckPutCheck (ChessBlock, NeighborSn[2], Who)) {
        return NeighborSn[2];
      }
    }
  }
  if (NeighborSn[3] != CHESS_INVALID_NEIGHBOR) {
    if (ChessBlock[NeighborSn[3]].Status == Empty && (!CheckDanger || GetEnemyNeighborNum (ChessBlock, NeighborSn[3], Who) < 2)) {
      if (CheckPutCheck (ChessBlock, NeighborSn[3], Who)) {
        return NeighborSn[3];
      }
    }
  }
  if (NeighborSn[4] != CHESS_INVALID_NEIGHBOR) {
    if (ChessBlock[NeighborSn[4]].Status == Empty && (!CheckDanger || GetEnemyNeighborNum (ChessBlock, NeighborSn[4], Who) < 2)) {
      if (CheckPutCheck (ChessBlock, NeighborSn[4], Who)) {
        return NeighborSn[4];
      }
    }
  }
  if (NeighborSn[5] != CHESS_INVALID_NEIGHBOR) {
    if (ChessBlock[NeighborSn[5]].Status == Empty && (!CheckDanger || GetEnemyNeighborNum (ChessBlock, NeighborSn[5], Who) < 2)) {
      if (CheckPutCheck (ChessBlock, NeighborSn[5], Who)) {
        return NeighborSn[5];
      }
    }
  }

  return -1;
}

SOLAR_STATUS
SOLAR_API
Round (
  EFI_GRAPHICS_OUTPUT_PROTOCOL *pGraph,
  SOLAR_IMAGE_INPUT            DesktopImage,
  CHESS_BLOCK_INFO             *ChessBlock,
  BOOLEAN                      isSinglePlayer
  )
{
  SOLAR_STATUS                 Status;
  EFI_INPUT_KEY                KeyCode;
  UINT16                       OldBlockPointSn;
  UINT16                       NewBlockPointSn;
  SOLAR_IMAGE_INPUT            WhiteChessImage;
  SOLAR_IMAGE_INPUT            BlackChessImage;
  SOLAR_IMAGE_INPUT            WhiteLocationImage;
  SOLAR_IMAGE_INPUT            BlackLocationImage;
  SOLAR_IMAGE_INPUT            LocationBackgroundImage;
  UINT16                       Player1InfoBlockX1;
  UINT16                       Player1InfoBlockY1;
  UINT16                       Player2InfoBlockX1;
  UINT16                       Player2InfoBlockY1;
  UINT16                       ThinkInfoBlockX1;
  UINT16                       ThinkInfoBlockY1;
  IMAGE_BLOCK                  WhiteInfoBlock;
  IMAGE_BLOCK                  BlackInfoBlock;
  IMAGE_BLOCK                  DefaultInfoBlock;
  IMAGE_BLOCK                  ThinkInfoBlock;
  UINT16                       PlayerInfoBlockWidth;
  UINT16                       PlayerInfoBlockHeight;
  UINT16                       ScoreInfoX;
  UINT16                       ScoreInfoY;
  UINTN                        BmpWidth;
  UINTN                        BmpHeight;
  BOOLEAN                      Who;
  BOOLEAN                      isRivalComputer;
  CHAR16                       Player1Name[PLAYER_NAME_STRING_LENGTH];
  CHAR16                       Player2Name[PLAYER_NAME_STRING_LENGTH];
  UINT16                       BlackScore;
  UINT16                       WhiteScore;
  UINT32                       ScoreHiddenColor;
  BOOLEAN                      NoFreeBlock;
  BOOLEAN                      PlayerColor;

  OldBlockPointSn                   = 0;
  NewBlockPointSn                   = 0;
  WhiteChessImage.BltBuffer         = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) NULL;
  WhiteChessImage.Width             = CHESS_IMAGE_WIDTH;
  WhiteChessImage.Height            = CHESS_IMAGE_HEIGHT;
  BlackChessImage.BltBuffer         = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) NULL;
  BlackChessImage.Width             = CHESS_IMAGE_WIDTH;
  BlackChessImage.Height            = CHESS_IMAGE_HEIGHT;
  WhiteLocationImage.BltBuffer      = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) NULL;
  WhiteLocationImage.Width          = CHESS_IMAGE_WIDTH;
  WhiteLocationImage.Height         = CHESS_IMAGE_HEIGHT;
  BlackLocationImage.BltBuffer      = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) NULL;
  BlackLocationImage.Width          = CHESS_IMAGE_WIDTH;
  BlackLocationImage.Height         = CHESS_IMAGE_HEIGHT;
  LocationBackgroundImage.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) NULL;
  LocationBackgroundImage.Width     = CHESS_IMAGE_WIDTH;
  LocationBackgroundImage.Height    = CHESS_IMAGE_HEIGHT;
  Who                               = 0;
  isRivalComputer                   = isSinglePlayer;
  BlackScore                        = 3;
  WhiteScore                        = 3;
  PlayerColor                       = BLACK_SITE;

  if (isSinglePlayer) {
    Player1InfoBlockX1    = 175;
    Player1InfoBlockY1    = 304;
    Player2InfoBlockX1    = 175;
    Player2InfoBlockY1    = 204;
    PlayerInfoBlockWidth  = 124;
    PlayerInfoBlockHeight = 90;
    ScoreInfoX            = 77;
    ScoreInfoY            = 55;
    ScoreHiddenColor      = CombineRGB(255, 255, 255);
    ThinkInfoBlockX1      = 250;
    ThinkInfoBlockY1      = 129;
  } else {
    Player1InfoBlockX1    = 590;
    Player1InfoBlockY1    = 440;
    Player2InfoBlockX1    = 10;
    Player2InfoBlockY1    = 10;
    PlayerInfoBlockWidth  = 200;
    PlayerInfoBlockHeight = 150;
    ScoreInfoX            = 67;
    ScoreInfoY            = 43;
    ScoreHiddenColor      = CombineRGB(255, 239, 223);
    ThinkInfoBlockX1      = 325;
    ThinkInfoBlockY1      = 250;
  }

  // Allocate a screen size memory space for the image buffer.
  if ((WhiteChessImage.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool (WhiteChessImage.Width * WhiteChessImage.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL))) == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  if ((BlackChessImage.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool (BlackChessImage.Width * BlackChessImage.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL))) == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  if ((WhiteLocationImage.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool (WhiteLocationImage.Width * WhiteLocationImage.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL))) == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto RoundDone;
  }
  if ((BlackLocationImage.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool (BlackLocationImage.Width * BlackLocationImage.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL))) == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto RoundDone;
  }
  if ((LocationBackgroundImage.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool (LocationBackgroundImage.Width * LocationBackgroundImage.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL))) == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto RoundDone;
  }

  // Set information block size
  WhiteInfoBlock.BlockInit (pGraph, PlayerInfoBlockWidth, PlayerInfoBlockHeight, SOLAR_BLACK, SOLAR_GRAY);
  BlackInfoBlock.BlockInit (pGraph, PlayerInfoBlockWidth, PlayerInfoBlockHeight, SOLAR_BLACK, SOLAR_GRAY);
  ThinkInfoBlock.BlockInit (pGraph, DEFAULT_INFO_BLOCK_WIDTH - 50, DEFAULT_INFO_BLOCK_HEIGHT - 50, SOLAR_BLACK, SOLAR_GRAY);
  DefaultInfoBlock.BlockInit   (pGraph, DEFAULT_INFO_BLOCK_WIDTH, DEFAULT_INFO_BLOCK_HEIGHT, SOLAR_BLACK, SOLAR_GRAY);

  // Load location BMP image file into the image buffer.
  BmpFileToSolarImage (pGraph, WHITE_LOCATION_IMAGE_FILENAME,   &WhiteLocationImage,          &BmpWidth, &BmpHeight);
  BmpFileToSolarImage (pGraph, BLACK_LOCATION_IMAGE_FILENAME,   &BlackLocationImage,          &BmpWidth, &BmpHeight);
  BmpFileToSolarImage (pGraph, WHITE_CHESS_IMAGE_FILENAME,      &WhiteChessImage,             &BmpWidth, &BmpHeight);
  BmpFileToSolarImage (pGraph, BLACK_CHESS_IMAGE_FILENAME,      &BlackChessImage,             &BmpWidth, &BmpHeight);
  BmpFileToSolarImage (pGraph, (isSinglePlayer) ? INFO_BLOCK_01_IMAGE_FILENAME : WHITE_INFO_BLOCK_IMAGE_FILENAME, &WhiteInfoBlock.Foreground, &BmpWidth, &BmpHeight);
  BmpFileToSolarImage (pGraph, (isSinglePlayer) ? INFO_BLOCK_01_IMAGE_FILENAME : BLACK_INFO_BLOCK_IMAGE_FILENAME, &BlackInfoBlock.Foreground, &BmpWidth, &BmpHeight);
  BmpFileToSolarImage (pGraph, THINK_INFO_BLOCK_IMAGE_FILENAME, &ThinkInfoBlock.Foreground, &BmpWidth, &BmpHeight);

SingleRoundStart:

  // Set player name.
  EfiStrCpy ((CHAR16 *) &Player1Name, L"PLAYER 1");
  EfiStrCpy ((CHAR16 *) &Player2Name, (isSinglePlayer) ? L"COMPUTER" : L"PLAYER 2");
  PrintString (pGraph, &BlackInfoBlock.Foreground, (isSinglePlayer) ? 20 : 70, (isSinglePlayer) ? 20 : 11, (isSinglePlayer) ? SOLAR_BLACK : SOLAR_WHITE, Player1Name);
  PrintString (pGraph, &WhiteInfoBlock.Foreground, (isSinglePlayer) ? 20 : 70, (isSinglePlayer) ? 20 : 11, SOLAR_BLACK, Player2Name);

  Who = 0;

  BlackScore = 3;
  WhiteScore = 3;
  PrintString (pGraph, &BlackInfoBlock.Foreground, ScoreInfoX, ScoreInfoY, SOLAR_BLACK, L"%d", BlackScore);
  PrintString (pGraph, &WhiteInfoBlock.Foreground, ScoreInfoX, ScoreInfoY, SOLAR_BLACK, L"%d", WhiteScore);

  // Display player information block.
  BlackInfoBlock.Display (DesktopImage, Player1InfoBlockX1, Player1InfoBlockY1, true);
  WhiteInfoBlock.Display (DesktopImage, Player2InfoBlockX1, Player2InfoBlockY1, true);

  // Put the initial 6 checks. (3 black checks & 3 white checks)
  PUT_BLACK_CHECK (ChessBlock[(ChessBlockAmount / 2)].TopLeft);
  PUT_BLACK_CHECK (ChessBlock[(ChessBlockAmount / 2)].Left);
  PUT_BLACK_CHECK (ChessBlock[(ChessBlockAmount / 2)].LowerLeft);
  PUT_WHITE_CHECK (ChessBlock[(ChessBlockAmount / 2)].TopRight);
  PUT_WHITE_CHECK (ChessBlock[(ChessBlockAmount / 2)].Right);
  PUT_WHITE_CHECK (ChessBlock[(ChessBlockAmount / 2)].LowerRight);

  // Put the first location image.
  Status = LocationActive (
             pGraph,
             DesktopImage,
             Who?WhiteLocationImage:BlackLocationImage,
             LocationBackgroundImage,
             ChessBlock,
             OldBlockPointSn
             );
  if (EFI_ERROR (Status)) {
    Status = EFI_DEVICE_ERROR;
    goto RoundDone;
  }
  
#ifndef EFI_NT_EMULATOR
  /*
  // The SimplePointer Protocol doesn't support NT32 emulator, so jump these code if run in the emulator.
  Status = (EFI_STATUS) InitiateGraphicsPointer (&pGraph, DesktopImage.Height, DesktopImage.Width);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  
  Status = (EFI_STATUS) EnableSimplePointer ();
  if (EFI_ERROR(Status)) {
    return Status;
  }

  while (1) {
    PointerState.LButton = false;
    Status = (EFI_STATUS) GetPointerState (&PointerState);
    if (EFI_ERROR(Status)) {
      return Status;
    }
    if (PointerState.LButton) break;
  }
  */
#endif  // EFI_NT_EMULATOR

  // Only keyboard support if run in the emulator.
  while (true) {

    // Check any block which can put check.
    NoFreeBlock = true;
    for (INT16 i = 0; i < ChessBlockAmount; i ++) {
      if (CheckPutCheck (ChessBlock, i, Who)) {
        NoFreeBlock = false;
        break;
      }
    }

    // No free block, accounting the round of game.
    if (NoFreeBlock) {
      goto SingleRoundEnd;        // End this round.
    }

    getch (&KeyCode);
    switch (KeyCode.ScanCode) {
      case SCAN_ESC:
        {
          UINT16 ItemPtr = 0xFF;
          UINT16 ItemNum = 2;

          BmpFileToSolarImage (pGraph, ABANDON_ROUND_MSG_WIN_FILENAME,   &DefaultInfoBlock.Foreground,   &BmpWidth, &BmpHeight);
          DefaultInfoBlock.Display (DesktopImage, BLOCK_SET_TO_CENTER_X(DEFAULT_INFO_BLOCK_WIDTH), BLOCK_SET_TO_CENTER_Y(DEFAULT_INFO_BLOCK_HEIGHT), true);
          ItemPtr = SelectItem (
                      pGraph, 
                      DesktopImage, 
                      140, 
                      ItemNum * ITEM_HEIGHT, 
                      BLOCK_SET_TO_CENTER_X(140), 
                      BLOCK_SET_TO_CENTER_Y(ItemNum * ITEM_HEIGHT) + 10, 
                      SOLAR_BLACK, 
                      SOLAR_WHITE, 
                      SOLAR_WHITE, 
                      SOLAR_RED, 
                      ItemNum, 
                      L"No, continue this round.\\Yes, break this round.\\"
                      );
          DefaultInfoBlock.Hidden ();
          if (ItemPtr == 1) {
            goto SingleRoundEnd;
          }
        }
        break;
      case SCAN_UP:
      case SCAN_DOWN:
      case SCAN_LEFT:
      case SCAN_RIGHT:

        // Put back the old-blockpoint background image.
        Status = TransparentBltBufferToVideo (
                   pGraph,
                   &LocationBackgroundImage,
                   &DesktopImage,
                   ChessBlock[OldBlockPointSn].Position.x - (CHESS_IMAGE_WIDTH / 2),
                   ChessBlock[OldBlockPointSn].Position.y - (CHESS_IMAGE_HEIGHT / 2),
                   TransparentColor
                   );
        if (EFI_ERROR (Status)) {
          Status = EFI_DEVICE_ERROR;
          goto RoundDone;
        }

        // Get the new-blockpoint S/N.
        NewBlockPointSn = CalculateLocationWhenDirectionKeyEvent (
                            ChessBlock,
                            OldBlockPointSn,
                            KeyCode.ScanCode
                            );

        // Put the location image.
        Status = LocationActive (
                   pGraph,
                   DesktopImage,
                   Who?WhiteLocationImage:BlackLocationImage,
                   LocationBackgroundImage,
                   ChessBlock,
                   NewBlockPointSn
                   );
        if (EFI_ERROR (Status)) {
          Status = EFI_DEVICE_ERROR;
          goto RoundDone;
        }
  
        OldBlockPointSn = NewBlockPointSn;
        break;

      case SCAN_NULL:

        switch (KeyCode.UnicodeChar) {
          case CHAR_CARRIAGE_RETURN:    // Enter Key
          case CHAR_SPACE:              // Space Key

            if (CheckPutCheck (ChessBlock, OldBlockPointSn, Who)) {
              // Restore the block background image.
              Status = TransparentBltBufferToVideo (
                         pGraph,
                         &LocationBackgroundImage,
                         &DesktopImage,
                         ChessBlock[OldBlockPointSn].Position.x - (CHESS_IMAGE_WIDTH / 2),
                         ChessBlock[OldBlockPointSn].Position.y - (CHESS_IMAGE_HEIGHT / 2),
                         TransparentColor
                         );
              if (EFI_ERROR (Status)) {
                Status = EFI_DEVICE_ERROR;
                goto RoundDone;
              }

              // Erase the old information in the block
              PrintString (pGraph, &BlackInfoBlock.Foreground, ScoreInfoX, ScoreInfoY, ScoreHiddenColor, L"%d", BlackScore);
              PrintString (pGraph, &WhiteInfoBlock.Foreground, ScoreInfoX, ScoreInfoY, ScoreHiddenColor, L"%d", WhiteScore);
              BlackInfoBlock.Hidden ();
              WhiteInfoBlock.Hidden ();

              // Put the new check on the block.
              if (Who) {
                PUT_WHITE_CHECK (OldBlockPointSn);
                WhiteScore ++;
              } else {
                PUT_BLACK_CHECK (OldBlockPointSn);
                BlackScore ++;
              }

              // Re-draw the location image.
              Status = LocationActive (
                         pGraph,
                         DesktopImage,
                         Who?WhiteLocationImage:BlackLocationImage,
                         LocationBackgroundImage,
                         ChessBlock,
                         OldBlockPointSn
                         );
              if (EFI_ERROR (Status)) {
                Status = EFI_DEVICE_ERROR;
                goto RoundDone;
              }

              // Check expel enemy when player put their check. 
              ExpelEnemy (pGraph, DesktopImage, WhiteChessImage, BlackChessImage, ChessBlock, OldBlockPointSn, Who, &BlackScore, &WhiteScore);

              // Display the new information in the block
              PrintString (pGraph, &BlackInfoBlock.Foreground, ScoreInfoX, ScoreInfoY, SOLAR_BLACK, L"%d", BlackScore);
              PrintString (pGraph, &WhiteInfoBlock.Foreground, ScoreInfoX, ScoreInfoY, SOLAR_BLACK, L"%d", WhiteScore);
              BlackInfoBlock.Display (DesktopImage, Player1InfoBlockX1, Player1InfoBlockY1, false);
              WhiteInfoBlock.Display (DesktopImage, Player2InfoBlockX1, Player2InfoBlockY1, false);

              Who = ENEMY(Who);         // Change the role.

              // Computer A.I. program - starts
              if (isRivalComputer) {

                INT16 StartNum;
                INT16 StepVal;
                INT16 TmpSn = -1;

                // Show computer is thinking...
                ThinkInfoBlock.Display (DesktopImage, ThinkInfoBlockX1, ThinkInfoBlockY1, true);

                if (RandomInitSeed () % 2) {
                  StartNum = 0;
                  StepVal = 1;
                } else {
                  StartNum = ChessBlockAmount - 1;
                  StepVal = -1;
                }

                // Check if put check for attack will be success.
                if (TmpSn == -1) {
                  for (INT16 i = StartNum, j = 0; j < ChessBlockAmount; j ++, i = i + StepVal) {
                    if (ChessBlock[i].Status == ENEMY(Who) && GetEnemyNeighborNum (ChessBlock, i, ENEMY(Who)) == 2) {
                      if ((TmpSn = GetBlockSnForPutAppropriateSelfNeighborAi(ChessBlock, i, Who, NO_CHECK_DANGER)) != -1) {
                        break;
                      }
                    }
                  }
                }

                // Check danger check, if chess have 2 enemy check near by.
                if (TmpSn == -1) {
                  for (INT16 i = StartNum, j = 0; j < ChessBlockAmount; j ++, i = i + StepVal) {
                    if (ChessBlock[i].Status == Who && GetEnemyNeighborNum (ChessBlock, i, Who) == 2) {
                      if ((TmpSn = GetBlockSnForPutAppropriateSelfNeighborAi(ChessBlock, i, Who, CHECK_DANGER)) != -1) {
                        break;
                      }
                    }
                  }
                }

                // Check if put check for obstruct will be well.
                if (TmpSn == -1) {
                  for (INT16 i = StartNum, j = 0; j < ChessBlockAmount; j ++, i = i + StepVal) {
                    if (ChessBlock[i].Status == ENEMY(Who) && GetEnemyNeighborNum (ChessBlock, i, ENEMY(Who)) == 1) {
                      if ((TmpSn = GetBlockSnForPutAppropriateSelfNeighborAi(ChessBlock, i, Who, CHECK_DANGER)) != -1) {
                        break;
                      }
                    }
                  }
                }

                // Check if put check for near by self check.
                if (TmpSn == -1) {
                  for (INT16 i = StartNum, j = 0; j < ChessBlockAmount; j ++, i = i + StepVal) {
                    if (ChessBlock[i].Status == Who) {
                      if ((TmpSn = GetBlockSnForPutAppropriateSelfNeighborAi(ChessBlock, i, Who, CHECK_DANGER)) != -1) {
                        break;
                      }
                    }
                  }
                }

                NoFreeBlock = false;
                // Check any block which can put check.
                if (TmpSn == -1) {
                  NoFreeBlock = true;
                  for (INT16 i = StartNum, j = 0; j < ChessBlockAmount; j ++, i = i + StepVal) {
                    if (CheckPutCheck (ChessBlock, i, Who)) {
                      TmpSn = i;
                      NoFreeBlock = false;
                      break;
                    }
                  }
                }

                ThinkInfoBlock.Hidden ();

                // No free block, accounting the round of game.
                if (NoFreeBlock) {
                  goto SingleRoundEnd;        // End this round.
                }

                Who ? PUT_WHITE_CHECK (TmpSn) : PUT_BLACK_CHECK (TmpSn);
                // Blink - starts
                gST->BootServices->Stall (500000);  // For delay
                Status = TransparentBltBufferToVideo (
                           pGraph,
                           &ChessBlock[TmpSn].BackgroundImage,
                           &DesktopImage,
                           ChessBlock[TmpSn].Position.x - (CHESS_IMAGE_WIDTH / 2),
                           ChessBlock[TmpSn].Position.y - (CHESS_IMAGE_HEIGHT / 2),
                           TransparentColor
                           );
                if (EFI_ERROR (Status)) {
                  Status = EFI_DEVICE_ERROR;
                  goto RoundDone;
                }
                gST->BootServices->Stall (500000);  // For delay
                // Blink - ends
                Who ? PUT_WHITE_CHECK (TmpSn) : PUT_BLACK_CHECK (TmpSn);

                // Erase the old information in the block
                PrintString (pGraph, &BlackInfoBlock.Foreground, ScoreInfoX, ScoreInfoY, ScoreHiddenColor, L"%d", BlackScore);
                PrintString (pGraph, &WhiteInfoBlock.Foreground, ScoreInfoX, ScoreInfoY, ScoreHiddenColor, L"%d", WhiteScore);
                BlackInfoBlock.Hidden ();
                WhiteInfoBlock.Hidden ();

                Who ? WhiteScore ++ : BlackScore ++;

                // Put back the old-blockpoint background image.
                Status = TransparentBltBufferToVideo (
                           pGraph,
                           &LocationBackgroundImage,
                           &DesktopImage,
                           ChessBlock[OldBlockPointSn].Position.x - (CHESS_IMAGE_WIDTH / 2),
                           ChessBlock[OldBlockPointSn].Position.y - (CHESS_IMAGE_HEIGHT / 2),
                           TransparentColor
                           );
                if (EFI_ERROR (Status)) {
                  Status = EFI_DEVICE_ERROR;
                  goto RoundDone;
                }

                // Check expel enemy when player put their check. 
                ExpelEnemy (pGraph, DesktopImage, WhiteChessImage, BlackChessImage, ChessBlock, TmpSn, Who, &BlackScore, &WhiteScore);

                // Put the location image.
                Status = LocationActive (
                           pGraph,
                           DesktopImage,
                           ENEMY(Who)?WhiteLocationImage:BlackLocationImage,
                           LocationBackgroundImage,
                           ChessBlock,
                           NewBlockPointSn
                           );
                if (EFI_ERROR (Status)) {
                  Status = EFI_DEVICE_ERROR;
                  goto RoundDone;
                }

                // Display the new information in the block
                PrintString (pGraph, &BlackInfoBlock.Foreground, ScoreInfoX, ScoreInfoY, SOLAR_BLACK, L"%d", BlackScore);
                PrintString (pGraph, &WhiteInfoBlock.Foreground, ScoreInfoX, ScoreInfoY, SOLAR_BLACK, L"%d", WhiteScore);
                BlackInfoBlock.Display (DesktopImage, Player1InfoBlockX1, Player1InfoBlockY1, false);
                WhiteInfoBlock.Display (DesktopImage, Player2InfoBlockX1, Player2InfoBlockY1, false);

                Who = ENEMY(Who);         // Change the role.
              }
              // Computer A.I. program - ends
            }
            break;
        }
        break;
    }
  }

SingleRoundEnd:

  // Erase the old information in the block
  PrintString (pGraph, &BlackInfoBlock.Foreground, ScoreInfoX, ScoreInfoY, ScoreHiddenColor, L"%d", BlackScore);
  PrintString (pGraph, &WhiteInfoBlock.Foreground, ScoreInfoX, ScoreInfoY, ScoreHiddenColor, L"%d", WhiteScore);
  BlackInfoBlock.Hidden ();
  WhiteInfoBlock.Hidden ();
  for (INT16 i = 0; i < ChessBlockAmount; i ++) {
    if (CheckPutCheck (ChessBlock, i, ENEMY(Who))) {
      ENEMY(Who) ? WhiteScore ++ : BlackScore ++;
    }
  }
  if (NoFreeBlock) {
    // Display the new information in the block
    PrintString (pGraph, &BlackInfoBlock.Foreground, ScoreInfoX, ScoreInfoY, SOLAR_BLACK, L"%d", BlackScore);
    PrintString (pGraph, &WhiteInfoBlock.Foreground, ScoreInfoX, ScoreInfoY, SOLAR_BLACK, L"%d", WhiteScore);
    BlackInfoBlock.Display (DesktopImage, Player1InfoBlockX1, Player1InfoBlockY1, false);
    WhiteInfoBlock.Display (DesktopImage, Player2InfoBlockX1, Player2InfoBlockY1, false);

    if (BlackScore > WhiteScore) {
      BmpFileToSolarImage (pGraph, (PlayerColor ? LOSE_MSG_WIN_FILENAME : WIN_MSG_WIN_FILENAME), &DefaultInfoBlock.Foreground, &BmpWidth, &BmpHeight);
    } else if (BlackScore < WhiteScore) {
      BmpFileToSolarImage (pGraph, (PlayerColor ? WIN_MSG_WIN_FILENAME : LOSE_MSG_WIN_FILENAME), &DefaultInfoBlock.Foreground, &BmpWidth, &BmpHeight);
    } else {
      BmpFileToSolarImage (pGraph, DRAW_MSG_WIN_FILENAME, &DefaultInfoBlock.Foreground, &BmpWidth, &BmpHeight);
    }
    DefaultInfoBlock.Display (DesktopImage, BLOCK_SET_TO_CENTER_X(DEFAULT_INFO_BLOCK_WIDTH), BLOCK_SET_TO_CENTER_Y(DEFAULT_INFO_BLOCK_HEIGHT), true);

    while (true) {
      getch (&KeyCode);
      if (KeyCode.ScanCode != SCAN_NULL || (KeyCode.ScanCode == SCAN_NULL && KeyCode.UnicodeChar != CHAR_NULL)) {
        break;
      }
    }
    DefaultInfoBlock.Hidden ();

    PrintString (pGraph, &BlackInfoBlock.Foreground, ScoreInfoX, ScoreInfoY, ScoreHiddenColor, L"%d", BlackScore);
    PrintString (pGraph, &WhiteInfoBlock.Foreground, ScoreInfoX, ScoreInfoY, ScoreHiddenColor, L"%d", WhiteScore);
    BlackInfoBlock.Hidden ();
    WhiteInfoBlock.Hidden ();
  }

  {
    UINT16 ItemPtr = 0xFF;
    UINT16 ItemNum = 2;

    BmpFileToSolarImage (pGraph, EXIT_GAME_MSG_WIN_FILENAME,   &DefaultInfoBlock.Foreground,   &BmpWidth, &BmpHeight);
    DefaultInfoBlock.Display (DesktopImage, BLOCK_SET_TO_CENTER_X(DEFAULT_INFO_BLOCK_WIDTH), BLOCK_SET_TO_CENTER_Y(DEFAULT_INFO_BLOCK_HEIGHT), true);
    while (ItemPtr == 0xFF) {
      ItemPtr = SelectItem (
                  pGraph, 
                  DesktopImage, 
                  140, 
                  ItemNum * ITEM_HEIGHT, 
                  BLOCK_SET_TO_CENTER_X(140), 
                  BLOCK_SET_TO_CENTER_Y(ItemNum * ITEM_HEIGHT) + 10, 
                  SOLAR_BLACK, 
                  SOLAR_WHITE, 
                  SOLAR_WHITE, 
                  SOLAR_RED, 
                  ItemNum, 
                  L"No, go to next round.\\Yes, exit game.\\"
                  );
    }
    DefaultInfoBlock.Hidden ();
    switch (ItemPtr) {
      case 0:
        for (UINT16 i = 0; i < ChessBlockAmount; i ++) {
          ChessBlock[i].Status = Empty;
          Status = TransparentBltBufferToVideo (
                     pGraph,
                     &ChessBlock[i].BackgroundImage,
                     &DesktopImage,
                     ChessBlock[i].Position.x - (CHESS_IMAGE_WIDTH / 2),
                     ChessBlock[i].Position.y - (CHESS_IMAGE_HEIGHT / 2),
                     TransparentColor
                     );
          if (EFI_ERROR (Status)) {
            Status = EFI_DEVICE_ERROR;
            goto RoundDone;
          }
        }
        goto SingleRoundStart;
      case 1:
        goto RoundDone;
    }
  }

////////////////////
// Exit RoundDone //
////////////////////
RoundDone:                            // Exit RunGame.

  if (WhiteLocationImage.BltBuffer) {
    gBS->FreePool (WhiteLocationImage.BltBuffer);
  }
  if (BlackLocationImage.BltBuffer) {
    gBS->FreePool (BlackLocationImage.BltBuffer);
  }
  if (LocationBackgroundImage.BltBuffer) {
    gBS->FreePool (LocationBackgroundImage.BltBuffer);
  }
  if (BlackChessImage.BltBuffer) {
    gBS->FreePool (BlackChessImage.BltBuffer);
  }
  if (WhiteChessImage.BltBuffer) {
    gBS->FreePool (WhiteChessImage.BltBuffer);
  }

  return SOLAR_SUCCESS;
}