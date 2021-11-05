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
  #include "memory.h"//sdk root basic func define 
  #include EFI_PROTOCOL_DEFINITION (ConsoleControl)
}
#include "SolarCppLib.h"
#include "ImageBlock.h"
#include "DrawGame.h"
//#include "DrawFunc.h"//need to comfirm "memcpy" func. is use this .h 211025G

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
INT32					  	  RecAry[25][25];



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
MakeThickLine(
  IN	 EFI_GRAPHICS_OUTPUT_PROTOCOL *pGraphOutput,
  IN OUT SOLAR_IMAGE_INPUT 			  Image,
  IN	 UINT16						  X,
  IN	 UINT32						  Color
)
{
	DrawBoundary(pGraphOutput, Image, X-1, 20-1, COLOR_BLOCK_EDGE+2 , Color);
	DrawBoundary(pGraphOutput, Image, X-2, 20-2, COLOR_BLOCK_EDGE+4 , Color);
	DrawBoundary(pGraphOutput, Image, X-3, 20-3, COLOR_BLOCK_EDGE+6 , Color);

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
	if(Locate[LX][LY] != DesireColor) return SOLAR_SUCCESS; 
	if(Locate[LX][LY] == NewColor) return SOLAR_SUCCESS;
	DrawRectangle(pGraphOutput, &Image, a, b, a+COLOR_BLOCK_EDGE, b+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, NewColor, 3); //T 
	Locate[LX][LY] = NewColor;
	
	FillRecursion(pGraphOutput, Image,  LX+1,    LY, NewColor, DesireColor);
	FillRecursion(pGraphOutput, Image,  LX-1,    LY, NewColor, DesireColor);
	FillRecursion(pGraphOutput, Image,    LX,  LY+1, NewColor, DesireColor);
	FillRecursion(pGraphOutput, Image,    LX,  LY-1, NewColor, DesireColor);
	
	return SOLAR_SUCCESS;
}

SOLAR_SYSRET
SOLAR_LIB
ConditionColor(
	IN  UINT64    X
)
{
	UINT32			    Color=0;
	if(X==30) {Color=SOLAR_YELLOW;}
	if(X==60) {Color=SOLAR_RED;}
	if(X==90) {Color=SOLAR_BLUE;}
	if(X==120) {Color=SOLAR_GREEN;}
	if(X==150) {Color=SOLAR_FLORALW;}

	return Color;
}

SOLAR_SYSRET
SOLAR_API
DecToHex(
	IN	UINT32	X
)
{
	if(X>=58)
		X-=55;
	else
		X-=48;
	
	return X;
}

