#pragma once
#include "def.h"

//  const
//  params  rpm avgeraging length
const static int avgNum = 4;
//  ms time  for max PWM to turn on, at start and after (sudden) stop
const static int msMax = 300;  // ?


const static int FN_All = 11;
extern const char *fanNames[FN_All];

enum FanMode
{
	FM_Hide, FM_Off, FM_On, /*FM_Auto,*/ FM_All
};
extern const char *fanModes[FM_All];


//  data, saved in eeprom
struct FanData
{
	uint8_t mode = 0;  // FanMode
	int16_t pwm = 0;   // 0 off .. 4095 full speed 12V
	//  for lcd, info
	uint8_t name = 0;  // fanNames
	uint8_t number = 0;
};

struct Fan
{
	FanData fd;

	bool noRpm = false;  // if no rpm pin to measure

	//  var, measure
	uint32_t lastRpmMS = 0;

	volatile uint8_t oldPin = 0;
	volatile uint16_t pulses = 0;
	
	uint16_t rpm = 0, rpmAvg = 0;  //  on Gui

	//  average rpm
	int8_t avgCnt = 0;
	uint16_t avgArr[avgNum] = {0,};


	//  time max pwm to start
	bool oldOn = 0;
	int32_t tmMax = 0;


	Fan();
	void Init(uint8_t fanId);

	void CalcRPM();
};


struct Fans
{
	uint32_t updateMS = 500;  // ms, 2Hz refresh
	uint32_t prevMS = 0;

	Fan fan[NumFans];


	Fans();
	void Init();

	void Check();
	void Update();
	void Update(uint8_t i, uint32_t dt);
};
