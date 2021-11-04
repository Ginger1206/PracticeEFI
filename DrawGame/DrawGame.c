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




/*+++++
//Function draw boundry
	UINT16 x=CoordinateX;
	UINT16 x1=x+COLOR_BLOCK_EDGE;
	UINT16 y=CoordinateY;
	UINT16 y1=y+COLOR_BLOCK_EDGE;
	DrawRec(*Blt,x,y,x1,y2){
		DrawLine(*pGraph, &TryBackGround, x, y, x1, y,SOLAR_RED,FullLine);
		DrawLine(*pGraph, &TryBackGround, x1, y, x1, y1, SOLAR_RED,FullLine);
		DrawLine(*pGraph, &TryBackGround, x1, y1, x, y1, SOLAR_RED,FullLine);
		DrawLine(*pGraph, &TryBackGround, x, y1, x, y, SOLAR_RED,FullLine);
	}
	DrawRec(x,y,x1,y2);

	DrawRec(*pGraph,image,x,y,x+W,y+W){
		DrawLine(p,&i,   x,   y, x+W,   y,red,FL);
		DrawLine(p,&i, x+W,   y, x+W, y+W,red,FL);
		DrawLine(p,&i, x+W, y+W,   x, y+W,red,FL);
		DrawLine(p,&i,   x, y+W,   x,   y,red,FL);
	}


	DelBoundry(*pGraph,image,x,y,x+W,y+W){
		DrawLine(p,&i,)
	}
-----*/
SOLAR_STATUS
SOLAR_LIB
RecBoundary (
  IN     EFI_GRAPHICS_OUTPUT_PROTOCOL *pGraphOutput,
  IN OUT SOLAR_IMAGE_INPUT            Image,
  IN     UINT16                       X,
  IN     UINT16                       Y
  )
/*++

Routine Description:

  Draw a boundary line into an image region.

Arguments:

  pGraphOutput - EFI_GRAPHICS_OUTPUT_PROTOCOL
  Image        - SOLAR_IMAGE_INPUT
  X1           - The start X-coordinate of line.
  Y1           - The start Y-coordinate of line.

Returns:

  SOLAR_SUCCESS or SOLAR_UNSUCCESS.

--*/
{
	EFI_STATUS    		  Status;
	UINT16 x2=X+COLOR_BLOCK_EDGE;
	UINT16 y2=Y+COLOR_BLOCK_EDGE;

	//> d < u		                                                          //哪個方向保留那個其他三個畫為白
	DrawLine( pGraphOutput, &Image,   X,   Y,  x2,   Y, SOLAR_RED, FullLine); //上移
	DrawLine( pGraphOutput, &Image,  x2,   Y,  x2,  y2, SOLAR_RED, FullLine); //右移
	DrawLine( pGraphOutput, &Image,  x2,  y2,   X,  y2, SOLAR_RED, FullLine); //下移
	DrawLine( pGraphOutput, &Image,   X,  y2,   X,   Y, SOLAR_RED, FullLine); //左移

	Status=(pGraphOutput)->Blt(pGraphOutput,Image.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image.Width, Image.Height, 0);
	
	return SOLAR_SUCCESS;
}