SOLAR_SYSRET
SOLAR_LIB
TypeColor(
  IN     EFI_GRAPHICS_OUTPUT_PROTOCOL *pGraphOutput,
  IN OUT SOLAR_IMAGE_INPUT			  Image,
  IN 	 EFI_INPUT_KEY				  Tkey,
  IN     UINT32						  TColor
)
{
	EFI_STATUS		    Status;
	UINT16				XL=435;//XLocation
	UINT32				UC[6]={0};
	UINT16				N=0;
	UINT32				T[6];
	UINT32              R=0,G=0,B=0;
	//TColor=SOLAR_DEEPPINK; //user type color put here 210628G

	UINT16 loop=1;
	while(loop)
	{
		getch(&Tkey);
		XL+=10;
		switch (Tkey.UnicodeChar)
		{
		case CHAR_A:
		case CHAR_a:
			PrintString(pGraphOutput, &Image, XL, 25, SOLAR_DEEPPINK, L"A");//LIME color for test PURPLE
			UC[N]='A';
			break;
		
		case CHAR_B:
		case CHAR_b:
			PrintString(pGraphOutput, &Image, XL, 25, SOLAR_DEEPPINK, L"B");
			UC[N]='B';
			break;

		case CHAR_C:
		case CHAR_c:
			PrintString(pGraphOutput, &Image, XL, 25, SOLAR_DEEPPINK, L"C");
			UC[N]='C';
			break;

		case CHAR_D:
		case CHAR_d:
			PrintString(pGraphOutput, &Image, XL, 25, SOLAR_DEEPPINK, L"D");
			UC[N]='D';
			break;

		case CHAR_E:
		case CHAR_e:
			PrintString(pGraphOutput, &Image, XL, 25, SOLAR_DEEPPINK, L"E");
			UC[N]='E';
			break;

		case CHAR_F:
		case CHAR_f:
			PrintString(pGraphOutput, &Image, XL, 25, SOLAR_DEEPPINK, L"F");
			UC[N]='F';
			break;

		case CHAR_0:
			PrintString(pGraphOutput, &Image, XL, 25, SOLAR_DEEPPINK, L"0");
			UC[N]='0';
			break;

		case CHAR_1:
			PrintString(pGraphOutput, &Image, XL, 25, SOLAR_DEEPPINK, L"1");
			UC[N]='1';
			break;

		case CHAR_2:
			PrintString(pGraphOutput, &Image, XL, 25, SOLAR_DEEPPINK, L"2");
			UC[N]='2';
			break;

		case CHAR_3:
			PrintString(pGraphOutput, &Image, XL, 25, SOLAR_DEEPPINK, L"3");
			UC[N]='3';
			break;

		case CHAR_4:
			PrintString(pGraphOutput, &Image, XL, 25, SOLAR_DEEPPINK, L"4");
			UC[N]='4';
			break;

		case CHAR_5:
			PrintString(pGraphOutput, &Image, XL, 25, SOLAR_DEEPPINK, L"5");
			UC[N]='5';
			break;

		case CHAR_6:
			PrintString(pGraphOutput, &Image, XL, 25, SOLAR_DEEPPINK, L"6");
			UC[N]='6';
			break;

		case CHAR_7:
			PrintString(pGraphOutput, &Image, XL, 25, SOLAR_DEEPPINK, L"7");
			UC[N]='7';
			break;

		case CHAR_8:
			PrintString(pGraphOutput, &Image, XL, 25, SOLAR_DEEPPINK, L"8");
			UC[N]='8';
			break;

		case CHAR_9:
			PrintString(pGraphOutput, &Image, XL, 25, SOLAR_DEEPPINK, L"9");
			UC[N]='9';
			break;

		case CHAR_CARRIAGE_RETURN :
			if(N<5)
				PrintString(pGraphOutput, &Image, 515, 25, SOLAR_RED, L"[Error!!!]");
			loop=0;
			break;

		case CHAR_Q:
		case CHAR_q:
			PrintString(pGraphOutput, &Image, 515, 25, SOLAR_RED, L"[Quit!]");
			loop=0;
			break;
		}

		for(UINT16 i=0;i<6;i++)
		{
			//PrintString(pGraphOutput, &Image, 0,  0+i*11, SOLAR_RED, L"Array [%d] is %c",i, UC[i] ); //Test char show
			T[i]=DecToHex(UC[i]);
		}

		if(N>5)
		{
			if(Tkey.UnicodeChar == CHAR_CARRIAGE_RETURN)
			{
				R = T[0]*16 + T[1];
				G = T[2]*16 + T[3];
				B = T[4]*16 + T[5];
				TColor=CombineRGB(R, G, B);
				PrintString(pGraphOutput, &Image, 515,  25, SOLAR_RED, L"[ENTER]"); 
				//PrintString(pGraphOutput, &Image, 515,  40, TColor, L"Now color here!!!"); //Test
				Status = (pGraphOutput)->Blt(pGraphOutput, Image.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 530, Image.Width, Image.Height, 0);
				loop=0;
				break;
			}
			PrintString(pGraphOutput, &Image, XL, 25, SOLAR_RED, L"  over type!");//LIME color for test
			XL=435;
			N=0;
			//DrawRectangle(pGraphOutput, &Image, 445, 17, 513, 35, SOLAR_LIME, 0, Fill, SOLAR_BLACK, 3); 
			//PrintString(pGraphOutput, &Image, XL,  25, SOLAR_WHITE, L"#________"); 
			loop=0;
		}
		else
		{
			N+=1;
		}

		Status = (pGraphOutput)->Blt(pGraphOutput, Image.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 530, Image.Width, Image.Height, 0);
	}
	/*
	//Value test show
	for(UINT16 i=0;i<6;i++)
	{ PrintString(pGraphOutput, &Image, 150, 0+i*11, SOLAR_GREEN, L"Array[%d] value is %d", i, T[i]); }
	R = T[0]*16 + T[1];
	PrintString(pGraphOutput, &Image, 300,  0, SOLAR_RED, L"R is [%d] ", R); 
	G = T[2]*16 + T[3];
	PrintString(pGraphOutput, &Image, 300,  11, SOLAR_RED, L"G is [%d] ", G); 
	B = T[4]*16 + T[5];
	PrintString(pGraphOutput, &Image, 300,  22, SOLAR_RED, L"B is [%d] ", B);
	Status = (pGraphOutput)->Blt(pGraphOutput, Image.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 530, Image.Width, Image.Height, 0);
	*/
	//maybe erase show text can put here then show again
	return TColor;
}

