#pragma once
#include <stdint.h>
#include "def.h"
#include "demos.h"


struct Gui
{
	Demos demos;
	Ada4_ST7735* d=0;

	//  main  ----
	Gui();
	void Init(Ada4_ST7735* tft);
	void Draw(), DrawEnd();
	void KeyPress();


	//  draw menus
	void DrawFans(), DrawKeys(), DrawMatrix(), DrawDisplay();  // set params
	void DrawGraphs(), DrawGraph(), DrawHelp(), DrawConfig();  // info

	//  draw util
	void Chk_y1(), DrawOperInfo();
	void DrawDispCur(int i, int16_t y);
	//  util
	void ClrTemp(int temp);
	void PrintInterval(uint32_t t);
	int TempFtoB(float t);  float TempBtoF(uint8_t b);


	//  keys
	void KeysFans(), KeysScan(), KeysDisplay();
	void KeysConfig(), KeysGraph();

	//  start
	void SetScreen(int8_t start);
	const char* StrScreen(int8_t s);


	//  fade color menu  ---
	enum EFadeClr
	{	C_Main=0, C_Demos, C_Test, 
		C_Setup, C_Disp, C_Clock, C_Setup2,
		C_Info, C_ALL  };
	const static uint8_t
		Mclr[C_ALL][2][3];

	void FadeClr(EFadeClr ec, const uint8_t minRGB, const uint8_t mul, const uint8_t div);
	void DrawMenu(int cnt, const char** str, EFadeClr ec, uint16_t curClr,
		uint16_t bckClr, int16_t yadd=10, int16_t nextCol=-1, int16_t numGap=-1);


	//  vars  ---
	int8_t mlevel = 0;  // 0 main, 1 level1, 2 level2

	int8_t ym = 0;      // 0 main y cursor
	int8_t ym1[M_All];  // 1 y cursor for all main menu entries
	int8_t yy = 0;      // = ym1[ym]  level1 y cursor

	//  time, key repeat
	uint32_t oldti=0, oldti_kr=0;
	int8_t kr(uint8_t sc, uint16_t dt);
	int8_t iRam = 0;  // ram info

	//  help
	int8_t hpage = 0;
	const static int8_t HAll = 2;
	#ifdef LED
	int8_t led = 0;
	#endif


	//  keys pressed, some +-1  _k_
	int8_t kRight=0, kUp=0,  kPgUp=0, kEnd=0,  kEnt=0, kBack=0;


	//  level 2 y cursors  - - -
	int8_t ym2Fan = 0, ym2Scan = 0;  // Setup
	int8_t ym2Disp = 0, pgDisp = 0;  // Display
	int8_t ym2Clock = 0, pgClock = G_Stats;  // Clock

	const static uint8_t
		DispPages[Di_All]; //, ScanPages[S_All];

	//  util
	int16_t RangeAdd(int16_t val, int16_t add, int16_t vmin, int16_t vmax, int8_t cycle=0);
	void Save(), Load(int8_t reset);


	//  Sequences  - - - -
	int8_t edit = 0;   // seq 0 view / 1 edit
	int8_t edins = 1;  // 1 ins 0 overwrite
	int8_t copyId = -1;  // copy/swap from

	int8_t slot=0, page=0;  int16_t edpos=0;  // edit vars
	void slotMax(),slotMin(),pageInc(),pageDec();
	int seqId()
	{	return slot + page*iPage;  }

	int8_t tBlnk = 0;
	int16_t tInfo=0;  int8_t infType=0;  // info text vars
	const int8_t iPage = 10, cBlnk = 25;  // par-


	//  Temp 'C  ---
#ifdef TEMP1
	float fTemp = -90.f;  // cur value
	int8_t temp1 = 1;     // fist, init
	//  last time read'C, add to graph
	uint32_t msTemp = 0, msTempGr = 0;
	void GetTemp();
#endif

	uint8_t grTemp[W];    // graph array
	uint8_t grTpos = 0;   // write pos
	// auto range
	uint8_t grTempUpd = 1;  // update
	uint8_t grFmin = 17, grFmax = 35;  // temp 'C
	uint8_t grBmin = 0, grBmax = 255;  // val Byte

};

