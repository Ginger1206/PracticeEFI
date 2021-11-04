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
UINT16						  ChessBlockAmount;
INT32					  	  Locate[25][25];
INT32					  	  **MaskAry[25][25];



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
	UINT16 				  x2 = X + COLOR_BLOCK_EDGE;
	UINT16 				  y2 = Y + COLOR_BLOCK_EDGE;

	//> d < u		                                                          //哪個方向保留那個其他三個畫為白
	DrawLine( pGraphOutput, &Image,   X,   Y,  x2,   Y, SOLAR_RED, FullLine); //上移
	DrawLine( pGraphOutput, &Image,  x2,   Y,  x2,  y2, SOLAR_RED, FullLine); //右移
	DrawLine( pGraphOutput, &Image,  x2,  y2,   X,  y2, SOLAR_RED, FullLine); //下移
	DrawLine( pGraphOutput, &Image,   X,  y2,   X,   Y, SOLAR_RED, FullLine); //左移

	Status = (pGraphOutput)->Blt(pGraphOutput, Image.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image.Width, Image.Height, 0);
	
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
	UINT16      		  x2 = X + COLOR_BLOCK_EDGE; 
	UINT16 			  	  y2 = Y + COLOR_BLOCK_EDGE;

	switch (ScanCode)
	{
	case SCAN_UP:
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
	Status = (pGraphOutput)->Blt(pGraphOutput, Image.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image.Width, Image.Height, 0);

	return SOLAR_SUCCESS;
}
/*
原呼叫主程式
case CHAR_F: //倒色 SOLAR_DEEPPINK
	LX = (X - COLOR_BOARD_X1) / COLOR_BLOCK_EDGE;
	LY = (Y - COLOR_BOARD_Y1) / COLOR_BLOCK_EDGE;
	DesireColor = Locate[LX][LY];
	FillRecursion(*pGraph, DrawingBoard, LX, LY, SOLAR_DEEPPINK, DesireColor);
	break;
*/
SOLAR_STATUS
SOLAR_LIB
FillRecursion(
  IN     EFI_GRAPHICS_OUTPUT_PROTOCOL *pGraphOutput,
  IN OUT SOLAR_IMAGE_INPUT            Image,
  IN     UINT16                       LX,
  IN     UINT16                       LY,
  IN     INT32						  NewColor,
  IN	 INT32						  DesireColor
  )
{
	//EFI_STATUS    		Status; //T
	UINT16 				a = 50 + (LX * COLOR_BLOCK_EDGE); //T
	UINT16 				b = 25 + (LY * COLOR_BLOCK_EDGE); //T

	if( ! ( LX>=0 && LX<25 && LY>=0 && LY<25) ) return SOLAR_SUCCESS;
	if(Locate[LX][LY] != DesireColor) return SOLAR_SUCCESS; //if(Locate[LX][LY]==DesireColor) 
	if(Locate[LX][LY] == NewColor) return SOLAR_SUCCESS;
	DrawRectangle(pGraphOutput, &Image, a, b, a+COLOR_BLOCK_EDGE, b+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, NewColor, 3); //T 
	//Status = (pGraphOutput)->Blt(pGraphOutput, Image.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image.Width, Image.Height, 0); //T
	Locate[LX][LY] = NewColor;
	
	FillRecursion(pGraphOutput, Image,  LX+1,    LY, NewColor, DesireColor);
	FillRecursion(pGraphOutput, Image,  LX-1,    LY, NewColor, DesireColor);
	FillRecursion(pGraphOutput, Image,    LX,  LY+1, NewColor, DesireColor);
	FillRecursion(pGraphOutput, Image,    LX,  LY-1, NewColor, DesireColor);
	
	return SOLAR_SUCCESS;
}
/*
//*******************Try for output mask Array
SOLAR_STATUS
SOLAR_LIB
FillRecursion(
  //IN OUT INT32					  	  MaskAry,
  IN     UINT16                       LX,
  IN     UINT16                       LY,
  IN     INT32						  NewColor,
  IN	 INT32						  DesireColor
  )
{
	if( ! ( LX>=0 && LX<25 && LY>=0 && LY<25) ) return SOLAR_SUCCESS;
	if(Locate[LX][LY] != DesireColor) return SOLAR_SUCCESS; //if(Locate[LX][LY]==DesireColor) 
	if(Locate[LX][LY] == NewColor) return SOLAR_SUCCESS;

	Locate[LX][LY] = NewColor;
	//MaskAry[LX][LY]  = 1;
	
	FillRecursion(  LX+1,    LY, NewColor, DesireColor);
	FillRecursion(  LX-1,    LY, NewColor, DesireColor);
	FillRecursion(    LX,  LY+1, NewColor, DesireColor);
	FillRecursion(    LX,  LY-1, NewColor, DesireColor);
	
	return SOLAR_SUCCESS;
}

SOLAR_STATUS
SOLAR_LIB
FillRecursion(
  IN     EFI_GRAPHICS_OUTPUT_PROTOCOL *pGraphOutput,
  IN OUT SOLAR_IMAGE_INPUT            Image,
  IN     UINT16                       LX,
  IN     UINT16                       LY,
  IN     INT32						  NewColor,
  IN	 INT32						  DesireColor
  )
{
	EFI_STATUS    		Status;
	UINT16 				a=50+(LX*COLOR_BLOCK_EDGE); 
	UINT16 				b=25+(LY*COLOR_BLOCK_EDGE);

	//if(!(LX>=0 && LX<25 && LY>=0 && LY <25))return SOLAR_SUCCESS;
	//if(Locate[LX][LY]!=DesireColor)return SOLAR_SUCCESS; //if(Locate[LX][LY]==DesireColor) 
	if(Locate[LX][LY]==NewColor)return SOLAR_SUCCESS;
	DrawRectangle(pGraphOutput, &Image, a, b, a+COLOR_BLOCK_EDGE, b+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, NewColor, 3); 
	Status=(pGraphOutput)->Blt(pGraphOutput,Image.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image.Width, Image.Height, 0);
	Locate[LX][LY]=NewColor;

	INT16 s[4]={1,-1,0,0};//1U,65535U,0U,0U
	INT16 w[4]={0,0,1,-1};
	for(INT16 i=0;i<4;i++)
	{
		//
		if(!(LX+s[i]>=0 && LX+s[i]<25 && LY+w[i]>=0 &&LY+w[i] <25))continue;
		if(Locate[LX+s[i]][LY+w[i]]==DesireColor)
		{
			return FillRecursion(pGraphOutput,Image,LX+s[i],LY+w[i],NewColor,DesireColor);
		}
		
	}
	
	return SOLAR_SUCCESS;
}
*/

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
)
{
	EFI_STATUS    		Status;
	EFI_INPUT_KEY 		KeyCode;
	INT16				CoordinateX = COLOR_BOARD_X1; 
	INT16				CoordinateY = COLOR_BOARD_Y1; 
	UINT16				X = CoordinateX, Y = CoordinateY;
	UINT16              LX = 0, LY = 0; //INT32				LX=0,LY=0;
	INT32				DesireColor = 0;
	
	
	SOLAR_IMAGE_INPUT	SelectBar;
	SOLAR_IMAGE_INPUT	DrawingBoard; 
	//SOLAR_IMAGE_INPUT	TryBG; 
	//SOLAR_IMAGE_INPUT	AllviewBoard;//for reset

	SelectBar.BltBuffer=(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)NULL;
	SelectBar.Width=170;
	SelectBar.Height=600;

	DrawingBoard.BltBuffer=(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)NULL; //
	DrawingBoard.Width=630; //
	DrawingBoard.Height=550; //
/*
	TryBG.BltBuffer=(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)NULL;
	TryBG.Width=630;
	TryBG.Height=550;

	AllviewBoard.BltBuffer=(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)NULL;
	AllviewBoard.Width=630;
	AllviewBoard.Height=550;
*/
	if ((SelectBar.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool(SelectBar.Width * SelectBar.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)))==NULL){
		return EFI_OUT_OF_RESOURCES;
	}

	if ((DrawingBoard.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool(DrawingBoard.Width * DrawingBoard.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)))==NULL){
		return EFI_OUT_OF_RESOURCES;
	} //