SOLAR_SYSRET
SOLAR_LIB
SelectShow(
  IN     EFI_GRAPHICS_OUTPUT_PROTOCOL *pGraphOutput,
  IN OUT SOLAR_IMAGE_INPUT            Image, 
  IN     UINT16						  SelectX,
  IN 	 EFI_INPUT_KEY				  Tkey,
  IN     UINT32                       NowColor,
  IN OUT UINT32						  *TabC
)
{
	EFI_STATUS    		Status;
	UINT32			    SColor=0;
	//UINT32				TabC=0;

	UINT16 loop=1;
	while(loop)
	{
		getch(&Tkey);
		switch (Tkey.ScanCode)
		{
		case SCAN_RIGHT:
			MakeThickLine(pGraphOutput, Image, SelectX, SOLAR_BLACK);
			if (SelectX >= SELECT_X_COORDINATE + 4*SELECT_STARTX_WIDTH)
				SelectX = SELECT_X_COORDINATE;
			else
				SelectX += SELECT_STARTX_WIDTH;
			MakeThickLine(pGraphOutput, Image, SelectX, SOLAR_RED);
			break;
		
		case SCAN_LEFT:
			MakeThickLine(pGraphOutput, Image, SelectX, SOLAR_BLACK);
			if (SelectX <= SELECT_X_COORDINATE )
				SelectX = SELECT_X_COORDINATE + 4*SELECT_STARTX_WIDTH;
			else
				SelectX -= SELECT_STARTX_WIDTH;
			MakeThickLine(pGraphOutput, Image, SelectX, SOLAR_RED);
			break;

		}
		//PrintString(pGraphOutput, &Image, 100,  0, SOLAR_WHITE,L" Get X position :%d", SelectX);// L" ArrayX: %d , ArrayY: %d ", num, fillc   L" Tesss "
		Status = (pGraphOutput)->Blt(pGraphOutput, Image.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 530, Image.Width, Image.Height, 0);

		switch (Tkey.UnicodeChar)
		{
		case CHAR_CARRIAGE_RETURN :
			//PrintString(pGraphOutput, &Image, 0, 0, SOLAR_WHITE, L" ENTER ");//for test
			MakeThickLine(pGraphOutput, Image, SelectX, SOLAR_BLACK);
			//get color and change
			Status = (pGraphOutput)->Blt(pGraphOutput, Image.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 530, Image.Width, Image.Height, 0);
			SColor = ConditionColor(SelectX);
			loop=0;
			*TabC=0;
			break;

		case CHAR_Q:
		case CHAR_q:
			MakeThickLine(pGraphOutput, Image, SelectX, SOLAR_BLACK);
			SColor = NowColor;
			*TabC=0;
			//PrintString(pGraphOutput, &Image, 100,  0, SOLAR_WHITE,L" Get New Color :%d", SColor);
			loop = 0;
			break;

		case CHAR_TAB:
			MakeThickLine(pGraphOutput, Image, SelectX, SOLAR_BLACK);//210628
			//PrintString(pGraphOutput, &Image, 435,  25, SOLAR_WHITE, L"___test___ "); //or 用判斷限制只能打六位數字
			*TabC=1; 	
			SColor = NowColor;//test
			loop=0;
			break;
		
		}
		//PrintString(pGraphOutput, &Image, 0,  0, SOLAR_WHITE, L"Try keyboard"); //try0521
			
	}
	return SColor;
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

	// Check the Solar graphics library is supported. #include"SolarGraphicsLib"
	if (InitiateSolarGraphicsLib(pGraph) != SOLAR_SUCCESS) {
		COut->OutputString(COut, L"\n>> SolarGraphicsLib is not ready.");
		return SOLAR_UNSUCCESS;
	}

	QueryRequestMode(*pGraph, HorizontalResolution, VerticalResolution, ModeNum);//Page "start boot option"

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
	UINT32				DesireColor = 0, TabC=0;
	UINT32				SelectedColor=SOLAR_YELLOW;//SelectedColor=0
	
	SOLAR_IMAGE_INPUT	DescripBar;
	SOLAR_IMAGE_INPUT	DrawingBoard; 
	SOLAR_IMAGE_INPUT	PaletteBar; 
	SOLAR_IMAGE_INPUT	AllviewBoard;
	SOLAR_IMAGE_INPUT   BitMapShow;

	DescripBar.BltBuffer=(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)NULL;
	DescripBar.Width=200; 
	DescripBar.Height=360; //400

	DrawingBoard.BltBuffer=(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)NULL; 
	DrawingBoard.Width=600; 
	DrawingBoard.Height=530; 

	PaletteBar.BltBuffer=(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)NULL;
	PaletteBar.Width=600; 
	PaletteBar.Height=70;

	AllviewBoard.BltBuffer=(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)NULL;
	AllviewBoard.Width=200;
	AllviewBoard.Height=200;

	BitMapShow.BltBuffer=(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)NULL;
	BitMapShow.Width=200;
	BitMapShow.Height=40;
	
	// Allocate a screen size memory for buffer.
	if ((DescripBar.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool(DescripBar.Width * DescripBar.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)))==NULL){
		return EFI_OUT_OF_RESOURCES;
	}

	if ((DrawingBoard.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool(DrawingBoard.Width * DrawingBoard.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)))==NULL){
		return EFI_OUT_OF_RESOURCES;
	} 

	if((PaletteBar.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool(PaletteBar.Width * PaletteBar.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)))==NULL){
		return EFI_OUT_OF_RESOURCES;
	}

	if((AllviewBoard.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool(AllviewBoard.Width * AllviewBoard.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)))==NULL){
		return EFI_OUT_OF_RESOURCES;
	}

	if((BitMapShow.BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) EfiLibAllocatePool(BitMapShow.Width * BitMapShow.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)))==NULL){
		return EFI_OUT_OF_RESOURCES;
	}

	//Drawline Demo
	// Allocate a screen size memory for buffer.
	if ((Image->BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)EfiLibAllocatePool(Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL))) == NULL) {
		return EFI_OUT_OF_RESOURCES;
	}

	EfiSetMem(Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);
	FillScreen(*pGraph, SOLAR_BLACK);
	PrintString(*pGraph, Image, 150, 290, SOLAR_WHITE, L"The Graphics Draw Game. Width = %d, Height = %d", Image->Width, Image->Height); 
	Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
	getch(&KeyCode);



	//Blt Try
	//BitMapShow
	EfiSetMem(BitMapShow.BltBuffer, BitMapShow.Height * BitMapShow.Width * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL),SOLAR_BLACK);
	FillScreen(*pGraph, SOLAR_YELLOW);//SOLAR_BLACK
	DrawRectangle(*pGraph, &BitMapShow,  150, 0,  178, 28, SOLAR_FLORALW, 0, Fill, 0, 3); //0, 0,  28, 28,


	//AllviewBoard
	EfiSetMem(AllviewBoard.BltBuffer, AllviewBoard.Height * AllviewBoard.Width * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL),0);
	FillScreen(*pGraph, SOLAR_YELLOW);//SOLAR_BLACK
	DrawHorizontalLine(*pGraph, &AllviewBoard, 0, 0, 5, SOLAR_WHITE, FullLine);
	DrawVerticalLine(*pGraph, &AllviewBoard, 0, 0, 5, SOLAR_WHITE, FullLine);

	DrawHorizontalLine(*pGraph, &AllviewBoard, 173, 0, 5, SOLAR_WHITE, FullLine);
	DrawVerticalLine(*pGraph, &AllviewBoard, 178, 0, 5, SOLAR_WHITE, FullLine);

	DrawHorizontalLine(*pGraph, &AllviewBoard, 0, 178, 5, SOLAR_WHITE, FullLine);
	DrawVerticalLine(*pGraph, &AllviewBoard, 0, 173, 5, SOLAR_WHITE, FullLine);

	DrawHorizontalLine(*pGraph, &AllviewBoard, 174, 178, 5, SOLAR_WHITE, FullLine);
	DrawVerticalLine(*pGraph, &AllviewBoard, 178, 174, 5, SOLAR_WHITE, FullLine);
	
	//PaletteBar
	EfiSetMem(PaletteBar.BltBuffer, PaletteBar.Height * PaletteBar.Width * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL),EFI_BLACK);
	FillScreen(*pGraph, SOLAR_YELLOW);//SOLAR_BLACK
	DrawRectangle(*pGraph, &PaletteBar,  30, 20,  30+COLOR_BLOCK_EDGE, 20+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, SOLAR_YELLOW, 3); 
	DrawRectangle(*pGraph, &PaletteBar,  60, 20,  60+COLOR_BLOCK_EDGE, 20+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, SOLAR_RED, 3); 
	DrawRectangle(*pGraph, &PaletteBar,  90, 20,  90+COLOR_BLOCK_EDGE, 20+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, SOLAR_BLUE, 3); 
	DrawRectangle(*pGraph, &PaletteBar, 120, 20, 120+COLOR_BLOCK_EDGE, 20+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, SOLAR_GREEN, 3); 
	DrawRectangle(*pGraph, &PaletteBar, 150, 20, 150+COLOR_BLOCK_EDGE, 20+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, SOLAR_FLORALW, 3);
	PrintString(*pGraph, &PaletteBar, 250,  25, SOLAR_WHITE, L"Please type HEX RGB color(6): "); //or 用判斷限制只能打六位數字
	PrintString(*pGraph, &PaletteBar, 425,  5, SOLAR_WHITE,L" NOW COLOR :"); // 400,  5, SOLAR_WHITE,L" BOUNDARY COLOR:"
	DrawRectangle(*pGraph, &PaletteBar, 530, 0, 530+COLOR_BLOCK_EDGE, 0+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, SelectedColor, 3);  
				
	//Select function Bar(之後可以跟畫框一起Show出來先把Function做好)
	EfiSetMem(DescripBar.BltBuffer, DescripBar.Width * DescripBar.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), 0);
	FillScreen(*pGraph, SOLAR_YELLOW);
	PrintString(*pGraph, &DescripBar, 0,  30, SOLAR_WHITE, L" 'Backspace' : Reset Board ");
	PrintString(*pGraph, &DescripBar, 0,  60, SOLAR_WHITE, L" 'SPACE' : Make Boundary ");//show不出來 鑽太久惹 650 20是800600的位置 要以Select的大小畫
	PrintString(*pGraph, &DescripBar, 0,  90, SOLAR_WHITE, L" 'Tab' : Choose or Type Color ");
	PrintString(*pGraph, &DescripBar, 0, 120, SOLAR_WHITE, L" 'Q' : Quit Choose or Type Bar ");
	PrintString(*pGraph, &DescripBar, 0, 150, SOLAR_WHITE, L" 'Enter' : Change Color ");
	PrintString(*pGraph, &DescripBar, 0, 180, SOLAR_WHITE, L" 'D' : Delete Single Block ");
	PrintString(*pGraph, &DescripBar, 0, 210, SOLAR_WHITE, L" 'F' : Fill Color ");
	PrintString(*pGraph, &DescripBar, 0, 240, SOLAR_WHITE, L" 'ESC' : End Draw Board ");

	EfiSetMem(DrawingBoard.BltBuffer, DrawingBoard.Width * DrawingBoard.Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_GREEN);
	FillScreen(*pGraph, SOLAR_BLACK); //SOLAR_YELLOW
	
	Status = (*pGraph)->Blt(*pGraph, BitMapShow.BltBuffer, EfiBltBufferToVideo, 0, 0, 600, 360, BitMapShow.Width, BitMapShow.Height, 0);  // 0, 0, 630, 530,
	Status = (*pGraph)->Blt(*pGraph, AllviewBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 600, 400, AllviewBoard.Width, AllviewBoard.Height, 0);  // 0, 0, 630, 530,
	Status = (*pGraph)->Blt(*pGraph, PaletteBar.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 530, PaletteBar.Width, PaletteBar.Height, 0); //跳視窗變成最上層0, 0, 0, 30
	Status = (*pGraph)->Blt(*pGraph, DescripBar.BltBuffer, EfiBltBufferToVideo, 0, 0, 600, 0, DescripBar.Width, DescripBar.Height, 0); // 0, 0, 630, 0
	//Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); //先畫背景

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
				DrawRectangle(*pGraph, &DrawingBoard, X, Y, X+COLOR_BLOCK_EDGE, Y+COLOR_BLOCK_EDGE, SOLAR_RED, 0, Fill, SelectedColor, 3);  
				LX = (X - COLOR_BOARD_X1) / COLOR_BLOCK_EDGE;
				LY = (Y - COLOR_BOARD_Y1) / COLOR_BLOCK_EDGE;
				if(SelectedColor==SOLAR_YELLOW)
				{
					Locate[LX][LY] = SOLAR_YELLOW;
				}
				else
				{
					Locate[LX][LY] = SelectedColor;
				}
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
						Locate[j][k] = 0; //之後抓陣列可能要確認這裡的數值0510
					}
				}
				break;

		//復原成上一個樣子
		case CHAR_R: 
		case CHAR_r:
				memcpy(&RecAry,&Locate,sizeof(Locate));//211015G function過 =>變現流
				//make store action
			/*
				for(UINT16 j=0; j<25 ; j++)
				{
					for(UINT16 k=0; k<25 ; k++)
					{
						UINT16 CX=k+6*k+2; //CX=k+6*k+1
						UINT16 CY=j+6*j+2;
						DrawRectangle(*pGraph, &AllviewBoard, CX, CY, CX+6, CY+6, RecAry[k][j], 0, Fill, RecAry[k][j], 3);  
					}
				}
				Status = (*pGraph)->Blt(*pGraph, AllviewBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 600, 400, AllviewBoard.Width, AllviewBoard.Height, 0);  //G
			*/
				break;				
