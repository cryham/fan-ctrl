#pragma once
#include "def.h"


//  const  ----
//  max rpm avgeraging length
const static int avgMax = 16;

//  for naming fans
const static int FNames_All = 13;
extern const char* fanNames[FNames_All];

enum FanMode
{
	FM_Hide, FM_Off, FM_On, FM_ExtOn, FM_ExtOff, FModes_All
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
	
	int8_t tempId = -1;  // sensor id  -1 none  for Auto
	uint8_t avgNum = 4;    // rpm values count for avgeraging  1..avgMax

	struct RpmGuard
	{
		uint8_t on = 1;
		uint16_t rpmMin = 100;  // if below triggers
		//  ms time  for max PWM to turn on, at start and after sudden stop
		uint16_t msOn = 300;    // triggered time and rpm
		uint16_t pwmOn = 2000;  // 4095 max
	} g;

	struct PwmAuto
	{
		uint8_t on = 0;
		uint16_t pwmMin = 1000, pwmMax = 4000;
		uint16_t tempMin = 350, tempMax = 650;  // /10.f 'C
		uint8_t exp = 100;  // power exponent /100.f
	} a;
};


struct Fan
{
	FanData fd;

	bool hasRpm = true;  // if fan has rpm pin to measure

	//  var, measure
	uint32_t lastRpmMS = 0;

	volatile uint8_t oldPin = 0;
	volatile uint16_t pulses = 0;
	
	uint16_t rpm = 0, rpmAvg = 0;  //  on Gui
	static inline uint8_t Rpm2B(uint16_t r)
	{	return r / 5;  }  // byte  max 1275 rpm

	//  average rpm
	int8_t avgCnt = 0;
	uint16_t avgArr[avgMax] = {0,};


	//  time max pwm to start
	bool on = 0, oldOn = 0;
	int32_t tmLeft = 0;  // guard on, time left
	uint16_t pwm = 0;  // real pwm for vis


	Fan();
	void Init(uint8_t fanId);

	void GetFanName(char* s) const;

	void CalcRPM();
	bool GetOn(bool ext_on) const;
	uint16_t GetPWM(float* fTemp) const;
	uint16_t GetPWMAuto(float fTemp) const;
	void Guard(uint32_t dt, uint16_t& pwm);
};


struct Fans
{
	uint32_t prevMS = 0;
	int onFans = 0;  // active
	
	bool ext_on = false;  // external on/off pin state

	Fan fan[NumFans];
	float* fTemp = nullptr;  // [MaxTemp]


	Fans();
	void Init(float* pfTemp);

	void CheckRPM();
	void Update();  // all
	
	void Update(uint8_t i, uint32_t dt);  // 1 fan
};
