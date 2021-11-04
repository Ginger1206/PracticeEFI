/*++

Copyright (c) 2008 - 2009, Solar Studio

Module Name:

  DrawGame.c

Abstract:

  A Draw Game module for EFI shell.

--*/

extern "C" {
  #include "Efi.h"
  #include "Solar.h"
  #include "EfiDriverLib.h"
  #include "SolarConsoleLib.h"
  #include "SolarGraphicsLib.h"
  #include EFI_PROTOCOL_DEFINITION (ConsoleControl)
}
#include "SolarCppLib.h"
#include "ImageBlock.h"
#include "DrawGame.h"

#ifdef __DEBUG__
#include "../_DevelopAddition/_Debug.h"
#endif

extern "C" {
	EFI_APPLICATION_ENTRY_POINT(DrawGame);
}

//extern EFI_GRAPHICS_OUTPUT_BLT_PIXEL TransparentColor;
EFI_GRAPHICS_OUTPUT_BLT_PIXEL TransparentColor; //型態問題D幫解，先把extern拿掉試試
//extern UINT16                        ChessBlockEndColNum[CHESS_BOARD_CHECK_BLOCK_ROW_MAX_NUM];
//UINT16                        ChessBlockEndColNum[CHESS_BOARD_CHECK_BLOCK_ROW_MAX_NUM];
//UINT16                        ChessBlockEndColNum[21];
UINT16						  ChessBlockAmount;
//UINT16						  ColorBlockAmount;

SOLAR_STATUS
SOLAR_API
InitiateGraphicsMode(
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
	if (InitiateSolarGraphicsLib(pGraph) != SOLAR_SUCCESS) {
		COut->OutputString(COut, L"\n>> SolarGraphicsLib is not ready.");
		return SOLAR_UNSUCCESS;
	}

	QueryRequestMode(*pGraph, HorizontalResolution, VerticalResolution, ModeNum);

	FillScreen(*pGraph, SOLAR_BLACK);

	return SOLAR_SUCCESS;
}