/*
		//for test 
		case CHAR_T:
		case CHAR_t:
				break;
*/
		//倒色 SOLAR_DEEPPINK
		case CHAR_F:
		case CHAR_f: 
				LX = (X - COLOR_BOARD_X1) / COLOR_BLOCK_EDGE;
				LY = (Y - COLOR_BOARD_Y1) / COLOR_BLOCK_EDGE;
				DesireColor = Locate[LX][LY]; 
				//choise the block position color to decide same situation to fill the same color
				FillRecursion(*pGraph, DrawingBoard, LX, LY, SelectedColor, DesireColor);
				break;

		case CHAR_TAB :
				DrawBoundary(*pGraph, DrawingBoard, X, Y, COLOR_BLOCK_EDGE, SOLAR_WHITE);
				Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); //T
				MakeThickLine(*pGraph, PaletteBar, SELECT_STARTX_WIDTH, SOLAR_RED);
				Status = (*pGraph)->Blt(*pGraph,PaletteBar.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 530, PaletteBar.Width, PaletteBar.Height, 0);
				//PrintString(*pGraph, &PaletteBar, 350,  0, SOLAR_WHITE,L" Locate VAL :%x", Locate[LX][LY]);
				//SelectedColor=SelectShow(*pGraph, PaletteBar, SELECT_X_COORDINATE, KeyCode, SelectedColor );
				SelectedColor=SelectShow(*pGraph, PaletteBar, SELECT_X_COORDINATE, KeyCode, SelectedColor, &TabC);
				//PrintString(*pGraph, &PaletteBar, 300,  0, SOLAR_WHITE,L" TabC VAL :%x", TabC);
				DrawRectangle(*pGraph, &PaletteBar, 530, 0, 530+COLOR_BLOCK_EDGE, 0+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, SelectedColor, 3);  
				Status = (*pGraph)->Blt(*pGraph,PaletteBar.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 530, PaletteBar.Width, PaletteBar.Height, 0);
				
				if(TabC==1)
				{
					PrintString(*pGraph, &PaletteBar, 435,  25, SOLAR_WHITE, L"#________"); 
					Status = (*pGraph)->Blt(*pGraph,PaletteBar.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 530, PaletteBar.Width, PaletteBar.Height, 0);
					SelectedColor=TypeColor(*pGraph, PaletteBar, KeyCode, SelectedColor);
				 	DrawRectangle(*pGraph, &PaletteBar, 530, 0, 530+COLOR_BLOCK_EDGE, 0+COLOR_BLOCK_EDGE, SOLAR_WHITE, 0, Fill, SelectedColor, 3);//Type now color show
				 	Status = (*pGraph)->Blt(*pGraph,PaletteBar.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 530, PaletteBar.Width, PaletteBar.Height, 0);
				}

				//Type color erase
				DrawRectangle(*pGraph, &PaletteBar, 445, 17, 513, 35, SOLAR_BLACK, 0, Fill, SOLAR_BLACK, 3); //SOLAR_LIME
				//PrintString(*pGraph, &PaletteBar, 435,  25, SOLAR_WHITE, L"#________");
				DrawRectangle(*pGraph, &PaletteBar, 513, 22, 580, 40, SOLAR_BLACK, 0, Fill, SOLAR_BLACK, 3);//type text erase but show timing need confirm SOLAR_PURPLE
				
				DrawBoundary(*pGraph, DrawingBoard, X, Y, COLOR_BLOCK_EDGE, SOLAR_RED);
				Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); 
				break;

		}	
		Status = (*pGraph)->Blt(*pGraph, DrawingBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, DrawingBoard.Width, DrawingBoard.Height, 0); 

		//AllView Function
		for(UINT16 j=0; j<25 ; j++)
		{
			for(UINT16 k=0; k<25 ; k++)
			{
				UINT16 CX=k+6*k+2; //CX=k+6*k+1
				UINT16 CY=j+6*j+2;
				DrawRectangle(*pGraph, &AllviewBoard, CX, CY, CX+6, CY+6, Locate[k][j], 0, Fill, Locate[k][j], 3);  
			}
		}
		Status = (*pGraph)->Blt(*pGraph, AllviewBoard.BltBuffer, EfiBltBufferToVideo, 0, 0, 600, 400, AllviewBoard.Width, AllviewBoard.Height, 0);  //G

        //Bitmap
		for(UINT16 j=0; j<25 ; j++)
		{
			for(UINT16 k=0;k<25;k++)
			{
				UINT16 Lk=k+152;
				UINT16 Lj=j+2;
				DrawPixel(*pGraph, &BitMapShow, Lk,Lj,Locate[k][j]);
			}
		}
		Status = (*pGraph)->Blt(*pGraph, BitMapShow.BltBuffer, EfiBltBufferToVideo, 0, 0, 600, 360, BitMapShow.Width, BitMapShow.Height, 0);  //G

	}

	gBS->FreePool(Image->BltBuffer);
	gBS->FreePool(DrawingBoard.BltBuffer);
	gBS->FreePool(DescripBar.BltBuffer);
	gBS->FreePool(PaletteBar.BltBuffer); 
	gBS->FreePool(AllviewBoard.BltBuffer);
	gBS->FreePool(BitMapShow.BltBuffer);

	return SOLAR_SUCCESS;
}