//Del
SOLAR_STATUS
SOLAR_LIB
MovDelBdy(
  IN     EFI_GRAPHICS_OUTPUT_PROTOCOL *pGraphOutput,
  IN OUT SOLAR_IMAGE_INPUT            Image,
  IN     UINT16                       X,
  IN     UINT16                       Y,
  IN     UINT16                       ScanCode
  )
{
	EFI_STATUS    		  Status;
	UINT16 x2=X+COLOR_BLOCK_EDGE; //maybe要換成減的 直接減就可 還要多加最大邊界限制範圍
	UINT16 y2=Y+COLOR_BLOCK_EDGE;

	switch (ScanCode)
	{
	case SCAN_UP:
	//if(Y<=COLOR_BOARD_Y1) Y=COLOR_BOARD_Y1+24*COLOR_BLOCK_EDGE;
		//DrawLine( pGraphOutput, &Image,   X,   Y,  x2,   Y, SOLAR_WHITE, FullLine); //上移
		DrawLine( pGraphOutput, &Image,  x2,   Y,  x2,  y2, SOLAR_WHITE, FullLine); //右移
		DrawLine( pGraphOutput, &Image,  x2,  y2,   X,  y2, SOLAR_WHITE, FullLine); //下移
		DrawLine( pGraphOutput, &Image,   X,  y2,   X,   Y, SOLAR_WHITE, FullLine); //左移
		break;

	case SCAN_RIGHT:
		DrawLine( pGraphOutput, &Image,   X,   Y,  x2,   Y, SOLAR_WHITE, FullLine); //上移
		DrawLine( pGraphOutput, &Image,  x2,  y2,   X,  y2, SOLAR_WHITE, FullLine); //下移
		DrawLine( pGraphOutput, &Image,   X,  y2,   X,   Y, SOLAR_WHITE, FullLine); //左移
		break;
	
	case SCAN_DOWN:
		DrawLine( pGraphOutput, &Image,   X,   Y,  x2,   Y, SOLAR_WHITE, FullLine); //上移
		DrawLine( pGraphOutput, &Image,  x2,   Y,  x2,  y2, SOLAR_WHITE, FullLine); //右移
		DrawLine( pGraphOutput, &Image,   X,  y2,   X,   Y, SOLAR_WHITE, FullLine); //左移
		break;

	case SCAN_LEFT:
		DrawLine( pGraphOutput, &Image,   X,   Y,  x2,   Y, SOLAR_WHITE, FullLine); //上移
		DrawLine( pGraphOutput, &Image,  x2,   Y,  x2,  y2, SOLAR_WHITE, FullLine); //右移
		DrawLine( pGraphOutput, &Image,  x2,  y2,   X,  y2, SOLAR_WHITE, FullLine); //下移
		break;
		
	}
	Status=(pGraphOutput)->Blt(pGraphOutput,Image.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image.Width, Image.Height, 0);

	return SOLAR_SUCCESS;
}


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
	//COLOR_BLOCK_INFO			  **ColorBlock //**ColorBlock
)
{
	EFI_STATUS    		Status;
	EFI_INPUT_KEY 		KeyCode;
	INT16				CoordinateX = COLOR_BOARD_X1; 
	INT16				CoordinateY = COLOR_BOARD_Y1; 
	UINT16				X = CoordinateX, Y = CoordinateY;

	SOLAR_IMAGE_INPUT	DrawingBoard; 
	SOLAR_IMAGE_INPUT	SelectBar;

	SOLAR_IMAGE_INPUT	TryBackGround; 
	//SOLAR_IMAGE_INPUT	AllviewBoard;//for reset
	

	//COLOR_BLOCK_INFO	  *ColorBlock;
	//(*ColorBlock)->Position.x= COLOR_BOARD_X1;
	//(*ColorBlock)->Position.y= COLOR_BOARD_Y1; 

	DrawingBoard.BltBuffer=(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)NULL;
	DrawingBoard.Width=630;
	DrawingBoard.Height=550;

	SelectBar.BltBuffer=(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)NULL;
	SelectBar.Width=170;
	SelectBar.Height=600;

	TryBackGround.BltBuffer=(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)NULL; //
	TryBackGround.Width=630; //
	TryBackGround.Height=550; //
/*
	AllviewBoard.BltBuffer=(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)NULL;
	AllviewBoard.Width=630;
	AllviewBoard.Height=550;
*/
	if((DrawingBoard.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool(DrawingBoard.Width * DrawingBoard.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)))==NULL){
		return EFI_OUT_OF_RESOURCES;
	}

	if ((SelectBar.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool(SelectBar.Width * SelectBar.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)))==NULL){
		return EFI_OUT_OF_RESOURCES;
	}

	if ((TryBackGround.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool(TryBackGround.Width * TryBackGround.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)))==NULL){
		return EFI_OUT_OF_RESOURCES;
	} //
