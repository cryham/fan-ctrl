#pragma once
#include <stdint.h>
#include "def.h"
#include "demos.h"
struct Fan;


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
	void DrawKeys(), DrawMatrix(), DrawDisplay();  // set params
	void DrawHelp(), DrawConfig();  // info

	//  draw util
	void Chk_y1(), DrawOperInfo();
	void DrawDispCur(int i, int16_t y);
	//  util
	void ClrByte(int val);
	void PrintInterval(uint32_t t);

	void DrawGraphTi(int16_t xMin, int16_t xMax, int16_t yMin, int16_t yMax, bool legend, uint8_t tempId=0);
	void DrawGraphT(int16_t xMin, int16_t xMax, int16_t yMin, int16_t yMax, bool legend, uint8_t fanId=0);
	void DrawGraphR(int16_t xMin, int16_t xMax, int16_t yMin, int16_t yMax, bool legend, uint8_t fanId=0);
	void DrawGraph(int16_t xMin, int16_t xMax, int16_t yMin, int16_t yMax,  // where
		bool temp, bool legend=true, int id=0);  // what
	void DrawAutoGraph(const Fan* f);


	//  fans  ***
	const static int NumFanDet = 4;
	void DrawFans(), DrawFanDetails(), RpmClr(const Fan* f);
	void DrawGraphs();
	void KeysFans();


	//  keys
	void KeysScan(), KeysDisplay();
	void KeysConfig(), KeysGraph(int d);

	//  start
	void SetScreen(int8_t start);
	void StrScreen(int8_t st, char* str);


	//  fade color menu  ---
	enum EFadeClr
	{	C_Main, C_Demos,  C_Fans, C_FanDet,
		C_Keys, C_Scan,  C_Disp, C_Config, C_ALL  };
	const static uint8_t Mclr[C_ALL][2][3];

	void FadeClr(EFadeClr ec, const uint8_t minRGB, const uint8_t mul, const uint8_t div);
	void DrawMenu(int cnt, const char** str, EFadeClr ec, uint16_t curClr,
		uint16_t bckClr, int16_t yadd=10, int16_t nextCol=-1, int16_t numGap=-1);


	//  menu vars  ---
	int8_t mlevel = 0;  // 0 main, 1 level1, 2 level2

	int8_t ym = 0;      // 0 main y cursor
	int8_t ym1[M_All];  // 1 y cursor for all main menu entries
	int8_t yy = 0;      // = ym1[ym]  level1 y cursor

	//  time, key repeat
	uint32_t oldti=0, oldti_kr=0;
	int8_t kr(uint8_t sc, uint16_t dt);

	//  help
	int8_t hpage = 0;
	const static int8_t HAll = 2;


	//  gui keys pressed, some +-1
	int8_t kRight=0, kUp=0, kPgUp=0,  kEnt=0, kBack=0, kQuick=0;
	
	//  fan pwm% add speeds
	const static int8_t NumFanAdd = 17;
	const static uint16_t tFanAdd[NumFanAdd];


	//  level 2  menu cursors  ---
	enum EFanDet  // fan details pages
	{	FD_PowerRpm, FD_NameTemp, FD_Auto, FD_Guard, FD_Advanced, FD_Graphs, FanDetPages  };

	int8_t ym2Fan = 0;
	int8_t pgDet = 0, yFanDet[FanDetPages] = {0,};
	static int8_t FanDetLines[FanDetPages], FanDetLinExt[FanDetPages];

	int8_t ym2Disp = 0, pgDisp = 0, ym2Scan = 0;
	//int8_t pgGraph = G_Temp;

	const static uint8_t DispPages[Di_All];

	//  util
	int16_t RangeAdd(int16_t val, int16_t add, int16_t vmin, int16_t vmax, int8_t cycle=0);
	void Save(), Load(int8_t reset);

	int16_t tInfo=0;  int8_t infType=0;  // info text vars


	//  Temp 'C  ------------
	//  sensors
	typedef uint8_t TempAddr[8];
	TempAddr addr[MaxTemp] = {0,};  // sensor address one wire

	enum TempInit
	{  TI_FAIL, TI_SEARCH, TI_DONE, TI_READ, TI_ALL  }
	tempInit = TI_SEARCH;  // first, init
	
	int tempCount = 0;   // count
	bool TIdOk(int id)
	{	return id >= 0 && id < tempCount && id < MaxTemp;  }

	void GetTemp();
	void ResetTemp();  // search sensors again
	void AddGraphRpm(uint32_t ms);


	//  Graph var  ------------
	//  cur temp 'C values
	float fTemp[MaxTemp];

	//  last time read'C, add to graph
	uint32_t msTemp = 0, msTempGr = 0;
	uint16_t grTpos = 0;  // write pos  ----
	uint8_t grTemp[MaxTemp][W];   // graph arrays,  all sensor, screen

	//  temp  full to byte
	int TempFtoB(float t);  float TempBtoF(uint8_t b);
	//  auto range T
	uint8_t grTempUpd[MaxTemp];  // update
	uint8_t grFmin[MaxTemp], grFmax[MaxTemp];  // temp 'C
	uint8_t grBmin[MaxTemp], grBmax[MaxTemp];  // val Byte
	void AutoRangeTemp(int d);

	//  Rpm  ------------
	uint32_t msRpm = 0;  // rpm dt time
	uint8_t grRpos = 0;  // write pos  ----
	uint8_t grRpm[NumFans][W];  // graph array,  all fans

	//  rpm  full to byte
	uint8_t RpmFtoB(uint16_t r);  uint16_t RpmBtoF(uint8_t b);
	//  auto range Rpm
	uint8_t grRpmUpd[NumFans];  // update
	uint16_t grRFmin[NumFans], grRFmax[NumFans];  // rpm full
	uint8_t grRBmin[NumFans], grRBmax[NumFans];  // val Byte
	void AutoRangeRpm(int d);
};
