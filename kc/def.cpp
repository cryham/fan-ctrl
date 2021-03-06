#include "def.h"
#include "gui.h"


//  Main Menu
const char* strMain[M_All] =
{
	"Fans", "Config", "Graphs",
	"Display", "Keys", "Help",
#ifdef DEMOS
	"Demos",
#endif
};


//  Setup kbd
const char* strKeys[K_All] =
{
	"Matrix #", "Scan #",
};

//  Info use, ver
const char* strConfig[C_All] =
{
	"Save", "Load", "Counter", "Version",
};


//  Demos
#ifdef DEMOS
const char* strDemo[D_All] =
{
#ifdef DEMOS_PLASMA
	"Plasma",
#endif
	"Wave", "Fire",
#ifdef DEMOS_3D
	"Hedrons",
#endif
	"CK Logo",
	"Rain",
	"N-gons", "Fonts",
};
#endif


//  sub pages counts
const uint8_t YM1[M_All] =
{
	1,     // M_Fans
	C_All, // M_Config
	1,     // M_Graphs

	1,     // M_Display
	K_All, // M_Keys
	1,     // M_Help
#ifdef DEMOS
	D_All, // M_Demos
#endif
};


//  time intervals *0.1s
const uint16_t gIntervals[gIntvMask+1] =
{
	0, 1 /*100ms 1*/, 2, 4, 6, 8,
	10   /*1s 6*/   ,20,40,60,80,
	100  /*10s 11*/ ,200,300,450,
	600  /*1m 15*/  ,720,900,
	1200 /*2m 18*/  ,2100,3000,4500,
	6000 /*10m 22*/ ,7200,9000,
	12000/*20m 25*/ ,15000,18000,24000,
	36000/*1h 29*/  ,45000,
	54000/*1.5h 31*/
};

//  menu colors
const uint8_t Gui::Mclr[Gui::C_ALL][2][3] =
{
	{{20,26,31},{5,3,1}},  // main
	{{27,26,31},{2,3,1}},  // demos
	
	{{25,25,30},{3,2,1}},  // fans
	{{27,30,30},{3,3,1}},  // fan details

	{{24,27,27},{5,4,3}},  // keys
	{{26,28,28},{4,3,3}},  // scan
	
	{{31,31,16},{3,5,5}},  // display
	{{26,26,28},{4,3,2}},  // config
};
