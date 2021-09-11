#include "def.h"


const static int avgNum = 4;  // par avg len


struct Fan
{
	int lastRpmMS = 0;

	volatile int pulses = 0;
	int16_t rpm = 0, rpmAvg = 0;
	int16_t pwm = 0;

	int8_t avgCnt = 0;
	int16_t avgArr[avgNum] = {0,};
	bool noRpm = false;

	Fan();

	void Setup(uint8_t fanId);

	void CalcRPM();
};


struct Fans
{
	int updateMS = 500;  // ms, 2Hz refresh
	int prevMS = 0;

	Fan fan[NumFans];

	Fans();

	void Init();

	void Update();

	void SetPWM(uint8_t fan, uint16_t val);
};