/*
	if((AllviewBoard.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool(AllviewBoard.Width * AllviewBoard.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)))==NULL){
		return EFI_OUT_OF_RESOURCES;
	}
*/
	//UINTN         BmpWidth;
	//UINTN         BmpHeight;

	//Drawline Demo
	// Allocate a screen size memory for buffer.
	if ((Image->BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)EfiLibAllocatePool(Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL))) == NULL) {
		return EFI_OUT_OF_RESOURCES;
	}

	EfiSetMem(Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);
	FillScreen(*pGraph, SOLAR_BLACK);
	PrintString(*pGraph, Image, 150, 290, SOLAR_WHITE, L"The Graphics Demo Solar System. Width = %d, Height = %d", Image->Width, Image->Height); 
	Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
	getch(&KeyCode);





	//Blt Try
	EfiSetMem(TryBackGround.BltBuffer, TryBackGround.Width * TryBackGround.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_GREEN);
	FillScreen(*pGraph, SOLAR_YELLOW);
	//EfiSetMem(ColorBlock.BltBuffer, ColorBlock.Height*ColorBlock.Width*sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL),0);
	//Status=(*pGraph)->Blt(*pGraph,ColorBlock.BltBuffer, EfiBltBufferToVideo, 0, 0,COLOR_BOARD_X1, COLOR_BOARD_Y1, ColorBlock.Width, ColorBlock.Height, 0); //COLOR_BOARD_X1, COLOR_BOARD_Y1,
	Status=(*pGraph)->Blt(*pGraph, TryBackGround.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, TryBackGround.Width, TryBackGround.Height, 0); //先畫背景
	for (UINT16 i = 0; i < 26; i++) {
		DrawHorizontalLine(*pGraph, &TryBackGround, CoordinateX, CoordinateY + i * COLOR_BLOCK_EDGE, COLOR_BOARD_WIDTH, SOLAR_WHITE, FullLine); 
		DrawVerticalLine(*pGraph, &TryBackGround, CoordinateX + i * COLOR_BLOCK_EDGE, CoordinateY, COLOR_BOARD_HEIGH, SOLAR_WHITE, FullLine); 
	}
