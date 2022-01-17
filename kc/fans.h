#pragma once
#include "def.h"

//  const  ----
//  params  rpm avgeraging length
const static int avgNum = 4;
//  ms time  for max PWM to turn on, at start and after (sudden) stop
const static int msMax = 300;  // ?

//  for naming fans
const static int FNames_All = 13;
extern const char* fanNames[FNames_All];

enum FanMode
{
	FM_Hide, FM_Off, FM_On, FM_ExtOn, FModes_All
};
extern const char* fanModes[FModes_All];


//  data, saved in eeprom  ----
struct FanData
{
	uint8_t mode = 0;  // FanMode
	int16_t pwm = 0;   // 0 off .. 4095 full speed 12V
		// either fan PWM or Analog voltage, after RC filter
	
	//  for lcd, info
	uint8_t name = 0;  // fanNames
	uint8_t number = 0;
	
	int8_t temp = -1;  // sensor id  -1 none  for auto adj

	//uint16_t rpmMin, rpmMax;  // todo: auto rpm
	//uint16_t tempMin, tempMax;
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

	void GetFanName(char* s) const;

	void CalcRPM();
};


struct Fans
{
	uint32_t prevMS = 0;
	
	bool ext_on = false;  // external on/off pin state

	Fan fan[NumFans];


	Fans();
	void Init();

	void Check();
	void Update();
	void Update(uint8_t i, uint32_t dt);
};
