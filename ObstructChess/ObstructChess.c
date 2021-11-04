/*++ 

Copyright (c) 2008 - 2010, Solar Studio                                                         
         
Module Name:

  ObstructChess.c

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

extern "C" {
  EFI_APPLICATION_ENTRY_POINT (ObstructChess);
}

UINT16                        ChessBlockEndColNum[CHESS_BOARD_CHECK_BLOCK_ROW_MAX_NUM];
UINT16                        ChessBlockAmount;
UINT16                        CheckBlockRowNum;
EFI_GRAPHICS_OUTPUT_BLT_PIXEL TransparentColor;

UINT16 CalculateChessBlockAmount (UINT8 CheckBlockOneSide) 
{
  UINT16 Amount = 0;

  for (UINT16 i = 0; i < CheckBlockOneSide - 1; i++) {
    Amount += ((CheckBlockOneSide + i) * 2);
  }
  Amount += (CheckBlockOneSide + i);

  return Amount;
}

SOLAR_STATUS
SOLAR_API
CalculateNeighbor (
  CHESS_BLOCK_INFO **ChessBlock,
  UINT16           Sn,
  UINT16           CheckBlockRowNum
  ) 
{
  UINT16           Col;
  UINT16           Row;
  UINT16           EndColThisRow;
  UINT16           ColRightCount;
  UINT16           ColLeftCount;

  Col           = ((*ChessBlock) + Sn)->Col;
  Row           = ((*ChessBlock) + Sn)->Row;
  EndColThisRow = ChessBlockEndColNum[Row];
  ColRightCount = EndColThisRow - Col - 1;
  ColLeftCount  = Col;

  ((*ChessBlock) + Sn)->TopLeft    = CHESS_INVALID_NEIGHBOR;
  ((*ChessBlock) + Sn)->TopRight   = CHESS_INVALID_NEIGHBOR;
  ((*ChessBlock) + Sn)->Right      = CHESS_INVALID_NEIGHBOR;
  ((*ChessBlock) + Sn)->LowerRight = CHESS_INVALID_NEIGHBOR;
  ((*ChessBlock) + Sn)->LowerLeft  = CHESS_INVALID_NEIGHBOR;
  ((*ChessBlock) + Sn)->Left       = CHESS_INVALID_NEIGHBOR;

  // Not 1st ROW.
  if (Row > 0) {
    // Set top-left & top-right neighbor S/N.
    if (EndColThisRow > ChessBlockEndColNum[Row - 1]) { // if in the upper half area of chess board.
      if (ColRightCount < ChessBlockEndColNum[Row - 1]) { // if not the most left block.
        ((*ChessBlock) + Sn)->TopLeft = Sn - ColLeftCount - ColRightCount - 1; // set top-left.
      }
      if (ColLeftCount < ChessBlockEndColNum[Row - 1]) { // if not the most right block.
        ((*ChessBlock) + Sn)->TopRight = Sn - ColLeftCount - ColRightCount; // set top-right.
      }
    } else { // if in the Lower half area of chess board.
      ((*ChessBlock) + Sn)->TopLeft = Sn - ColLeftCount - ColRightCount - 2; // set top-left.
      ((*ChessBlock) + Sn)->TopRight = Sn - ColLeftCount - ColRightCount - 1; // set top-right.
    }
  }

  // Set left neighbor S/N.
  if (ColLeftCount > 0) {
    ((*ChessBlock) + Sn)->Left = Sn - 1;
  }
  
  // Set right neighbor S/N.
  if (ColRightCount > 0) {
    ((*ChessBlock) + Sn)->Right = Sn + 1;
  }

  // Not latest ROW.
  if (Row < CheckBlockRowNum - 1) {
    // Set lower-left & lower-right neighbor S/N.
    if (EndColThisRow < ChessBlockEndColNum[Row + 1]) { // if in the upper half area of chess board.
      ((*ChessBlock) + Sn)->LowerLeft = Sn + ColRightCount + ColLeftCount + 1; // set lower-left.
      ((*ChessBlock) + Sn)->LowerRight = Sn + ColRightCount + ColLeftCount + 2; // set lower-right.
    } else { // if in the Lower half area of chess board.
      if (ColRightCount < ChessBlockEndColNum[Row + 1]) { // if not the most left block.
        ((*ChessBlock) + Sn)->LowerLeft = Sn + ColRightCount + ColLeftCount; // set lower-left.
      }
      if (ColLeftCount < ChessBlockEndColNum[Row + 1]) { // if not the most right block.
        ((*ChessBlock) + Sn)->LowerRight = Sn + ColRightCount + ColLeftCount + 1; // set lower-right.
      }
    }  
  }

  return SOLAR_SUCCESS;
}

SOLAR_STATUS
SOLAR_API
InitiateGraphicsMode (
  EFI_GRAPHICS_OUTPUT_PROTOCOL **pGraph,
  UINT16                       HorizontalResolution,
  UINT16                       VerticalResolution,
  UINT32                       *ModeNum
  )
{
  EFI_SIMPLE_TEXT_OUT_PROTOCOL *COut;

  // Get the console service table entry point.
  COut = gST->ConOut;
  
  // Check the Solar graphics library is supported.
  if (InitiateSolarGraphicsLib (pGraph) != SOLAR_SUCCESS) {
    COut->OutputString (COut, L"\n>> SolarGraphicsLib is not ready.");
    return SOLAR_UNSUCCESS;
  }

  QueryRequestMode (*pGraph, HorizontalResolution, VerticalResolution, ModeNum);

  FillScreen (*pGraph, SOLAR_BLACK);

  return SOLAR_SUCCESS;
}

SOLAR_STATUS
SOLAR_API
InitiateGame (
  EFI_GRAPHICS_OUTPUT_PROTOCOL **pGraph,
  SOLAR_IMAGE_INPUT            *DesktopImage,
  CHESS_BLOCK_INFO             **ChessBlock,
  BOOLEAN                      *isSinglePlayer
  )
{
  EFI_STATUS                   Status;
  UINTN                        BmpWidth;
  UINTN                        BmpHeight;
  UINT8                        CheckBlockOneSideOnChessBoard;
  UINT16                       CheckBlockInterval;
  UINT16                       ChessBlockWidth;
  UINT16                       ChessBlockHeight;
  UINT16                       ChessBoardX1;
  UINT16                       ChessBoardY1;
  UINT16                       ChessBoardX2;
  UINT16                       ChessBoardY2;
  UINT16                       ChessBoardLUX;
  UINT16                       ChessBoardLUY;
  UINT16                       ChessBoardRUX;
  UINT16                       ChessBoardRUY;
  UINT16                       ChessBoardLX;
  UINT16                       ChessBoardLY;
  UINT16                       ChessBoardRX;
  UINT16                       ChessBoardRY;
  UINT16                       ChessBoardLBX;
  UINT16                       ChessBoardLBY;
  UINT16                       ChessBoardRBX;
  UINT16                       ChessBoardRBY;

  // Set transparent color for all images of ObstructChess program. 
  TransparentColor.Blue = TransparentColor.Green = TransparentColor.Red = CHESS_IMAGE_TRANSPARENT;

  // Allocate a screen size memory for buffer.
  if ((DesktopImage->BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool (DesktopImage->Width * DesktopImage->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL))) == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  // Fill the black color into buffer.
  EfiSetMem (DesktopImage->BltBuffer, DesktopImage->Width * DesktopImage->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);

  // Show background
  EfiSetMem (DesktopImage->BltBuffer, DesktopImage->Width * DesktopImage->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);
  FillScreen (*pGraph, SOLAR_BLACK);    // Fill black color in to background.
  BmpFileToSolarImage (*pGraph, INIT_BACKGROUND_FILE, DesktopImage, &BmpWidth, &BmpHeight);
  Status = (*pGraph)->Blt (
             *pGraph,
             DesktopImage->BltBuffer,
             EfiBltBufferToVideo,
             0,
             0,
             0,
             0,
             DesktopImage->Width,
             DesktopImage->Height,
             0
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  // Select Player Mode - starts
  {
    UINT16 ItemPtr = 0xFF;
    UINT16 ItemNum = 4;

    *isSinglePlayer = true;
    while (ItemPtr == 0xFF) {
      ItemPtr = SelectItem (
                *pGraph, 
                *DesktopImage, 
                102, 
                ItemNum * ITEM_HEIGHT, 
                BLOCK_SET_TO_CENTER_X(102), 
                BLOCK_SET_TO_CENTER_Y(ItemNum * ITEM_HEIGHT) + 30, 
                SOLAR_BLACK, 
                SOLAR_WHITE, 
                SOLAR_WHITE, 
                SOLAR_RED, 
                ItemNum, 
                L"SINGLE\\TWO PLAYERS\\GAME RULES\\EXIT GAME\\"
                );
    }
    switch (ItemPtr) {
      case 0 :                            // SINGLE
        *isSinglePlayer = true;
        break;
      case 1 :                            // TWO PLAYERS
        *isSinglePlayer = false;
        break;
      case 2 :                            // GAME RULES
        return EFI_LOAD_ERROR;
        break;
      case 3 :                            // EXIT GAME
        return EFI_LOAD_ERROR;
        break;
    }
  }
  // Select Player Mode - ends

  // Chess board setting - starts
  {
    UINT16 ItemPtr = 0xFF;
    UINT16 ItemNum = 4;

    while (ItemPtr == 0xFF) {
      ItemPtr = SelectItem (
                *pGraph, 
                *DesktopImage, 
                102, 
                ItemNum * ITEM_HEIGHT, 
                BLOCK_SET_TO_CENTER_X(102), 
                BLOCK_SET_TO_CENTER_Y(ItemNum * ITEM_HEIGHT) + 30, 
                SOLAR_BLACK, 
                SOLAR_WHITE, 
                SOLAR_WHITE, 
                SOLAR_BLUE, 
                ItemNum, 
                L"5 x 5 x 5\\7 x 7 x 7\\9 x 9 x 9\\11 x 11 x 11\\"
                );
    }
    CheckBlockOneSideOnChessBoard = 11;
    switch (ItemPtr) {
      case 0 :                            // 5x5x5
        CheckBlockOneSideOnChessBoard = 5;
        break;
      case 1 :                            // 7x7x7
        CheckBlockOneSideOnChessBoard = 7;
        break;
      case 2 :                            // 9x9x9
        CheckBlockOneSideOnChessBoard = 9;
        break;
      case 3 :                            // 11x11x11
        CheckBlockOneSideOnChessBoard = 11;
        break;
    }
    CheckBlockInterval = (CHESS_BOARD_WIDTH - 1) / (CheckBlockOneSideOnChessBoard - 1) / 2;
    CheckBlockRowNum   = (CheckBlockOneSideOnChessBoard - 1) * 2 + 1;
    ChessBlockWidth    = ChessBlockHeight = CheckBlockInterval;
  }
  // Chess board setting - ends

  // Calculate chess check amount in the board.
  ChessBlockAmount   = CalculateChessBlockAmount (CheckBlockOneSideOnChessBoard);

  // Allocate chess block information storage.
  if ((*ChessBlock = (CHESS_BLOCK_INFO *) EfiLibAllocatePool (ChessBlockAmount * sizeof(CHESS_BLOCK_INFO))) == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  for (UINT16 i = 0; i < ChessBlockAmount; i ++) {
    ((*ChessBlock) + i)->BackgroundImage.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) NULL;
  }

  // Draw background & chess board.
  if (*isSinglePlayer) {
    BmpFileToSolarImage (*pGraph, CHESS_BOARD_BACKGROUND_FILE2, DesktopImage, &BmpWidth, &BmpHeight);
    ChessBoardX1  = 312;
    ChessBoardY1  = CHESS_BOARD_Y1;
  } else {
    BmpFileToSolarImage (*pGraph, CHESS_BOARD_BACKGROUND_FILE1, DesktopImage, &BmpWidth, &BmpHeight);
    ChessBoardX1  = CHESS_BOARD_X1;
    ChessBoardY1  = CHESS_BOARD_Y1;
  }
  ChessBoardX2  = ChessBoardX1 + CHESS_BOARD_WIDTH - 1;
  ChessBoardY2  = ChessBoardY1 + CHESS_BOARD_HEIGHT - 1;
  ChessBoardLUX = ChessBoardX1 + ((CHESS_BOARD_WIDTH - 1) / 4);
  ChessBoardLUY = ChessBoardY1;
  ChessBoardRUX = ChessBoardX2 - ((CHESS_BOARD_WIDTH - 1) / 4);
  ChessBoardRUY = ChessBoardY1;
  ChessBoardLX  = ChessBoardX1;
  ChessBoardLY  = ChessBoardY1 + ((CHESS_BOARD_WIDTH - 1) / 2);
  ChessBoardRX  = ChessBoardX2;
  ChessBoardRY  = ChessBoardY1 + ((CHESS_BOARD_WIDTH - 1) / 2);
  ChessBoardLBX = ChessBoardLUX;
  ChessBoardLBY = ChessBoardY2;
  ChessBoardRBX = ChessBoardRUX;
  ChessBoardRBY = ChessBoardY2;
  DrawLine (*pGraph, DesktopImage, ChessBoardLUX, ChessBoardLUY, ChessBoardRUX, ChessBoardRUY, SOLAR_BLACK, FullLine);
  DrawLine (*pGraph, DesktopImage, ChessBoardRUX, ChessBoardRUY, ChessBoardRX, ChessBoardRY, SOLAR_BLACK, FullLine);
  DrawLine (*pGraph, DesktopImage, ChessBoardRX, ChessBoardRY, ChessBoardRBX, ChessBoardRBY, SOLAR_BLACK, FullLine);
  DrawLine (*pGraph, DesktopImage, ChessBoardRBX, ChessBoardRBY, ChessBoardLBX, ChessBoardLBY, SOLAR_BLACK, FullLine);
  DrawLine (*pGraph, DesktopImage, ChessBoardLBX, ChessBoardLBY, ChessBoardLX, ChessBoardLY, SOLAR_BLACK, FullLine);
  DrawLine (*pGraph, DesktopImage, ChessBoardLX, ChessBoardLY, ChessBoardLUX, ChessBoardLUY, SOLAR_BLACK, FullLine);
  for (UINT16 i = 1; i <= (CheckBlockOneSideOnChessBoard - 1); i ++) {
    DrawLine (*pGraph, DesktopImage, ChessBoardLUX - i * (CheckBlockInterval / 2), ChessBoardLUY + i * CheckBlockInterval, ChessBoardRUX + i * (CheckBlockInterval / 2), ChessBoardRUY + i * CheckBlockInterval, SOLAR_BLACK, FullLine);
    DrawLine (*pGraph, DesktopImage, ChessBoardLUX - i * (CheckBlockInterval / 2), ChessBoardLBY - i * CheckBlockInterval, ChessBoardRUX + i * (CheckBlockInterval / 2), ChessBoardRBY - i * CheckBlockInterval, SOLAR_BLACK, FullLine);
    DrawLine (*pGraph, DesktopImage, ChessBoardRUX - i * CheckBlockInterval, ChessBoardRUY, ChessBoardRX - i * (CheckBlockInterval / 2), ChessBoardRY + i * CheckBlockInterval, SOLAR_BLACK, FullLine);
    DrawLine (*pGraph, DesktopImage, ChessBoardLUX + i * CheckBlockInterval, ChessBoardLUY, ChessBoardLX + i * (CheckBlockInterval / 2), ChessBoardLY + i * CheckBlockInterval, SOLAR_BLACK, FullLine);
    DrawLine (*pGraph, DesktopImage, ChessBoardRBX - i * CheckBlockInterval, ChessBoardRBY, ChessBoardRX - i * (CheckBlockInterval / 2), ChessBoardRY - i * CheckBlockInterval, SOLAR_BLACK, FullLine);
    DrawLine (*pGraph, DesktopImage, ChessBoardLBX + i * CheckBlockInterval, ChessBoardLBY, ChessBoardLX + i * (CheckBlockInterval / 2), ChessBoardLY - i * CheckBlockInterval, SOLAR_BLACK, FullLine);
  }
  Status = (*pGraph)->Blt (
             *pGraph,
             DesktopImage->BltBuffer,
             EfiBltBufferToVideo,
             0,
             0,
             0,
             0,
             DesktopImage->Width,
             DesktopImage->Height,
             0
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  // Calculate col number of every row.
  for (UINT16 i = 0, j = CheckBlockOneSideOnChessBoard;
       i < CheckBlockOneSideOnChessBoard;
       i ++) {
    ChessBlockEndColNum[i] = j;
    if (i < CheckBlockOneSideOnChessBoard - 1) {
      ChessBlockEndColNum[CheckBlockRowNum - 1 - i] = j;
      j ++;
    }
  }

  { // Block Start - Set position of the chess board block.
    UINT16 StartX   = ChessBoardLUX;
    UINT16 EndX     = ChessBoardRUX;
    UINT16 StartY   = ChessBoardLUY;
    UINT16 x        = StartX;
    UINT16 y        = StartY;
    UINT16 i        = 0;
    UINT16 RowCount = 0;
    UINT16 ColCount = 0;
    do {
      ((*ChessBlock) + i)->Status = Empty;
      ((*ChessBlock) + i)->Position.x = x;
      ((*ChessBlock) + i)->Position.y = y;
      ((*ChessBlock) + i)->Col = ColCount;
      ((*ChessBlock) + i)->Row = RowCount;
      CalculateNeighbor (ChessBlock, i, CheckBlockRowNum);
      i ++;
      ColCount ++;
      x = x + ChessBlockWidth;
      if (x > EndX) {
        if (RowCount < CheckBlockOneSideOnChessBoard - 1) {
          StartX -= (ChessBlockWidth / 2);
          EndX += (ChessBlockWidth / 2);
        } else {
          StartX += (ChessBlockWidth / 2);
          EndX -= (ChessBlockWidth / 2);
        }
        x = StartX;
        y = y + ChessBlockHeight;
        ColCount = 0;
        RowCount ++;
      }
    } while (i < ChessBlockAmount);
  } // Block End

  for (UINT16 i = 0; i < ChessBlockAmount; i ++) {
    ((*ChessBlock) + i)->BackgroundImage.Width  = CHESS_IMAGE_WIDTH;
    ((*ChessBlock) + i)->BackgroundImage.Height = CHESS_IMAGE_HEIGHT;
    if ((((*ChessBlock) + i)->BackgroundImage.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool (((*ChessBlock) + i)->BackgroundImage.Width * ((*ChessBlock) + i)->BackgroundImage.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL))) == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    Status = (*pGraph)->Blt (
               *pGraph,
               ((*ChessBlock) + i)->BackgroundImage.BltBuffer,
               EfiBltVideoToBltBuffer,
               ((*ChessBlock) + i)->Position.x - (CHESS_IMAGE_WIDTH / 2),
               ((*ChessBlock) + i)->Position.y - (CHESS_IMAGE_HEIGHT / 2),
               0,
               0,
               ((*ChessBlock) + i)->BackgroundImage.Width,
               ((*ChessBlock) + i)->BackgroundImage.Height,
               0
               );
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }
  }

  return SOLAR_SUCCESS;
}

SOLAR_STATUS
SOLAR_API
EndGame (
  EFI_GRAPHICS_OUTPUT_PROTOCOL **pGraph,
  SOLAR_IMAGE_INPUT            *Image,
  CHESS_BLOCK_INFO             **ChessBlock
  )
{
  EFI_STATUS                   Status = SOLAR_SUCCESS;
  EFI_INPUT_KEY                KeyCode;

  EfiSetMem (Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);  
  FillScreen (*pGraph, SOLAR_BLACK);
  PrintString (*pGraph, Image, 285, 290, SOLAR_WHITE, L"Thank for your play");
  PrintString (*pGraph, Image, 410, 290, SOLAR_GREEN, L"OBSTRUCT-CHESS.");
  Status = (*pGraph)->Blt (
           *pGraph,
           Image->BltBuffer,
           EfiBltBufferToVideo,
           0,
           0,
           0,
           0,
           Image->Width,
           Image->Height,
           0
           );
  getch (&KeyCode);

  // Free memories.
  if (Image->BltBuffer) {
    gBS->FreePool (Image->BltBuffer);
  }
  for (UINT16 i = 0; i < ChessBlockAmount; i ++) {
    if (((*ChessBlock) + i)->BackgroundImage.BltBuffer) {
      gBS->FreePool (((*ChessBlock) + i)->BackgroundImage.BltBuffer);
    }
  }
  if (*ChessBlock) {
    gBS->FreePool (*ChessBlock);
  }

  return Status;
}

/////////////////////////////////////////////////////////////////////////////
//
//  ObstructChess () - The entry point for this application program.
//
//  ARGUMENTS:
//    ImageHandle           - EFI_HANDLE: The firmware allocated handle for 
//                            the EFI Driver image.
//    SystemTable           - EFI_SYSTEM_TABLE: A pointer to the EFI system table  
//
//  RETURNS:
//    EFI_SUCCESS           - The program is loaded success.
//
/////////////////////////////////////////////////////////////////////////////
extern "C"
EFI_STATUS
EFIAPI
ObstructChess (
  IN EFI_HANDLE                 ImageHandle,
  IN EFI_SYSTEM_TABLE           *SystemTable
  )
{
  EFI_STATUS                    Status = EFI_SUCCESS;
  EFI_SIMPLE_TEXT_OUT_PROTOCOL  *COut;
  UINT16                        HorizontalResolution;
  UINT16                        VerticalResolution;
  EFI_GRAPHICS_OUTPUT_PROTOCOL  *pGraph;
  SOLAR_IMAGE_INPUT             DesktopImage;
  UINT32                        ModeNum;
  CHESS_BLOCK_INFO              *ChessBlock;
  BOOLEAN                       isSinglePlayer;
  //EFI_INPUT_KEY                 KeyCode;

#ifdef EFI_NT_EMULATOR
  EFI_CONSOLE_CONTROL_PROTOCOL  *pConsole = (EFI_CONSOLE_CONTROL_PROTOCOL *) NULL;
#endif  // EFI_NT_EMULATOR
  //OUT POINTER_STATE            PointerState;

  // Nnever be detected by compiler that variable is used or not.
  ALWAYS_USED (Status)

  // Initialize "EfiDriverLib.Lib".
  EfiInitializeDriverLib (ImageHandle, SystemTable);

  // Get the console service table entry point.
  COut = gST->ConOut;
  
  // Check the Solar C++ library is supported.
  if (InitiateSolarCppLib (ImageHandle, SystemTable) != SOLAR_SUCCESS) {
    COut->OutputString (COut, L"\n>> SolarCppLib isn not ready.");
    return EFI_UNSUPPORTED;
  }

  // Check the Solar console library is supported.
  if (InitiateSolarConsoleLib () != SOLAR_SUCCESS) {
    COut->OutputString (COut, L"\n>> SolarConsleLib is not ready.");
    return EFI_UNSUPPORTED;
  }

#ifdef EFI_NT_EMULATOR
  // Locate Console Control Protocol.
  Status = gBS->LocateProtocol (
                &gEfiConsoleControlProtocolGuid,
                NULL,
                (void **) &pConsole
                );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  // Set console control mode for graphics using.
  pConsole->SetMode(pConsole, EfiConsoleControlScreenGraphics);
#endif  // EFI_NT_EMULATOR

  // Initiate Video.
  HorizontalResolution = HORIZONTAL_RESOLUTION;
  VerticalResolution   = VERTICAL_RESOLUTION;

  if (InitiateGraphicsMode (&pGraph, HorizontalResolution, VerticalResolution, &ModeNum) != SOLAR_SUCCESS) {
    COut->OutputString (COut, L"\n>> Initiate Graphics Mode UNSUCCESS!");
    return EFI_UNSUPPORTED;
  }

  // Image status setting.
  DesktopImage.BltBuffer       = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) NULL;
  DesktopImage.Height          = VerticalResolution;
  DesktopImage.Width           = HorizontalResolution;

  ChessBlock = (CHESS_BLOCK_INFO *) NULL;
  
  // Initiate Game
  Status = (EFI_STATUS) InitiateGame (&pGraph, &DesktopImage, &ChessBlock, &isSinglePlayer);
  if (EFI_ERROR (Status)) {
    goto ProgramDone;
  }

  // Start & Run Game
  Status = (EFI_STATUS) Round (pGraph, DesktopImage, ChessBlock, isSinglePlayer);
  if (EFI_ERROR (Status)) {
    goto ProgramDone;
  }
  
  goto ProgramDone;                     // Jump for exit program.
////////////////////////////////////////////////////////////////////////////
//                                                                        //
// Program ending, please release the memory and others exit code below ! //
//                                                                        //
////////////////////////////////////////////////////////////////////////////
ProgramDone:                            // Exit program.

  EndGame (&pGraph, &DesktopImage, &ChessBlock);
#ifdef EFI_NT_EMULATOR
  pConsole->SetMode(pConsole, EfiConsoleControlScreenText);
#endif  // EFI_NT_EMULATOR
  setAttr (7, 0);                       // Setup to normal attribute.
  cls;
  showCursor;                           // Show cursor.

  
#ifdef __DEBUG__
  //_DebugPrintf (0, 24, 15, 0, L"[DEBUG]:EFI_STATUS = %x ", Status);
#endif  // _DEBUG_

#ifdef __DEBUG__
  //_DebugWaitKey ();
#endif  // _DEBUG_
  
  return Status;
}