/*
	//> d < u
	DrawLine(*pGraph, &TryBackGround, CoordinateX, CoordinateY, CoordinateX+COLOR_BLOCK_EDGE, CoordinateY,SOLAR_RED,FullLine);
	DrawLine(*pGraph, &TryBackGround, CoordinateX+COLOR_BLOCK_EDGE, CoordinateY, CoordinateX+COLOR_BLOCK_EDGE, CoordinateY+COLOR_BLOCK_EDGE, SOLAR_RED,FullLine);
	DrawLine(*pGraph, &TryBackGround, CoordinateX+COLOR_BLOCK_EDGE, CoordinateY+COLOR_BLOCK_EDGE, CoordinateX, CoordinateY+COLOR_BLOCK_EDGE, SOLAR_RED,FullLine);
	DrawLine(*pGraph, &TryBackGround, CoordinateX, CoordinateY+COLOR_BLOCK_EDGE, CoordinateX, CoordinateY, SOLAR_RED,FullLine);
*/
	RecBoundary(*pGraph,TryBackGround,CoordinateX,CoordinateY);

	//getch(&KeyCode);

	UINT16 loop=1;
	while(loop){
		getch(&KeyCode);

		switch (KeyCode.ScanCode)
		{
		
		case SCAN_UP:
				if(Y<=CoordinateY){
					MovDelBdy(*pGraph,TryBackGround,X,Y,KeyCode.ScanCode);
					DrawLine(*pGraph, &TryBackGround,   X,   Y,  X+COLOR_BLOCK_EDGE,   Y, SOLAR_WHITE, FullLine); //上移
					Status=(*pGraph)->Blt(*pGraph,TryBackGround.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, TryBackGround.Width, TryBackGround.Height, 0);
					Y=CoordinateY+24*COLOR_BLOCK_EDGE;
					RecBoundary(*pGraph,TryBackGround,X,Y);
				}
				else{
					MovDelBdy(*pGraph,TryBackGround,X,Y,KeyCode.ScanCode);
					Y-=COLOR_BLOCK_EDGE;
					RecBoundary(*pGraph,TryBackGround,X,Y);
				}
				break;

		case SCAN_DOWN:
				if(Y>=CoordinateY+24*COLOR_BLOCK_EDGE){
					MovDelBdy(*pGraph,TryBackGround,X,Y,KeyCode.ScanCode);
					DrawLine( *pGraph, &TryBackGround,  X+COLOR_BLOCK_EDGE,  Y+COLOR_BLOCK_EDGE,   X,  Y+COLOR_BLOCK_EDGE, SOLAR_WHITE, FullLine); //下移
					Status=(*pGraph)->Blt(*pGraph,TryBackGround.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, TryBackGround.Width, TryBackGround.Height, 0);
					Y=CoordinateY;
					RecBoundary(*pGraph,TryBackGround,X,Y);
				}
				else{
					MovDelBdy(*pGraph,TryBackGround,X,Y,KeyCode.ScanCode);
					Y+=COLOR_BLOCK_EDGE;
					RecBoundary(*pGraph,TryBackGround,X,Y);
				}
				break;
		case SCAN_RIGHT:
				if (X >= CoordinateX+24*COLOR_BLOCK_EDGE){
					MovDelBdy(*pGraph,TryBackGround,X,Y,KeyCode.ScanCode);
					DrawLine(*pGraph, &TryBackGround,  X+COLOR_BLOCK_EDGE,   Y,  X+COLOR_BLOCK_EDGE,  Y+COLOR_BLOCK_EDGE, SOLAR_WHITE, FullLine); //右移
					Status=(*pGraph)->Blt(*pGraph,TryBackGround.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, TryBackGround.Width, TryBackGround.Height, 0);
					X =CoordinateX;
					RecBoundary(*pGraph,TryBackGround,X,Y);
				}
				else{ //正常右移
					MovDelBdy(*pGraph,TryBackGround,X,Y,KeyCode.ScanCode);
					X+=COLOR_BLOCK_EDGE;
					RecBoundary(*pGraph,TryBackGround,X,Y);
				}
				break;
		case SCAN_LEFT:
				if (X <= CoordinateX){
					MovDelBdy(*pGraph,TryBackGround,X,Y,KeyCode.ScanCode);
					DrawLine(*pGraph, &TryBackGround,   X,  Y+COLOR_BLOCK_EDGE,   X,   Y, SOLAR_WHITE, FullLine); //左移
					Status=(*pGraph)->Blt(*pGraph,TryBackGround.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, TryBackGround.Width, TryBackGround.Height, 0);
					X =CoordinateX+24*COLOR_BLOCK_EDGE;
					RecBoundary(*pGraph,TryBackGround,X,Y);
				}
				else{ 
					MovDelBdy(*pGraph,TryBackGround,X,Y,KeyCode.ScanCode);
					X-=COLOR_BLOCK_EDGE;
					RecBoundary(*pGraph,TryBackGround,X,Y);
				}
			break;
		
		case SCAN_ESC:
			loop=0;
			break;
		}

		switch (KeyCode.UnicodeChar)
		{
		case CHAR_SPACE  :
				DrawRectangle(*pGraph, &TryBackGround, X, Y, X+COLOR_BLOCK_EDGE, Y+COLOR_BLOCK_EDGE, SOLAR_RED, 0, Fill, SOLAR_PALEGREEN, 3);  //try
				Status = (*pGraph)->Blt(*pGraph, TryBackGround.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, TryBackGround.Width, TryBackGround.Height, 0); 
				break;
		case CHAR_R: //reset畫板
				X=COLOR_BOARD_X1;
				Y=COLOR_BOARD_Y1;
		        EfiSetMem(TryBackGround.BltBuffer, TryBackGround.Width * TryBackGround.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);
				for (UINT16 i = 0; i < 26; i++) {
						DrawHorizontalLine(*pGraph, &TryBackGround, CoordinateX, CoordinateY + i * COLOR_BLOCK_EDGE, COLOR_BOARD_WIDTH, SOLAR_WHITE, FullLine); 
						DrawVerticalLine(*pGraph, &TryBackGround, CoordinateX + i * COLOR_BLOCK_EDGE, CoordinateY, COLOR_BOARD_HEIGH, SOLAR_WHITE, FullLine); 
					}
				RecBoundary(*pGraph,TryBackGround,X,Y);
				Status=(*pGraph)->Blt(*pGraph, TryBackGround.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, TryBackGround.Width, TryBackGround.Height, 0);
				break;
		case CHAR_F: //倒色
				break;
		}

		//TransparentBltBufferToVideo(*pGraph,&AllviewBoard,&TryBackGround,CoordinateX,CoordinateY,TransparentColor);
		//EfiSetMem(AllviewBoard.BltBuffer, AllviewBoard.Width * AllviewBoard.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_GREEN);
		//Status=(*pGraph)->Blt(*pGraph, AllviewBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, AllviewBoard.Width, AllviewBoard.Height, 0); //先畫背景
				
	}
	/*
    //d > u <
	DrawLine(*pGraph, &TryBackGround, CoordinateX, CoordinateY, CoordinateX, CoordinateY+COLOR_BLOCK_EDGE, SOLAR_RED,FullLine);
	DrawLine(*pGraph, &TryBackGround, CoordinateX, CoordinateY+COLOR_BLOCK_EDGE, CoordinateX+COLOR_BLOCK_EDGE, CoordinateY+COLOR_BLOCK_EDGE, SOLAR_RED,FullLine);
	DrawLine(*pGraph, &TryBackGround, CoordinateX+COLOR_BLOCK_EDGE, CoordinateY+COLOR_BLOCK_EDGE, CoordinateX+COLOR_BLOCK_EDGE, CoordinateY, SOLAR_RED,FullLine);
	DrawLine(*pGraph, &TryBackGround, CoordinateX+COLOR_BLOCK_EDGE, CoordinateY, CoordinateX, CoordinateY, SOLAR_RED,FullLine);
	*/

	//Status=(*pGraph)->Blt(*pGraph,TryBackGround.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, TryBackGround.Width, TryBackGround.Height, 0);
	//getch(&KeyCode);

	/*此作法記憶體會爆
	//SOLAR_IMAGE_INPUT   ColorBlock;

	ColorBlock.BltBuffer=(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)NULL;
	ColorBlock.Width=630;//COLOR_BLOCK_EDGE
	ColorBlock.Height=550; 


	if ((ColorBlock.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool( ColorBlock.Width * ColorBlock.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL) ))== NULL){
		return EFI_OUT_OF_RESOURCES;
	}


	Status=(*pGraph)->Blt(*pGraph, TryBackGround.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, TryBackGround.Width, TryBackGround.Height, 0); //先畫背景
	for(UINT16 j=1;j<(26*COLOR_BLOCK_EDGE);j+=COLOR_BLOCK_EDGE) //UINT16 j=0;j<(25*COLOR_BLOCK_EDGE);j+=COLOR_BLOCK_EDGE
	{
		for(UINT16 k=1;k<(26*COLOR_BLOCK_EDGE);k+=COLOR_BLOCK_EDGE)
		{
					
			ColorBlock[k*j].BltBuffer=(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)NULL;
			ColorBlock[k*j].Width=COLOR_BLOCK_EDGE;
			ColorBlock[k*j].Height=COLOR_BLOCK_EDGE;

			if ( (ColorBlock[k*j].BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool( ColorBlock[k*j].Width * ColorBlock[k*j].Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)))== NULL){
				return EFI_OUT_OF_RESOURCES;
			}
			
			//DrawRectangle(*pGraph, &ColorBlock[k*j], k-1, j-1, k+COLOR_BLOCK_EDGE-1, j+COLOR_BLOCK_EDGE-1, SOLAR_RED, 0, Fill, SOLAR_PALEGREEN, 3); //全部畫出來後可以不用-1  (0+k, 0+j, k+COLOR_BLOCK_EDGE, j+COLOR_BLOCK_EDGE,)

			//Status=(*pGraph)->Blt(*pGraph,ColorBlock[k*j].BltBuffer, EfiBltBufferToVideo, 0, 0, COLOR_BOARD_X1, COLOR_BOARD_Y1, ColorBlock[k*j].Width, ColorBlock[k*j].Height, 0); //畫小buffer
			
			DrawRectangle(*pGraph, &ColorBlock, 0+k, 0+j, k+COLOR_BLOCK_EDGE-1, j+COLOR_BLOCK_EDGE-1, SOLAR_RED, 0, Fill, SOLAR_PALEGREEN, 3); 
		}
	}
	//DrawRectangle(*pGraph, &ColorBlock, 0, 0, COLOR_BLOCK_EDGE-1, COLOR_BLOCK_EDGE-1, SOLAR_RED, 0, Fill, SOLAR_PALEGREEN, 3);
	Status=(*pGraph)->Blt(*pGraph,ColorBlock.BltBuffer, EfiBltBufferToVideo, 0, 0, COLOR_BOARD_X1, COLOR_BOARD_Y1, ColorBlock.Width, ColorBlock.Height, 0); //畫小buffer
	*/


	
	//Select function Bar(之後可以跟畫框一起Show出來先把Function做好)
	EfiSetMem(SelectBar.BltBuffer, SelectBar.Width * SelectBar.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), 0);
	FillScreen(*pGraph, SOLAR_YELLOW);
	PrintString(*pGraph, &SelectBar, 0, 30, SOLAR_WHITE, L" 'SPACE' : make boundary ");//show不出來 鑽太久惹 650 20是800600的位置 要以Select的大小畫
	//Status = (*pGraph)->Blt(*pGraph, SelectBar.BltBuffer, EfiBltBufferToVideo, 0, 0, 630, 0, SelectBar.Width, SelectBar.Height, 0);  //0, 0, 630, 0,
	//getch(&KeyCode);


	// Fill the black color into buffer. Line Demo => 25*25
	EfiSetMem(DrawingBoard.BltBuffer, DrawingBoard.Width * DrawingBoard.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_YELLOW);
	//EfiSetMem(SelectBar.BltBuffer, SelectBar.Width * SelectBar.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_YELLOW); //select Bar
	FillScreen(*pGraph, SOLAR_YELLOW);
	//PrintString(*pGraph, &DrawingBoard, 50, 50, SOLAR_WHITE, L"Width = %d, Height = %d",  DrawingBoard.Width, DrawingBoard.Height);
	Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); 
	Status = (*pGraph)->Blt(*pGraph, SelectBar.BltBuffer, EfiBltBufferToVideo, 0, 0, 630, 0, SelectBar.Width, SelectBar.Height, 0); //Select Bar
	//getch(&KeyCode);
	
	
	for (UINT16 i = 0; i < 26; i++) {
		DrawHorizontalLine(*pGraph, &DrawingBoard, CoordinateX, CoordinateY + i * COLOR_BLOCK_EDGE, COLOR_BOARD_WIDTH, SOLAR_WHITE, FullLine); 
		DrawVerticalLine(*pGraph, &DrawingBoard, CoordinateX + i * COLOR_BLOCK_EDGE, CoordinateY, COLOR_BOARD_HEIGH, SOLAR_WHITE, FullLine); 
	}

	DrawRectangle(*pGraph, &DrawingBoard, CoordinateX, CoordinateY, CoordinateX+COLOR_BLOCK_EDGE, CoordinateY+COLOR_BLOCK_EDGE, SOLAR_RED, 0, Fill, SOLAR_TRANSPARENT, 3); 

	//Back=DrawingBoard; //210422KG 記概念

	Status = (*pGraph)->Blt(
		*pGraph,
		DrawingBoard.BltBuffer,
		EfiBltBufferToVideo,
		0,
		0,
		0,
		0,
		DrawingBoard.Width,
		DrawingBoard.Height,
		0
	);

	

	//Listen event
	UINT16 boolean =1;
	//DrawRectangle(*pGraph, &DrawingBoard, 150, 50, 170, 70, SOLAR_CYAN, 0, Fill, SOLAR_PALEGREEN, 3);
	while(boolean){
		//position;
		getch(&KeyCode);
		
		//if (CoordinateX>=630 && CoordinateX<=150) break;
		switch(KeyCode.ScanCode){
			//DrawRectangle(*pGraph, &DrawingBoard, CoordinateX, CoordinateY, CoordinateX+20, CoordinateY+20, SOLAR_WHITE, 0, Fill, SOLAR_TRANSPARENT, 3);
			case SCAN_RIGHT:
				//condition for boundary
				//if (CoordinateX>=630) break;
				
				//condition for boundary 
				if (CoordinateX >= (COLOR_BOARD_X1 + 24*COLOR_BLOCK_EDGE)) { //CoordinateX >=530 [530=COLOR_BOARD_X1+(25-1)*COLOR_BLOCK_EDGE]
					DrawRectangle(*pGraph, &DrawingBoard, CoordinateX, CoordinateY, CoordinateX+COLOR_BLOCK_EDGE, CoordinateY+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, SOLAR_BLACK, 3);  //
					PrintString(*pGraph, &DrawingBoard, 5, 5, SOLAR_WHITE, L"X1= %d, Y1 = %d", CoordinateX, CoordinateY); //test for last coordination
					Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); 
					CoordinateX=COLOR_BOARD_X1;
					DrawRectangle(*pGraph, &DrawingBoard, CoordinateX, CoordinateY, CoordinateX+COLOR_BLOCK_EDGE, CoordinateY+COLOR_BLOCK_EDGE, SOLAR_RED, 0, Fill, SOLAR_TRANSPARENT, 3);  //try
					Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); 
					break;
					}
					
				else{
					//reset first cursor
					DrawRectangle(*pGraph, &DrawingBoard, CoordinateX, CoordinateY, CoordinateX+COLOR_BLOCK_EDGE, CoordinateY+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, SOLAR_TRANSPARENT, 3);  //try
					Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); 
					//buttom action
					CoordinateX+=COLOR_BLOCK_EDGE;
					DrawRectangle(*pGraph, &DrawingBoard,CoordinateX, CoordinateY, CoordinateX+COLOR_BLOCK_EDGE, CoordinateY+COLOR_BLOCK_EDGE, SOLAR_RED, 0, Fill, SOLAR_TRANSPARENT, 3);
					Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); 
					break;
				}
				
				/*
				//condition for boundary
				if (CoordinateX>=480) {CoordinateX=50;}//if (CoordinateX>=630) {CoordinateX=150; CoordinateX-=20;}
				//reset first cursor
				//DrawRectangle(*pGraph,  &DrawingBoard, CoordinateX, CoordinateY, CoordinateX+20, CoordinateY+20, SOLAR_WHITE, 0, Fill, SOLAR_TRANSPARENT, 3);  //try
				//Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0);
				//buttom action
				CoordinateX+=20;
				DrawRectangle(*pGraph,  &DrawingBoard,CoordinateX, CoordinateY, CoordinateX+20, CoordinateY+20, SOLAR_CYAN, 0, Fill, SOLAR_PALEGREEN, 3); //try
				//DrawRectangle(*pGraph,  &DrawingBoard,CoordinateX, CoordinateY, CoordinateX+20, CoordinateY+20, SOLAR_RED, 0, Fill, SOLAR_TRANSPARENT, 3);
				Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0);
				break;
				
				case NULL:
					DrawingBoard=&Back;
					Status=(*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0);
					break;
				*/
			case SCAN_LEFT:
				//condition for boundary
				if (CoordinateX<=COLOR_BOARD_X1) {
					DrawRectangle(*pGraph, &DrawingBoard,CoordinateX, CoordinateY, CoordinateX+COLOR_BLOCK_EDGE,CoordinateY+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, SOLAR_BLACK, 3);  //try
					Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); 
					CoordinateX = (COLOR_BOARD_X1 + 24*COLOR_BLOCK_EDGE); //CoordinateX=530;
					DrawRectangle(*pGraph, &DrawingBoard, CoordinateX, CoordinateY, CoordinateX+COLOR_BLOCK_EDGE, CoordinateY+COLOR_BLOCK_EDGE, SOLAR_RED, 0, Fill, SOLAR_TRANSPARENT, 3);  //try
					Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); 
					break;
					}
					
				else{
					//reset first cursor
					DrawRectangle(*pGraph, &DrawingBoard, CoordinateX, CoordinateY, CoordinateX+COLOR_BLOCK_EDGE, CoordinateY+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, SOLAR_TRANSPARENT, 3);  //try
					Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); 
					//buttom action
					CoordinateX-=COLOR_BLOCK_EDGE;
					DrawRectangle(*pGraph, &DrawingBoard,CoordinateX, CoordinateY, CoordinateX+COLOR_BLOCK_EDGE, CoordinateY+COLOR_BLOCK_EDGE, SOLAR_RED, 0, Fill, SOLAR_TRANSPARENT, 3);
					Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); 
					break;
				}


			case SCAN_DOWN:
				//condition for boundary
				if (CoordinateY >= (COLOR_BOARD_Y1 + 24*COLOR_BLOCK_EDGE)) {
					DrawRectangle(*pGraph, &DrawingBoard, CoordinateX, CoordinateY, CoordinateX+COLOR_BLOCK_EDGE, CoordinateY+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, SOLAR_BLACK, 3);  //try
					Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); 
					CoordinateY = COLOR_BOARD_Y1;
					DrawRectangle(*pGraph, &DrawingBoard, CoordinateX, CoordinateY, CoordinateX+COLOR_BLOCK_EDGE, CoordinateY+COLOR_BLOCK_EDGE, SOLAR_RED, 0, Fill, SOLAR_TRANSPARENT, 3);  //try
					Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); 
					break;
					}
					
				else{
					//reset first cursor
					DrawRectangle(*pGraph, &DrawingBoard, CoordinateX, CoordinateY, CoordinateX+COLOR_BLOCK_EDGE, CoordinateY+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, SOLAR_TRANSPARENT, 3);  //try
					Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); 
					//buttom action
					CoordinateY+=COLOR_BLOCK_EDGE;
					DrawRectangle(*pGraph, &DrawingBoard,CoordinateX, CoordinateY, CoordinateX+COLOR_BLOCK_EDGE, CoordinateY+COLOR_BLOCK_EDGE, SOLAR_RED, 0, Fill, SOLAR_TRANSPARENT, 3);
					Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); 
					break;
				}

			
			case SCAN_UP:
				//condition for boundary
				if (CoordinateY <= COLOR_BOARD_Y1) {
					DrawRectangle(*pGraph, &DrawingBoard, CoordinateX, CoordinateY, CoordinateX+COLOR_BLOCK_EDGE, CoordinateY+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, SOLAR_BLACK, 3);  //try
					Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); 
					CoordinateY = (COLOR_BOARD_Y1 + 24*COLOR_BLOCK_EDGE);
					DrawRectangle(*pGraph, &DrawingBoard, CoordinateX, CoordinateY, CoordinateX+COLOR_BLOCK_EDGE, CoordinateY+COLOR_BLOCK_EDGE, SOLAR_RED, 0, Fill, SOLAR_TRANSPARENT, 3);  //try
					Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); 
					break;
					}
					
				else{
					//reset first cursor
					DrawRectangle(*pGraph, &DrawingBoard, CoordinateX, CoordinateY, CoordinateX+COLOR_BLOCK_EDGE, CoordinateY+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, SOLAR_TRANSPARENT, 3);  //try
					Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); 
					//buttom action
					CoordinateY-=COLOR_BLOCK_EDGE;
					DrawRectangle(*pGraph, &DrawingBoard,CoordinateX, CoordinateY, CoordinateX+COLOR_BLOCK_EDGE, CoordinateY+COLOR_BLOCK_EDGE, SOLAR_RED, 0, Fill, SOLAR_TRANSPARENT, 3);
					Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); 
					break;
				}

			case SCAN_ESC: //可以到時設一個詢問結束框 搭配ProgramDone的KeyCode一起完整結束
				boolean = 0;
				break;
		}

		switch (KeyCode.UnicodeChar)
		{
		case CHAR_SPACE:
				DrawRectangle(*pGraph, &DrawingBoard, CoordinateX, CoordinateY, CoordinateX+COLOR_BLOCK_EDGE, CoordinateY+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, SOLAR_TRANSPARENT, 3);  //try
				Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); 
				DrawRectangle(*pGraph, &DrawingBoard, CoordinateX, CoordinateY, CoordinateX+COLOR_BLOCK_EDGE, CoordinateY+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, SOLAR_PALEGREEN, 3);
				Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); 
					break;
		}
	}
	
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
	
