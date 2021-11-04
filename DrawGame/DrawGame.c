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
DrawBoundary (
  IN     EFI_GRAPHICS_OUTPUT_PROTOCOL *pGraphOutput,
  IN OUT SOLAR_IMAGE_INPUT            Image,
  IN     UINT16                       X,
  IN     UINT16                       Y,
  IN 	 UINT16						  Widge,
  IN	 UINT32						  Color
  )
{
	UINT16 				  x2 = X + Widge;
	UINT16 				  y2 = Y + Widge;

	//> d < u		                                                        
	DrawLine( pGraphOutput, &Image,   X,   Y,  x2,   Y, Color, FullLine); //上移
	DrawLine( pGraphOutput, &Image,  x2,   Y,  x2,  y2, Color, FullLine); //右移
	DrawLine( pGraphOutput, &Image,  x2,  y2,   X,  y2, Color, FullLine); //下移
	DrawLine( pGraphOutput, &Image,   X,  y2,   X,   Y, Color, FullLine); //左移

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
	UINT16 				a = 50 + (LX * COLOR_BLOCK_EDGE); //T
	UINT16 				b = 25 + (LY * COLOR_BLOCK_EDGE); //T

	if( ! ( LX>=0 && LX<25 && LY>=0 && LY<25) ) return SOLAR_SUCCESS;
	if(Locate[LX][LY] != DesireColor) return SOLAR_SUCCESS; //if(Locate[LX][LY]==DesireColor) 
	if(Locate[LX][LY] == NewColor) return SOLAR_SUCCESS;
	DrawRectangle(pGraphOutput, &Image, a, b, a+COLOR_BLOCK_EDGE, b+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, NewColor, 3); //T 
	Locate[LX][LY] = NewColor;
	
	FillRecursion(pGraphOutput, Image,  LX+1,    LY, NewColor, DesireColor);
	FillRecursion(pGraphOutput, Image,  LX-1,    LY, NewColor, DesireColor);
	FillRecursion(pGraphOutput, Image,    LX,  LY+1, NewColor, DesireColor);
	FillRecursion(pGraphOutput, Image,    LX,  LY-1, NewColor, DesireColor);
	
	return SOLAR_SUCCESS;
}



