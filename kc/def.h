#pragma once
#include <stdint.h>

//----  Setup  ----
//  optional features, comment out to disable
#define DEMOS			//  9%  flash used
#define DEMOS_PLASMA	//  11%
#define DEMOS_3D		//  15%

//----------------------------------
//----  Features, pins  ----

//  Keyboard pins 3x3 in matrix.h
#define LCD_LED  23  // LCD PWM brightness


#define NumFans  7  // Fans Count
#define RpmUpdateMS  500  // ms, 2Hz refresh
//  Fan pins
extern const uint8_t FAN_PWM[NumFans];  // PWM outut pins  in fans.cpp
extern const uint8_t FAN_RPM[NumFans];  // RPM input pins
#define NoRPM  99   // use if fan has no rpm pin


//  Temperature 'C sensor(s)  DS18B20, one wire bus
#define TEMP_PIN  18	//  9%  24k  flash used
#define MaxTemp   4		// max sensors count


#define EXT_ON  26		// external on/off pin for fans
//#define LED  14   // led light-

//----  Used pins:  ----
//  Fans 7*2 + Keys 3+3 + Lcd SPI 5+1 led + 1 T'C
//  = 14 + 6 + 6 + 1 = 27  (all T3 pins = 35)

//----  Free pins:
//  14,15,16,17, 12?, a12 dac, 27

//----------------------------------
//----  End of Setup  ----


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