*/

	gBS->FreePool(Image->BltBuffer);
	gBS->FreePool(DrawingBoard.BltBuffer);
	gBS->FreePool(SelectBar.BltBuffer);
	gBS->FreePool(TryBackGround.BltBuffer); //0423
	//gBS->FreePool(AllviewBoard.BltBuffer);
	

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
	EFI_SIMPLE_TEXT_OUT_PROTOCOL* COut; 
	//EFI_INPUT_KEY                 KeyCode;
	//UINT8						  i=2; //
	UINT16						  HorizontalResolution;
	UINT16                        VerticalResolution;
	EFI_GRAPHICS_OUTPUT_PROTOCOL  *pGraph;
	SOLAR_IMAGE_INPUT             DesktopImage;
	UINT32                        ModeNum;
	//CHESS_BLOCK_INFO              *ChessBlock;
	COLOR_BLOCK_INFO			  *ColorBlock;
	
	
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

	//ChessBlock = (CHESS_BLOCK_INFO *) NULL;
	ColorBlock = (COLOR_BLOCK_INFO *) NULL;

	
	GraphicsSimpleDemo(&pGraph, &DesktopImage);
	//Status=(EFI_STATUS)GraphicsSimpleDemo(&pGraph, &DesktopImage, &ColorBlock);

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
