#include "def.h"


const static int avgNum = 4;  // par  avgeraging length


struct Fan
{
	bool noRpm = false;  // if no rpm pin to measure
	int16_t pwm = 0;     // 0 off - 4095 full speed 12V

	uint32_t lastRpmMS = 0;

	volatile uint8_t oldPin = 0;
	volatile uint16_t pulses = 0;
	
	uint16_t rpm = 0, rpmAvg = 0;

	int8_t avgCnt = 0;
	uint16_t avgArr[avgNum] = {0,};


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

	void SetPWM(uint8_t fan, uint16_t val);
};