/*
DrawRectangle(*pGraph, &PaletteBar,  30, 20,  30+COLOR_BLOCK_EDGE, 20+COLOR_BLOCK_EDGE, SOLAR_RED, 0, Fill, SOLAR_YELLOW, 3); 
DrawRectangle(*pGraph, &PaletteBar,  60, 20,  60+COLOR_BLOCK_EDGE, 20+COLOR_BLOCK_EDGE, SOLAR_RED, 0, Fill, SOLAR_RED, 3); 
DrawRectangle(*pGraph, &PaletteBar,  90, 20,  90+COLOR_BLOCK_EDGE, 20+COLOR_BLOCK_EDGE, SOLAR_RED, 0, Fill, SOLAR_BLUE, 3); 
DrawRectangle(*pGraph, &PaletteBar, 120, 20, 120+COLOR_BLOCK_EDGE, 20+COLOR_BLOCK_EDGE, SOLAR_RED, 0, Fill, SOLAR_GREEN, 3); 
DrawRectangle(*pGraph, &PaletteBar, 150, 20, 150+COLOR_BLOCK_EDGE, 20+COLOR_BLOCK_EDGE, SOLAR_RED, 0, Fill, SOLAR_WHITE, 3);
Status = (*pGraph)->Blt(*pGraph,PaletteBar.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 530, PaletteBar.Width, PaletteBar.Height, 0); 
DrawBoundary(*pGraph, DrawingBoard, X, Y, COLOR_BLOCK_EDGE, SOLAR_WHITE);
*/
SOLAR_STATUS
SOLAR_LIB
SelectShow(
  IN     EFI_GRAPHICS_OUTPUT_PROTOCOL *pGraphOutput,
  IN OUT SOLAR_IMAGE_INPUT            Image, 
  IN     UINT16						  SelectX,
  IN 	 EFI_INPUT_KEY				  Tkey
)
{
	/*
	INT32				  num=5,fillc=2;
	INT32 				  ChoiseColor[num][fillc]={ { 30, SOLAR_YELLOW},{ 60, SOLAR_RED},{ 90, SOLAR_BLUE},{120, SOLAR_GREEN},{150, SOLAR_WHITE} };
	PrintString(pGraphOutput, &Image, 0,  0, SOLAR_WHITE,L" ArrayX: %d , ArrayY: %d ", num, fillc);// L" ArrayX: %d , ArrayY: %d ", num, fillc   L" Tesss "
	Status = (pGraphOutput)->Blt(pGraphOutput, Image.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 530, Image.Width, Image.Height, 0);
	*/
	//while來等待直到Tkey.ScanCode= SCAN_RIGHT;被敲滿5次挑出迴圈繼續判斷是敲哪個件做不同動作
	/*
	UINT16 loop=1,times=0;
	while(Tkey.ScanCode=SCAN_RIGHT)
	{
		times=1;
		//Draw
		times+=1;
		if(times=6)
			loop=0;
	}
	if (SX>150) => SX= 30 //right
	if (SX< 30) => SX=150 // left
	*/
	EFI_STATUS    		Status;
	UINT16 loop=1;
	while(loop)
	{
		getch(&Tkey);
		switch (Tkey.ScanCode)
		{
		case SCAN_RIGHT:
			DrawBoundary(pGraphOutput, Image, SelectX-1, 20-1, COLOR_BLOCK_EDGE+2, SOLAR_BLACK);
			DrawBoundary(pGraphOutput, Image, SelectX-2, 20-2, COLOR_BLOCK_EDGE+4 , SOLAR_BLACK);
			DrawBoundary(pGraphOutput, Image, SelectX-3, 20-3, COLOR_BLOCK_EDGE+6 , SOLAR_BLACK);
			if (SelectX >= 30 + 4*SELECT_STARTX_WIDTH)
				SelectX = 30;
			else
				SelectX += SELECT_STARTX_WIDTH;
			DrawBoundary(pGraphOutput, Image, SelectX-1, 20-1, COLOR_BLOCK_EDGE+2, SOLAR_RED);
			DrawBoundary(pGraphOutput, Image, SelectX-2, 20-2, COLOR_BLOCK_EDGE+4 , SOLAR_RED);
			DrawBoundary(pGraphOutput, Image, SelectX-3, 20-3, COLOR_BLOCK_EDGE+6 , SOLAR_RED);
			break;
		
		case SCAN_LEFT:
			DrawBoundary(pGraphOutput, Image, SelectX-1, 20-1, COLOR_BLOCK_EDGE+2, SOLAR_BLACK);
			DrawBoundary(pGraphOutput, Image, SelectX-2, 20-2, COLOR_BLOCK_EDGE+4 , SOLAR_BLACK);
			DrawBoundary(pGraphOutput, Image, SelectX-3, 20-3, COLOR_BLOCK_EDGE+6 , SOLAR_BLACK);
			if (SelectX <= 30 )
				SelectX = 30 + 4*SELECT_STARTX_WIDTH;
			else
				SelectX -= SELECT_STARTX_WIDTH;
			DrawBoundary(pGraphOutput, Image, SelectX-1, 20-1, COLOR_BLOCK_EDGE+2, SOLAR_RED);
			DrawBoundary(pGraphOutput, Image, SelectX-2, 20-2, COLOR_BLOCK_EDGE+4 , SOLAR_RED);
			DrawBoundary(pGraphOutput, Image, SelectX-3, 20-3, COLOR_BLOCK_EDGE+6 , SOLAR_RED);
			break;
		}
		Status = (pGraphOutput)->Blt(pGraphOutput, Image.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 530, Image.Width, Image.Height, 0);

		switch (Tkey.UnicodeChar)
		{
		case CHAR_CARRIAGE_RETURN :
			PrintString(pGraphOutput, &Image, 0, 0, SOLAR_WHITE, L" ENTER ");
			DrawBoundary(pGraphOutput, Image, SelectX-1, 20-1, COLOR_BLOCK_EDGE+2, SOLAR_BLACK);
			DrawBoundary(pGraphOutput, Image, SelectX-2, 20-2, COLOR_BLOCK_EDGE+4 , SOLAR_BLACK);
			DrawBoundary(pGraphOutput, Image, SelectX-3, 20-3, COLOR_BLOCK_EDGE+6 , SOLAR_BLACK);
			//get color and change
			loop=0;
			break;
		}
			
	}
/*
	switch (Tkey.ScanCode)
	{
	case SCAN_RIGHT:

		for(UINT16 SX=30; SX<160 ; SX+=SELECT_STARTX_WIDTH)
		{
			DrawBoundary(pGraphOutput, Image, SX, 20, COLOR_BLOCK_EDGE, SOLAR_WHITE);
			Tkey.ScanCode= SCAN_RIGHT;
			DrawBoundary(pGraphOutput, Image, SX+SELECT_STARTX_WIDTH, 20, COLOR_BLOCK_EDGE, SOLAR_DEEPPINK);
		}
		

		DrawBoundary(pGraphOutput, Image, SelectX, 20, COLOR_BLOCK_EDGE, SOLAR_WHITE);
		Status = (pGraphOutput)->Blt(pGraphOutput, Image.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 530, Image.Width, Image.Height, 0);
		if (SelectX >= 30 + 4*SELECT_STARTX_WIDTH)
			SelectX = 30;
		else
			SelectX += SELECT_STARTX_WIDTH;
		DrawBoundary(pGraphOutput, Image, SelectX, 20, COLOR_BLOCK_EDGE, SOLAR_LIME);
		Status = (pGraphOutput)->Blt(pGraphOutput, Image.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 530, Image.Width, Image.Height, 0);
		break;
	
	case SCAN_LEFT:
		break;
	}
	switch (Tkey.UnicodeChar)
	{
	case CHAR_CARRIAGE_RETURN :
		PrintString(pGraphOutput, &Image, 0, 0, SOLAR_WHITE, L" ENTER ");
		//get color and change
		break;
	}
*/
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
)
{
	EFI_STATUS    		Status;
	EFI_INPUT_KEY 		KeyCode;//TKey;
	UINT16				CoordinateX = COLOR_BOARD_X1; 
	UINT16				CoordinateY = COLOR_BOARD_Y1; 
	UINT16				X = CoordinateX, Y = CoordinateY;
	UINT16              LX = 0, LY = 0; 
	UINT32				DesireColor = 0;
	//UINT16				SelX=30,SelY=20;
	//EFI_SIMPLE_TEXT_IN_PROTOCOL     *cIn; //07G
  	//EFI_SIMPLE_TEXT_OUT_PROTOCOL    *cOut;//07G
	
	//cOut = gST->ConOut;//07G
  	//cIn  = gST->ConIn;//07G
	SOLAR_IMAGE_INPUT	SelectBar;
	SOLAR_IMAGE_INPUT	DrawingBoard; 
	SOLAR_IMAGE_INPUT	PaletteBar; 
	//SOLAR_IMAGE_INPUT	AllviewBoard;//for reset

	SelectBar.BltBuffer=(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)NULL;
	SelectBar.Width=170;
	SelectBar.Height=600;

	DrawingBoard.BltBuffer=(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)NULL; //
	DrawingBoard.Width=630; //
	DrawingBoard.Height=530; //

	PaletteBar.BltBuffer=(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)NULL;
	PaletteBar.Width=630;
	PaletteBar.Height=70;
/*
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

	if((PaletteBar.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool(PaletteBar.Width * PaletteBar.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)))==NULL){
		return EFI_OUT_OF_RESOURCES;
	}
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
	//PaletteBar
	EfiSetMem(PaletteBar.BltBuffer, PaletteBar.Height*PaletteBar.Width*sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL),EFI_BLACK);
	FillScreen(*pGraph, SOLAR_BLACK);
	//PrintString(*pGraph, &PaletteBar, 0,  20, SOLAR_WHITE, L" COOOOOOOLLLL");
	DrawRectangle(*pGraph, &PaletteBar,  30, 20,  30+COLOR_BLOCK_EDGE, 20+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, SOLAR_YELLOW, 3); 
	DrawRectangle(*pGraph, &PaletteBar,  60, 20,  60+COLOR_BLOCK_EDGE, 20+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, SOLAR_RED, 3); 
	DrawRectangle(*pGraph, &PaletteBar,  90, 20,  90+COLOR_BLOCK_EDGE, 20+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, SOLAR_BLUE, 3); 
	DrawRectangle(*pGraph, &PaletteBar, 120, 20, 120+COLOR_BLOCK_EDGE, 20+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, SOLAR_GREEN, 3); 
	DrawRectangle(*pGraph, &PaletteBar, 150, 20, 150+COLOR_BLOCK_EDGE, 20+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, SOLAR_WHITE, 3);
	PrintString(*pGraph, &PaletteBar, 250,  25, SOLAR_WHITE, L"Please type HEX RGB color(6): "); //or 用判斷限制只能打六位數字

	//Select function Bar(之後可以跟畫框一起Show出來先把Function做好)
	EfiSetMem(SelectBar.BltBuffer, SelectBar.Width * SelectBar.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), 0);
	FillScreen(*pGraph, SOLAR_YELLOW);
	PrintString(*pGraph, &SelectBar, 0,  30, SOLAR_WHITE, L" 'Backspace' : Reset Board ");
	PrintString(*pGraph, &SelectBar, 0,  60, SOLAR_WHITE, L" 'SPACE' : Make Boundary ");//show不出來 鑽太久惹 650 20是800600的位置 要以Select的大小畫
	PrintString(*pGraph, &SelectBar, 0,  90, SOLAR_WHITE, L" 'Tab' : Choose Color ");
	PrintString(*pGraph, &SelectBar, 0, 120, SOLAR_WHITE, L" 'D' : Delete Single Block ");
	PrintString(*pGraph, &SelectBar, 0, 150, SOLAR_WHITE, L" 'F' : Fill Color ");

	EfiSetMem(DrawingBoard.BltBuffer, DrawingBoard.Width * DrawingBoard.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_GREEN);
	FillScreen(*pGraph, SOLAR_BLACK); //SOLAR_YELLOW
	
	Status = (*pGraph)->Blt(*pGraph,PaletteBar.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 530, PaletteBar.Width, PaletteBar.Height, 0); //COLOR_BOARD_X1, COLOR_BOARD_Y1,
	Status = (*pGraph)->Blt(*pGraph, SelectBar.BltBuffer, EfiBltBufferToVideo, 0, 0, 630, 0, SelectBar.Width, SelectBar.Height, 0); //Select Bar
	Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); //先畫背景

	for (UINT16 i = 0; i < 26; i++) {
		DrawHorizontalLine(*pGraph, &DrawingBoard, CoordinateX, CoordinateY + i * COLOR_BLOCK_EDGE, COLOR_BOARD_WIDTH, SOLAR_WHITE, FullLine); 
		DrawVerticalLine(*pGraph, &DrawingBoard, CoordinateX + i * COLOR_BLOCK_EDGE, CoordinateY, COLOR_BOARD_HEIGH, SOLAR_WHITE, FullLine); 
	}
	DrawBoundary(*pGraph, DrawingBoard, CoordinateX, CoordinateY, COLOR_BLOCK_EDGE, SOLAR_RED);
	Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0);

	UINT16 loop = 1;
	while(loop){
		getch(&KeyCode);
		switch (KeyCode.ScanCode)
		{
		case SCAN_UP:
				DrawBoundary(*pGraph, DrawingBoard, X, Y, COLOR_BLOCK_EDGE, SOLAR_WHITE);
				if(Y <= CoordinateY)
					Y = CoordinateY + 24*COLOR_BLOCK_EDGE;
				else
					Y -= COLOR_BLOCK_EDGE;
				DrawBoundary(*pGraph, DrawingBoard, X, Y, COLOR_BLOCK_EDGE, SOLAR_RED);
				break;

		case SCAN_DOWN:
				DrawBoundary(*pGraph, DrawingBoard, X, Y, COLOR_BLOCK_EDGE, SOLAR_WHITE);
				if(Y >= CoordinateY + 24*COLOR_BLOCK_EDGE)
					Y = CoordinateY;
				else
					Y += COLOR_BLOCK_EDGE;
				DrawBoundary(*pGraph, DrawingBoard, X, Y, COLOR_BLOCK_EDGE, SOLAR_RED);
				break;

		case SCAN_RIGHT:
				DrawBoundary(*pGraph, DrawingBoard, X, Y, COLOR_BLOCK_EDGE, SOLAR_WHITE);
				if (X >= CoordinateX + 24*COLOR_BLOCK_EDGE)
					X = CoordinateX;
				else
					X += COLOR_BLOCK_EDGE;
				DrawBoundary(*pGraph, DrawingBoard, X, Y, COLOR_BLOCK_EDGE, SOLAR_RED);
				break;

		case SCAN_LEFT:
				DrawBoundary(*pGraph, DrawingBoard, X, Y, COLOR_BLOCK_EDGE, SOLAR_WHITE);
				if (X <= CoordinateX)
					X = CoordinateX + 24*COLOR_BLOCK_EDGE;
				else
					X -= COLOR_BLOCK_EDGE;
				DrawBoundary(*pGraph, DrawingBoard, X, Y, COLOR_BLOCK_EDGE, SOLAR_RED);
				break;
		
		case SCAN_ESC:
				loop = 0;
				break;

		}
		Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0);
		
		switch (KeyCode.UnicodeChar)
		{
		case CHAR_SPACE  :
				DrawRectangle(*pGraph, &DrawingBoard, X, Y, X+COLOR_BLOCK_EDGE, Y+COLOR_BLOCK_EDGE, SOLAR_RED, 0, Fill, SOLAR_PALEGREEN, 3);  
				LX = (X - COLOR_BOARD_X1) / COLOR_BLOCK_EDGE;
				LY = (Y - COLOR_BOARD_Y1) / COLOR_BLOCK_EDGE;
				Locate[LX][LY] = SOLAR_PALEGREEN;
				//PrintString(*pGraph, &SelectBar, 0, 120, SOLAR_WHITE, L" Locate :[%d][%d]",LX,LY);
				//PrintString(*pGraph, &SelectBar, 0, 140, SOLAR_WHITE, L" L color %x",Locate[LX][LY]);
				//Status = (*pGraph)->Blt(*pGraph, SelectBar.BltBuffer, EfiBltBufferToVideo, 0, 0, 630, 0, SelectBar.Width, SelectBar.Height, 0); //Select Bar
				break;

		//刪除單格
		case CHAR_D:
		case CHAR_d:
				DrawRectangle(*pGraph, &DrawingBoard, X, Y, X+COLOR_BLOCK_EDGE, Y+COLOR_BLOCK_EDGE, SOLAR_RED, 0, Fill, SOLAR_BLACK, 3);
				LX = (X - COLOR_BOARD_X1) / COLOR_BLOCK_EDGE;
				LY = (Y - COLOR_BOARD_Y1) / COLOR_BLOCK_EDGE;
				Locate[LX][LY] = SOLAR_BLACK;
				break;

		//reset畫板
		case CHAR_BackSpace: 
				X = COLOR_BOARD_X1;
				Y = COLOR_BOARD_Y1;
		        EfiSetMem(DrawingBoard.BltBuffer, DrawingBoard.Width * DrawingBoard.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);
				for (UINT16 i = 0; i < 26; i++) {
						DrawHorizontalLine(*pGraph, &DrawingBoard, CoordinateX, CoordinateY + i * COLOR_BLOCK_EDGE, COLOR_BOARD_WIDTH, SOLAR_WHITE, FullLine); 
						DrawVerticalLine(*pGraph, &DrawingBoard, CoordinateX + i * COLOR_BLOCK_EDGE, CoordinateY, COLOR_BOARD_HEIGH, SOLAR_WHITE, FullLine); 
					}
				DrawBoundary(*pGraph, DrawingBoard, X, Y, COLOR_BLOCK_EDGE, SOLAR_RED);
				for(int j=0; j<25 ; j++)
				{
					for(int k=0; k<25 ; k++)
					{
						Locate[j][k] = 000000; //之後抓陣列可能要確認這裡的數值0510
					}
				}
				break;

		//復原成上一個樣子
		//case CHAR_R: 
		//case CHAR_r:

		//倒色 SOLAR_DEEPPINK
		case CHAR_F:
		case CHAR_f: 
				LX = (X - COLOR_BOARD_X1) / COLOR_BLOCK_EDGE;
				LY = (Y - COLOR_BOARD_Y1) / COLOR_BLOCK_EDGE;
				DesireColor = Locate[LX][LY];
				FillRecursion(*pGraph, DrawingBoard, LX, LY, SOLAR_DEEPPINK, DesireColor);
				break;

		case CHAR_TAB :
				DrawBoundary(*pGraph, DrawingBoard, X, Y, COLOR_BLOCK_EDGE, SOLAR_WHITE);
				Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); //T
				//DrawRectangle(*pGraph, &PaletteBar,  30,  20,  30+COLOR_BLOCK_EDGE, 20+COLOR_BLOCK_EDGE, SOLAR_RED, 0, Fill, SOLAR_YELLOW, 3); 
				DrawBoundary(*pGraph, PaletteBar, 30-1, 20-1, COLOR_BLOCK_EDGE+2, SOLAR_RED);
				DrawBoundary(*pGraph, PaletteBar, 30-2, 20-2, COLOR_BLOCK_EDGE+4 , SOLAR_RED);
				DrawBoundary(*pGraph, PaletteBar, 30-3, 20-3, COLOR_BLOCK_EDGE+6 , SOLAR_RED);
				Status = (*pGraph)->Blt(*pGraph,PaletteBar.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 530, PaletteBar.Width, PaletteBar.Height, 0);
				//UINT16 ttab=1; //switch (KeyCode.UnicodeChar)
				UINT16 SX=30;
				//while(ttab){
					//getch(&TKey);
					SelectShow(*pGraph, PaletteBar, SX, KeyCode);
					Status = (*pGraph)->Blt(*pGraph, PaletteBar.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 530, PaletteBar.Width, PaletteBar.Height, 0); //T
				//	if(TKey.UnicodeChar == CHAR_CARRIAGE_RETURN)
				//		ttab=0;
				//}
				DrawLine( *pGraph, &PaletteBar,   0,   20,  COLOR_BLOCK_EDGE,   20+COLOR_BLOCK_EDGE, SOLAR_RED, DottedLineTen);//FullLine
				DrawBoundary(*pGraph, DrawingBoard, X, Y, COLOR_BLOCK_EDGE, SOLAR_RED);
				Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); //T
				Status = (*pGraph)->Blt(*pGraph,PaletteBar.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 530, PaletteBar.Width, PaletteBar.Height, 0);
				break;//進程式的判斷

		}	
		Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); //T

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
	gBS->FreePool(PaletteBar.BltBuffer); 
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
	//COLOR_BLOCK_INFO			  *ColorBlock;
	
	
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
	//ColorBlock = (COLOR_BLOCK_INFO *) NULL;

	
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
