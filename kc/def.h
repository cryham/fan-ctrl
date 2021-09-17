#pragma once
#include <stdint.h>

//----  Setup  ----	  use:  ram B      flash
//  optional features, comment out to disable
#define DEMOS			//  28        9%
#define DEMOS_PLASMA	//  36        11%
#define DEMOS_3D		//  44        15%


//-----------------
//  features, pins
#define LCD_LED  23  // pwm lcd brightness

#define NumFans  7  // count
//  fan pins
extern const uint8_t FAN_PWM[NumFans];  // PWM outut pins  in fans.cpp
extern const uint8_t FAN_RPM[NumFans];  // RPM input pins  use NoRPM if not present
#define NoRPM  99

//  led light
//#define LED  12
//  temperature 'C sensor DS18B20
//#define TEMP1  31	//  44   9%  24k

//  Keyboard pins in matrix.h
//-----------------


#define W 160  //  display dim
#define H 128

//  R F800  G 07E0  B 001F  R 32 G 64 B 32  565
#define RGB(r,g,b)  ( ((r)<<11)+ ((g)<<6) +(b))   // 31 31 31
#define RGB2(r,g,b) ( ((r)<<11)+ ((g)<<5) +(b))  // 31 63 31

#ifdef __cplusplus
class Ada4_ST7735;
#endif


//  main menu entries, level 0  ---
enum EMainMenu  
{
	M_Fans, M_Config, M_Graphs,
	M_Display, M_Keys, M_Help,
	#ifdef DEMOS
		M_Demos,
	#endif
	M_All,  M_Next = M_Display  // 2nd column, -1 off
};

//  menus  level 1  --
enum EConfig
{
	C_Save, C_Load, C_Use, C_Version, C_All
};
enum EGraphs
{
	G_Stats, G_Graphs, G_All
};
enum EDisplay  // pages
{
	Di_Bright, Di_Key, Di_Stats, Di_Graph, Di_Debug, Di_All
};
enum EKeys
{
	K_Matrix, K_Scan, K_All
};


#ifdef DEMOS
enum EDemo  //  Demos, level1
{
	#ifdef DEMOS_PLASMA
		D_Plasma,
	#endif
		D_Wave, D_Fire,
	#ifdef DEMOS_3D
		D_Hedrons,
	#endif
		D_CK_Logo,
	D_Rain, D_Ngons, D_Fonts,
	D_All,  D_Next = D_CK_Logo
};
extern const char *strDemo[D_All];
#endif


//  string names for all above ^
extern const char
	*strMain[M_All], *strKeys[K_All], *strConfig[C_All];

//  sub page counts, inside each main menu entry
extern const uint8_t YM1[M_All];


//  time intervals  *0.1s
extern const uint16_t gIntervals[];
const static uint8_t  gIntvMask = 0x1F;