SOLAR_STATUS
SOLAR_API
GraphicsSimpleDemo(
	EFI_GRAPHICS_OUTPUT_PROTOCOL  **pGraph,
	SOLAR_IMAGE_INPUT			  *Image
	//CHESS_BLOCK_INFO              **ChessBlock //到時改colorblock 0416
)
{
	EFI_STATUS    		Status;
	EFI_INPUT_KEY 		KeyCode;
	INT16				CoordinateX =150;
	INT16				CoordinateY =50;

	//BLOCK_COORDINATE    *position;
	//position->x =0;
	//position->y =0;
	//UINTN         BmpWidth;
	//UINTN         BmpHeight;

	//Drawline Demo
	// Allocate a screen size memory for buffer.
	if ((Image->BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)EfiLibAllocatePool(Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL))) == NULL) {
		return EFI_OUT_OF_RESOURCES;
	}
	//EFI_GRAPHICS_OUTPUT_BLT_PIXEL *bb =(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)Image->BltBuffer; //K0412
	//Image->BltBuffer = bb; //K0412

	EfiSetMem(Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);
	FillScreen(*pGraph, SOLAR_BLACK);
	PrintString(*pGraph, Image, 150, 290, SOLAR_WHITE, L"The Graphics Demo Solar System. Width = %d, Height = %d", Image->Width, Image->Height);
	Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
	//Status = (*pGraph)->Blt(*pGraph, bb, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0); //K0412
	getch(&KeyCode);
	
	// Fill the black color into buffer. Line Demo => 25*25
	EfiSetMem(Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);
	//EfiSetMem(Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_YELLOW); //G//try
	//getch(&KeyCode);//try
	FillScreen(*pGraph, SOLAR_BLACK);
	//FillScreen(*pGraph, SOLAR_YELLOW); //G SOLAR_YELLOW//try
	//getch(&KeyCode); //try
	//Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);//try
	//DrawHorizontalLine(*pGraph, Image, 150, 50 , 500, SOLAR_WHITE, FullLine);//try
	//getch(&KeyCode);//try
	//Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);//try
	//getch(&KeyCode);//try
	
	for (UINT16 i = 0; i < 26; i++) {
		DrawHorizontalLine(*pGraph, Image, 150, 50 + i * 20, 500, SOLAR_WHITE, FullLine); // DrawHorizontalLine (*pGraph, Image, 355, 50 + i * 10, 90, SOLAR_WHITE, (UINT8) i)
		DrawVerticalLine(*pGraph, Image, 150 + i * 20, 50, 500, SOLAR_WHITE, FullLine); // DrawVerticalLine (*pGraph, Image, 355 + i * 10, 50, 90, SOLAR_WHITE, (UINT8) i)
	}

	//DrawRectangle(*pGraph, Image, CoordinateX, CoordinateY, CoordinateX+20, CoordinateY+20, SOLAR_WHITE, 0, Fill, SOLAR_PALEGREEN, 3); //try
	DrawRectangle(*pGraph, Image, CoordinateX, CoordinateY, CoordinateX+20, CoordinateY+20, SOLAR_RED, 0, Fill, SOLAR_TRANSPARENT, 3); //try
	//DrawRectangle(*pGraph, Image, CoordinateX, CoordinateY, CoordinateX+20, CoordinateY+20, SOLAR_WHITE, 0, Fill, SOLAR_REVCYAN, 3);  //互補應該要寫函式255自己減 白?

	Status = (*pGraph)->Blt(
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

	//Listen event
	UINT16 boolean =1;
	//DrawRectangle(*pGraph, Image, 150, 50, 170, 70, SOLAR_CYAN, 0, Fill, SOLAR_PALEGREEN, 3);
	while(boolean){
		//position;
		
		getch(&KeyCode);
		
		//if (CoordinateX>=630 && CoordinateX<=150) break;
			
		//PrintString(*pGraph, Image, 150, 290, SOLAR_WHITE, L"Hi event occur , KeyCode = %x",KeyCode.ScanCode );
		switch(KeyCode.ScanCode){
			//DrawRectangle(*pGraph, Image, CoordinateX, CoordinateY, CoordinateX+20, CoordinateY+20, SOLAR_WHITE, 0, Fill, SOLAR_TRANSPARENT, 3);
			case SCAN_RIGHT:
				//condition for boundary
				//if (CoordinateX>=630) break;
				//condition for boundary
				if (CoordinateX>=630) {
					DrawRectangle(*pGraph, Image, CoordinateX, CoordinateY, CoordinateX+20, CoordinateY+20, SOLAR_WHITE, 0, Fill, SOLAR_BLACK, 3);  //try
					Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
					CoordinateX=150;
					DrawRectangle(*pGraph, Image, CoordinateX, CoordinateY, CoordinateX+20, CoordinateY+20, SOLAR_RED, 0, Fill, SOLAR_TRANSPARENT, 3);  //try
					Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
					break;
					}
					
				else{
					//reset first cursor
					DrawRectangle(*pGraph, Image, CoordinateX, CoordinateY, CoordinateX+20, CoordinateY+20, SOLAR_WHITE, 0, Fill, SOLAR_TRANSPARENT, 3);  //try
					Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
					//buttom action
					CoordinateX+=20;
					DrawRectangle(*pGraph, Image,CoordinateX, CoordinateY, CoordinateX+20, CoordinateY+20, SOLAR_RED, 0, Fill, SOLAR_TRANSPARENT, 3);
					Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
					break;
				}
				

				/*
				//condition for boundary
				if (CoordinateX>=630) {CoordinateX=150;}//if (CoordinateX>=630) {CoordinateX=150; CoordinateX-=20;}
				//reset first cursor
				DrawRectangle(*pGraph, Image, CoordinateX, CoordinateY, CoordinateX+20, CoordinateY+20, SOLAR_WHITE, 0, Fill, SOLAR_TRANSPARENT, 3);  //try
				Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
				//buttom action
				CoordinateX+=20;
				//DrawRectangle(*pGraph, Image,CoordinateX, CoordinateY, CoordinateX+20, CoordinateY+20, SOLAR_CYAN, 0, Fill, SOLAR_PALEGREEN, 3); //try
				DrawRectangle(*pGraph, Image,CoordinateX, CoordinateY, CoordinateX+20, CoordinateY+20, SOLAR_RED, 0, Fill, SOLAR_TRANSPARENT, 3);
				Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
				break;

				*/

			case SCAN_LEFT:
				//condition for boundary
				if (CoordinateX<=150) {
					DrawRectangle(*pGraph, Image, CoordinateX, CoordinateY, CoordinateX+20, CoordinateY+20, SOLAR_WHITE, 0, Fill, SOLAR_BLACK, 3);  //try
					Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
					CoordinateX=630;
					DrawRectangle(*pGraph, Image, CoordinateX, CoordinateY, CoordinateX+20, CoordinateY+20, SOLAR_RED, 0, Fill, SOLAR_TRANSPARENT, 3);  //try
					Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
					break;
					}
					
				else{
					//reset first cursor
					DrawRectangle(*pGraph, Image, CoordinateX, CoordinateY, CoordinateX+20, CoordinateY+20, SOLAR_WHITE, 0, Fill, SOLAR_TRANSPARENT, 3);  //try
					Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
					//buttom action
					CoordinateX-=20;
					DrawRectangle(*pGraph, Image,CoordinateX, CoordinateY, CoordinateX+20, CoordinateY+20, SOLAR_RED, 0, Fill, SOLAR_TRANSPARENT, 3);
					Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
					break;
				}


			case SCAN_DOWN:
				//condition for boundary
				if (CoordinateY>=530) {
					DrawRectangle(*pGraph, Image, CoordinateX, CoordinateY, CoordinateX+20, CoordinateY+20, SOLAR_WHITE, 0, Fill, SOLAR_BLACK, 3);  //try
					Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
					CoordinateY=50;
					DrawRectangle(*pGraph, Image, CoordinateX, CoordinateY, CoordinateX+20, CoordinateY+20, SOLAR_RED, 0, Fill, SOLAR_TRANSPARENT, 3);  //try
					Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
					break;
					}
					
				else{
					//reset first cursor
					DrawRectangle(*pGraph, Image, CoordinateX, CoordinateY, CoordinateX+20, CoordinateY+20, SOLAR_WHITE, 0, Fill, SOLAR_TRANSPARENT, 3);  //try
					Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
					//buttom action
					CoordinateY+=20;
					DrawRectangle(*pGraph, Image,CoordinateX, CoordinateY, CoordinateX+20, CoordinateY+20, SOLAR_RED, 0, Fill, SOLAR_TRANSPARENT, 3);
					Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
					break;
				}

			
			case SCAN_UP:
				//condition for boundary
				if (CoordinateY<=50) {
					DrawRectangle(*pGraph, Image, CoordinateX, CoordinateY, CoordinateX+20, CoordinateY+20, SOLAR_WHITE, 0, Fill, SOLAR_BLACK, 3);  //try
					Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
					CoordinateY=530;
					DrawRectangle(*pGraph, Image, CoordinateX, CoordinateY, CoordinateX+20, CoordinateY+20, SOLAR_RED, 0, Fill, SOLAR_TRANSPARENT, 3);  //try
					Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
					break;
					}
					
				else{
					//reset first cursor
					DrawRectangle(*pGraph, Image, CoordinateX, CoordinateY, CoordinateX+20, CoordinateY+20, SOLAR_WHITE, 0, Fill, SOLAR_TRANSPARENT, 3);  //try
					Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
					//buttom action
					CoordinateY-=20;
					DrawRectangle(*pGraph, Image,CoordinateX, CoordinateY, CoordinateX+20, CoordinateY+20, SOLAR_RED, 0, Fill, SOLAR_TRANSPARENT, 3);
					Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
					break;
				}

			case SCAN_ESC: //可以到時設一個詢問結束框 搭配ProgramDone的KeyCode一起完整結束
				boolean = 0;
				break;
		}
		//gBS->FreePool(Image->BltBuffer);
	
	}
/*
	getch(&KeyCode);

	// Free memories.
  	if (Image->BltBuffer) {
    	gBS->FreePool (Image->BltBuffer);
  	}
*/
	/*
	//BMP Demo
	EfiSetMem(Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);
	FillScreen(*pGraph, SOLAR_BLACK);
	BmpFileToSolarImage(*pGraph, L"111.bmp", Image, &BmpWidth, &BmpHeight);
	DrawRectangle(*pGraph, Image, 261, 118, 294, 148, SOLAR_PALEGREEN, 0, Fill, SOLAR_PALEGREEN, 3); // (x1,y1,x2,y2)=(261,118,294,148)
	DrawRectangle(*pGraph, Image, 360, 245, 393, 275, SOLAR_WHITE, 0, Fill, SOLAR_WHITE, 3); //(x1,y1,x2,y2)=(360,245,393,275)
	Status = (*pGraph)->Blt(
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

	getch(&KeyCode); //�D�{����ProgramDone�]���@��KeyCode�|�M�o�ӭ���(�Y���禡�񵲧�����  210121G
	*/
	
	//getch(&KeyCode);
	/*
	//Test color block
	EfiSetMem(Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);
	FillScreen(*pGraph, SOLAR_BLACK);
	DrawRectangle(*pGraph, Image, 370, 250, 429, 349, SOLAR_WHITE, 0, UL_ObliqueLine, SOLAR_WHITE, 3);
	Status = (*pGraph)->Blt(
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
	//getch(&KeyCode);
	*/
/*
	//Try for block init
	EfiSetMem(Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);
	FillScreen(*pGraph, SOLAR_BLACK);
	for (UINT16 i = 0; i < 26; i++) {
		DrawHorizontalLine(*pGraph, Image, 150, 50 + i * 20, 500, SOLAR_WHITE, FullLine); // DrawHorizontalLine (*pGraph, Image, 355, 50 + i * 10, 90, SOLAR_WHITE, (UINT8) i)
		DrawVerticalLine(*pGraph, Image, 150 + i * 20, 50, 500, SOLAR_WHITE, FullLine); // DrawVerticalLine (*pGraph, Image, 355 + i * 10, 50, 90, SOLAR_WHITE, (UINT8) i)
	}
	DrawRectangle(*pGraph, Image, 150, 50, 170, 70, SOLAR_CYAN, 0, Fill, SOLAR_PALEGREEN, 3); //try

	Status = (*pGraph)->Blt(
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
	//Block start 
	{
		UINT16		StartX;
		UINT16		StartY;
		UINT16		EndX;
		UINT16 x		= StartX;
		UINT16 y		= StartY;
		UINT16 i		= 0;
		UINT16 RowCount = 0;
		UINT16 ColCount = 0;
		do{
			((*ChessBlock) + i )->Status = Empty;
			((*ChessBlock) + i )->Position.x = x;
			((*ChessBlock) + i )->Position.y = y;
			((*ChessBlock) + i )->Col = ColCount;
			((*ChessBlock) + i )->Row = RowCount;
		}

	}
*/




	//gBS->FreePool(Image->BltBuffer);
	

	return SOLAR_SUCCESS;
}




