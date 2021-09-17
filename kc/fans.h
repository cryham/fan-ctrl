#pragma once
#include "def.h"

// params  rpm avgeraging length
const static int avgNum = 4;
// ms time for max PWM to turn on at start and after stop
const static int msMax = 300;  // ?


struct FanData
{
	//  data saved in eeprom
	uint8_t off = 0;
	int16_t pwm = 0;     // 0 off - 4095 full speed 12V
};

struct Fan
{
	FanData fd;

	bool noRpm = false;  // if no rpm pin to measure

	//  var, measure
	uint32_t lastRpmMS = 0;

	volatile uint8_t oldPin = 0;
	volatile uint16_t pulses = 0;
	
	uint16_t rpm = 0, rpmAvg = 0;

	int8_t avgCnt = 0;
	uint16_t avgArr[avgNum] = {0,};


	bool oldOn = 0;
	int32_t tmMax = 0;  // time max pwm to start


	Fan();

	void Setup(uint8_t fanId);

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