/*
	if((TryBG.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool(TryBG.Width * TryBG.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)))==NULL){
		return EFI_OUT_OF_RESOURCES;
	}

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
	//Select function Bar(之後可以跟畫框一起Show出來先把Function做好)
	EfiSetMem(SelectBar.BltBuffer, SelectBar.Width * SelectBar.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), 0);
	FillScreen(*pGraph, SOLAR_YELLOW);
	PrintString(*pGraph, &SelectBar, 0,  30, SOLAR_WHITE, L" 'Backspace' : Reset Board ");
	PrintString(*pGraph, &SelectBar, 0,  60, SOLAR_WHITE, L" 'SPACE' : Make Boundary ");//show不出來 鑽太久惹 650 20是800600的位置 要以Select的大小畫
	PrintString(*pGraph, &SelectBar, 0,  90, SOLAR_WHITE, L" 'D' : Delete Single Boundary ");
	PrintString(*pGraph, &SelectBar, 0, 120, SOLAR_WHITE, L" 'F' : Fill Color ");

	EfiSetMem(DrawingBoard.BltBuffer, DrawingBoard.Width * DrawingBoard.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_GREEN);
	FillScreen(*pGraph, SOLAR_BLACK); //SOLAR_YELLOW
	//EfiSetMem(ColorBlock.BltBuffer, ColorBlock.Height*ColorBlock.Width*sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL),0);
	//Status=(*pGraph)->Blt(*pGraph,ColorBlock.BltBuffer, EfiBltBufferToVideo, 0, 0,COLOR_BOARD_X1, COLOR_BOARD_Y1, ColorBlock.Width, ColorBlock.Height, 0); //COLOR_BOARD_X1, COLOR_BOARD_Y1,
	
	Status = (*pGraph)->Blt(*pGraph, SelectBar.BltBuffer, EfiBltBufferToVideo, 0, 0, 630, 0, SelectBar.Width, SelectBar.Height, 0); //Select Bar
	Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); //先畫背景
	for (UINT16 i = 0; i < 26; i++) {
		DrawHorizontalLine(*pGraph, &DrawingBoard, CoordinateX, CoordinateY + i * COLOR_BLOCK_EDGE, COLOR_BOARD_WIDTH, SOLAR_WHITE, FullLine); 
		DrawVerticalLine(*pGraph, &DrawingBoard, CoordinateX + i * COLOR_BLOCK_EDGE, CoordinateY, COLOR_BOARD_HEIGH, SOLAR_WHITE, FullLine); 
	}

	RecBoundary(*pGraph, DrawingBoard, CoordinateX, CoordinateY);

	UINT16 loop = 1;
	while(loop){
		getch(&KeyCode);
		switch (KeyCode.ScanCode)
		{
		case SCAN_UP:
				if(Y <= CoordinateY){
					MovDelBdy(*pGraph, DrawingBoard, X, Y, KeyCode.ScanCode);
					DrawLine(*pGraph, &DrawingBoard, X, Y, X+COLOR_BLOCK_EDGE, Y, SOLAR_WHITE, FullLine); //上移
					Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0);
					Y = CoordinateY + 24*COLOR_BLOCK_EDGE;
					RecBoundary(*pGraph, DrawingBoard, X, Y);
				}
				else{
					MovDelBdy(*pGraph, DrawingBoard, X, Y, KeyCode.ScanCode);
					Y -= COLOR_BLOCK_EDGE;
					RecBoundary(*pGraph, DrawingBoard, X, Y);
				}
				break;

		case SCAN_DOWN:
				if(Y >= CoordinateY + 24*COLOR_BLOCK_EDGE){
					MovDelBdy(*pGraph, DrawingBoard, X, Y, KeyCode.ScanCode);
					DrawLine( *pGraph, &DrawingBoard, X+COLOR_BLOCK_EDGE, Y+COLOR_BLOCK_EDGE, X, Y+COLOR_BLOCK_EDGE, SOLAR_WHITE, FullLine); //下移
					Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0);
					Y = CoordinateY;
					RecBoundary(*pGraph, DrawingBoard, X, Y);
				}
				else{
					MovDelBdy(*pGraph, DrawingBoard, X, Y, KeyCode.ScanCode);
					Y += COLOR_BLOCK_EDGE;
					RecBoundary(*pGraph, DrawingBoard, X, Y);
				}
				break;
		case SCAN_RIGHT:
				if (X >= CoordinateX + 24*COLOR_BLOCK_EDGE){
					MovDelBdy(*pGraph, DrawingBoard, X, Y, KeyCode.ScanCode);
					DrawLine(*pGraph, &DrawingBoard, X+COLOR_BLOCK_EDGE, Y, X+COLOR_BLOCK_EDGE, Y+COLOR_BLOCK_EDGE, SOLAR_WHITE, FullLine); //右移
					Status=(*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0);
					X = CoordinateX;
					RecBoundary(*pGraph, DrawingBoard, X, Y);
				}
				else{ //正常右移
					MovDelBdy(*pGraph, DrawingBoard, X, Y, KeyCode.ScanCode);
					X += COLOR_BLOCK_EDGE;
					RecBoundary(*pGraph, DrawingBoard, X, Y);
				}
				break;
		case SCAN_LEFT:
				if (X <= CoordinateX){
					MovDelBdy(*pGraph, DrawingBoard, X, Y, KeyCode.ScanCode);
					DrawLine(*pGraph, &DrawingBoard, X, Y+COLOR_BLOCK_EDGE, X, Y, SOLAR_WHITE, FullLine); //左移
					Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0);
					X = CoordinateX + 24*COLOR_BLOCK_EDGE;
					RecBoundary(*pGraph, DrawingBoard, X, Y);
				}
				else{ 
					MovDelBdy(*pGraph, DrawingBoard, X, Y, KeyCode.ScanCode);
					X -= COLOR_BLOCK_EDGE;
					RecBoundary(*pGraph, DrawingBoard, X, Y);
				}
			break;
		
		case SCAN_ESC:
			loop = 0;
			break;
		}

		switch (KeyCode.UnicodeChar)
		{
		case CHAR_SPACE  :
				DrawRectangle(*pGraph, &DrawingBoard, X, Y, X+COLOR_BLOCK_EDGE, Y+COLOR_BLOCK_EDGE, SOLAR_RED, 0, Fill, SOLAR_PALEGREEN, 3);  
				LX = (X-COLOR_BOARD_X1) / COLOR_BLOCK_EDGE;
				LY = (Y-COLOR_BOARD_Y1) / COLOR_BLOCK_EDGE;
				Locate[LX][LY] = SOLAR_PALEGREEN;
				//PrintString(*pGraph, &SelectBar, 0, 120, SOLAR_WHITE, L" Locate :[%d][%d]",LX,LY);
				//PrintString(*pGraph, &SelectBar, 0, 140, SOLAR_WHITE, L" L color %x",Locate[LX][LY]);
				//Status = (*pGraph)->Blt(*pGraph, SelectBar.BltBuffer, EfiBltBufferToVideo, 0, 0, 630, 0, SelectBar.Width, SelectBar.Height, 0); //Select Bar
				Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); 
				break;
		case CHAR_D: //刪除單格
				DrawRectangle(*pGraph, &DrawingBoard, X, Y, X+COLOR_BLOCK_EDGE, Y+COLOR_BLOCK_EDGE, SOLAR_RED, 0, Fill, SOLAR_BLACK, 3);
				LX = (X-COLOR_BOARD_X1) / COLOR_BLOCK_EDGE;
				LY = (Y-COLOR_BOARD_Y1) / COLOR_BLOCK_EDGE;
				Locate[LX][LY] = SOLAR_BLACK;
				Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); 
				break;
		case CHAR_BackSpace: //reset畫板
				X = COLOR_BOARD_X1;
				Y = COLOR_BOARD_Y1;
		        EfiSetMem(DrawingBoard.BltBuffer, DrawingBoard.Width * DrawingBoard.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);
				for (UINT16 i = 0; i < 26; i++) {
						DrawHorizontalLine(*pGraph, &DrawingBoard, CoordinateX, CoordinateY + i * COLOR_BLOCK_EDGE, COLOR_BOARD_WIDTH, SOLAR_WHITE, FullLine); 
						DrawVerticalLine(*pGraph, &DrawingBoard, CoordinateX + i * COLOR_BLOCK_EDGE, CoordinateY, COLOR_BOARD_HEIGH, SOLAR_WHITE, FullLine); 
					}
				RecBoundary(*pGraph, DrawingBoard, X, Y);
				Status=(*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0);
				for(int j=0; j<25 ; j++)
				{
					for(int k=0; k<25 ; k++)
					{
						Locate[j][k] = 000000;
					}
				}
				break;
		//case CHAR_R: //復原成上一個樣子
/*		case CHAR_F: //倒色 SOLAR_DEEPPINK
				LX = (X - COLOR_BOARD_X1) / COLOR_BLOCK_EDGE;
				LY = (Y - COLOR_BOARD_Y1) / COLOR_BLOCK_EDGE;
				DesireColor = Locate[LX][LY];
				//*MaskAry[25][25]= 0 ;
				FillRecursion( LX, LY, SOLAR_DEEPPINK, DesireColor);
				for(UINT16 j=0; j<25 ; j++)
				{
					UINT16 m=50+(j*COLOR_BLOCK_EDGE);
					for(UINT16 k=0; k<25 ; k++)
					{
						UINT16 n=25+(k*COLOR_BLOCK_EDGE);
						if(Locate[j][k]==SOLAR_DEEPPINK)
						{
							DrawRectangle(*pGraph, &DrawingBoard, m, n, m+COLOR_BLOCK_EDGE, n+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, SOLAR_DEEPPINK, 3); //T 
						}
						
					}
				}
				Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); //T
				break;
*/
		case CHAR_F: //倒色 SOLAR_DEEPPINK
				LX = (X - COLOR_BOARD_X1) / COLOR_BLOCK_EDGE;
				LY = (Y - COLOR_BOARD_Y1) / COLOR_BLOCK_EDGE;
				DesireColor = Locate[LX][LY];
				FillRecursion(*pGraph, DrawingBoard, LX, LY, SOLAR_DEEPPINK, DesireColor);
				Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); //T
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
	*/


	gBS->FreePool(Image->BltBuffer);
	gBS->FreePool(DrawingBoard.BltBuffer);
	gBS->FreePool(SelectBar.BltBuffer);
	//gBS->FreePool(TryBG.BltBuffer); //0423
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