SOLAR_STATUS
SOLAR_API
EndShow(
	EFI_GRAPHICS_OUTPUT_PROTOCOL    **pGraph,
	SOLAR_IMAGE_INPUT    			*Image //maybe need to add free storage
)
{
	EFI_INPUT_KEY			KeyCode;
	EFI_STATUS    		Status;
	//UINTN         BmpWidth;
	//UINTN         BmpHeight;
	
	EfiSetMem (Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);
	FillScreen(*pGraph, SOLAR_BLACK);
	//BmpFileToSolarImage(*pGraph, L"images.bmp", Image, &BmpWidth, &BmpHeight);
	PrintString (*pGraph, Image, 285, 290, SOLAR_WHITE, L"Thank for your play");
  	PrintString (*pGraph, Image, 410, 290, SOLAR_PURPLE, L"DRAW-GAME.");
  	Status = (*pGraph)->Blt ( *pGraph, Image->BltBuffer,EfiBltBufferToVideo,0,0,0,0,Image->Width,Image->Height,0);
  	getch (&KeyCode);

	/*
	//BMP Demo
	EfiSetMem(Image->BltBuffer, Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), EFI_BLACK);
	FillScreen(*pGraph, SOLAR_BLACK);
	BmpFileToSolarImage(*pGraph, L"111.bmp", Image, &BmpWidth, &BmpHeight);
	DrawRectangle(*pGraph, Image, 261, 118, 294, 148, SOLAR_YELLOW, 0, Fill, SOLAR_YELLOW, 3); // (x1,y1,x2,y2)=(261,118,294,148)
	DrawRectangle(*pGraph, Image, 360, 245, 393, 275, SOLAR_WHITE, 0, Fill, SOLAR_WHITE, 3); //(x1,y1,x2,y2)=(360,245,393,275)
	Status = (*pGraph)->Blt(*pGraph, Image->BltBuffer, EfiBltBufferToVideo, 0, 0, 0, 0, Image->Width, Image->Height, 0);
	
	getch(&KeyCode);//跳太快惹想找Delay
    */

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
  EndShow(&pGraph, &DesktopImage);
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