SOLAR_STATUS
SOLAR_API
LocationActive (
  EFI_GRAPHICS_OUTPUT_PROTOCOL *pGraph,
  SOLAR_IMAGE_INPUT            DesktopImage,
  SOLAR_IMAGE_INPUT            LocationImage,
  SOLAR_IMAGE_INPUT            LocationBackgroundImage,
  CHESS_BLOCK_INFO             *ChessBlock,
  //COLOR_BLOCK_INFO			   *ColorBlock,
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
  CHESS_BLOCK_INFO  *ChessBlock,
  //COLOR_BLOCK_INFO  *ColorBlock,
  UINT16           Sn,
  UINT16           ScanCode
  )
{
  switch (ScanCode) {

    case SCAN_RIGHT:
      if ((ChessBlock + Sn)->Right == CHESS_INVALID_NEIGHBOR) {
        //return Sn - ChessBlockEndColNum[(ChessBlock + Sn)->Row] + 1;
		return Sn - (ChessBlock + Sn)->Row;
      } else {
        return (ChessBlock + Sn)->Right;
      }
      break;
  }

  return 0;
}

//0331 for chess put 此段可能要改 ChessBlockStatus
SOLAR_STATUS
SOLAR_API
BlockActive (
  EFI_GRAPHICS_OUTPUT_PROTOCOL *pGraph,
  SOLAR_IMAGE_INPUT            DesktopImage,
  SOLAR_IMAGE_INPUT            WhiteChessImage,
  SOLAR_IMAGE_INPUT            BlackChessImage,
  CHESS_BLOCK_INFO             *ChessBlock,
  //COLOR_BLOCK_INFO			   *ColorBlock,
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

//////////
SOLAR_STATUS
SOLAR_API
Cursor(
	EFI_GRAPHICS_OUTPUT_PROTOCOL 	*pGraph,
	SOLAR_IMAGE_INPUT				DesktopImage,
	//COLOR_BLOCK_INFO				*Colorblock
	CHESS_BLOCK_INFO 				*ChessBlock
)
{
	SOLAR_STATUS                 Status;
	EFI_INPUT_KEY                KeyCode;
	UINT16                       NewBlockPointSn;
	UINT16                       OldBlockPointSn;
	UINTN                        BmpWidth;
    UINTN                        BmpHeight;
	NewBlockPointSn              = 0;
	OldBlockPointSn              = 0;
	

	SOLAR_IMAGE_INPUT            LocationBackgroundImage;
	SOLAR_IMAGE_INPUT            WhiteLocationImage;
	SOLAR_IMAGE_INPUT            WhiteChessImage;
	SOLAR_IMAGE_INPUT            BlackChessImage;

	LocationBackgroundImage.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) NULL;
	LocationBackgroundImage.Width     = CHESS_IMAGE_WIDTH;
	LocationBackgroundImage.Height    = CHESS_IMAGE_HEIGHT;

	WhiteLocationImage.BltBuffer      = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) NULL;
  	WhiteLocationImage.Width          = CHESS_IMAGE_WIDTH;
  	WhiteLocationImage.Height         = CHESS_IMAGE_HEIGHT;

	WhiteChessImage.BltBuffer         = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) NULL;
    WhiteChessImage.Width             = CHESS_IMAGE_WIDTH;
    WhiteChessImage.Height            = CHESS_IMAGE_HEIGHT;

	BlackChessImage.BltBuffer         = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) NULL;
    BlackChessImage.Width             = CHESS_IMAGE_WIDTH;
    BlackChessImage.Height            = CHESS_IMAGE_HEIGHT;

	getch (&KeyCode); //???


	if ((LocationBackgroundImage.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool (LocationBackgroundImage.Width * LocationBackgroundImage.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL))) == NULL) {
    	Status = EFI_OUT_OF_RESOURCES;
    	//goto RoundDone;
  	}

	if ((WhiteLocationImage.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool (WhiteLocationImage.Width * WhiteLocationImage.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL))) == NULL) {
    	Status = EFI_OUT_OF_RESOURCES;
    	//goto RoundDone;
    }

	if ((WhiteChessImage.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool (WhiteChessImage.Width * WhiteChessImage.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL))) == NULL) {
    	return EFI_OUT_OF_RESOURCES;
    }

	if ((BlackChessImage.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool (BlackChessImage.Width * BlackChessImage.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL))) == NULL) {
    	return EFI_OUT_OF_RESOURCES;
    }


    BmpFileToSolarImage (pGraph, WHITE_LOCATION_IMAGE_FILENAME,   &WhiteLocationImage,          &BmpWidth, &BmpHeight); //待確認
	BmpFileToSolarImage (pGraph, WHITE_CHESS_IMAGE_FILENAME,      &WhiteChessImage,             &BmpWidth, &BmpHeight);//同上
	BmpFileToSolarImage (pGraph, BLACK_CHESS_IMAGE_FILENAME,      &BlackChessImage,             &BmpWidth, &BmpHeight);

	PUT_WHITE_CHECK (ChessBlock[(ChessBlockAmount / 2)].TopRight);
    PUT_WHITE_CHECK (ChessBlock[(ChessBlockAmount / 2)].Right);
    PUT_WHITE_CHECK (ChessBlock[(ChessBlockAmount / 2)].LowerRight);


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
                   WhiteLocationImage,
                   LocationBackgroundImage,
                   ChessBlock,
                   NewBlockPointSn
                   );


	if (WhiteLocationImage.BltBuffer) {
    	gBS->FreePool (WhiteLocationImage.BltBuffer);
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



extern "C"
EFI_STATUS
EFIAPI
DrawGame(
	IN EFI_HANDLE                            ImageHandle,
	IN EFI_SYSTEM_TABLE                      *SystemTable
)
{
	EFI_STATUS                    Status = EFI_SUCCESS;
	EFI_SIMPLE_TEXT_OUT_PROTOCOL* COut; //��ŧiD
	//EFI_SIMPLE_TEXT_OUT_PROTOCOL  *COut, *cTry;
	//EFI_INPUT_KEY                 KeyCode;
	//UINT8						  i=2; //
	UINT16						  HorizontalResolution;
	UINT16                        VerticalResolution;
	EFI_GRAPHICS_OUTPUT_PROTOCOL  *pGraph;
	SOLAR_IMAGE_INPUT             DesktopImage;
	UINT32                        ModeNum;
	CHESS_BLOCK_INFO              *ChessBlock;
	//COLOR_BLOCK_INFO			   *ColorBlock,
	
	
	#ifdef EFI_NT_EMULATOR
		EFI_CONSOLE_CONTROL_PROTOCOL* pConsole = (EFI_CONSOLE_CONTROL_PROTOCOL*)NULL;
	#endif  // EFI_NT_EMULATOR
	
	// Never be detected by compiler that variable is used or not.
	ALWAYS_USED(Status)

	EfiInitializeDriverLib(ImageHandle, SystemTable);


	//
	// Console initial.
	//
	COut = gST->ConOut;
	//cTry = gST->ConOut;


	// Check the Solar C++ library is supported.
	if (InitiateSolarCppLib(ImageHandle, SystemTable) != SOLAR_SUCCESS) { 
		COut->OutputString(COut, L"\n>> SolarCppLib is not ready.");
		return EFI_UNSUPPORTED;
	}

	// Check the Solar console library is supported.
	if (InitiateSolarConsoleLib() != SOLAR_SUCCESS) {
		COut->OutputString(COut, L"\n>> SolarConsleLib is not ready.");
		return EFI_UNSUPPORTED;
	}
	
	#ifdef EFI_NT_EMULATOR
		// Locate Console Control Protocol.
		Status = gBS->LocateProtocol(
			&gEfiConsoleControlProtocolGuid,
			NULL,
			(void**)&pConsole
		);
		if (EFI_ERROR(Status)) {
			return Status;
		}
		// Set console control mode for graphics using.
		pConsole->SetMode(pConsole, EfiConsoleControlScreenGraphics);
		//pConsole->SetMode(pConsole, EfiConsoleControlScreenText);//�� �ۥ[
	#endif  // EFI_NT_EMULATOR
	/// 
	
	// Initiate Video.
	HorizontalResolution = HORIZONTAL_RESOLUTION;
	VerticalResolution = VERTICAL_RESOLUTION;

	if (InitiateGraphicsMode(&pGraph, HorizontalResolution, VerticalResolution, &ModeNum) != SOLAR_SUCCESS) {
		COut->OutputString(COut, L"\n>> Initiate Graphics Mode UNSUCCESS!");
		return EFI_UNSUPPORTED;
	}

	DesktopImage.Height = VerticalResolution;
	DesktopImage.Width = HorizontalResolution;

	ChessBlock = (CHESS_BLOCK_INFO *) NULL;
	//ColorBlock = (COLOR_BLOCK_INFO *) NULL;

	
	//GraphicsSimpleDemo(&pGraph, &DesktopImage);
	Status=(EFI_STATUS)GraphicsSimpleDemo(&pGraph, &DesktopImage);

	//Cursor(pGraph, DesktopImage, ChessBlock); //Status=(EFI_STATUS)Cursor(pGraph, DesktopImage, ChessBlock); Cursor(pGraph, DesktopImage, ColorBlock);

	/*
	//cOut->OutputString(cOut, L"Hello EFI123 ! \t"); //add "\n" show enter&tab 
	switch (i) {
	case 0:                            // 10X10
		cTry->OutputString(cTry, L"this is 1");
		break;
	case 1:                            // 25x25
		cTry->OutputString(cTry, L"Here is 2");
		break;
	case 2:                            // 50*50
		cTry->OutputString(cTry, L"See ya");
		break;
	}
	*/
	

	goto ProgramDone;                     // Jump for exit program.
////////////////////////////////////////////////////////////////////////
//                                                                    //
// Program Ending, please release memory and others exit code below ! //
//                                                                    //
////////////////////////////////////////////////////////////////////////
ProgramDone:                            // Exit program.

  //getch (&KeyCode);                     // Wait any in-key.
#ifdef EFI_NT_EMULATOR
  pConsole->SetMode(pConsole, EfiConsoleControlScreenText);
#endif  // EFI_NT_EMULATOR
  setAttr (7, 0);                       // Setup to normal attribute.
  cls;
  showCursor;                           // Show cursor.

  
#ifdef __DEBUG__
  //_DebugPrintf (0, 24, 15, 0, L"[DEBUG]:END OF DEBUG !!", 0x00); //���ƻsSolarGraphicsLib�ܼƲz�ѫ�Цۦ���
#endif  // _DEBUG_

#ifdef __DEBUG__
  //_DebugWaitKey ();
#endif  // _DEBUG_

	return Status;
}
