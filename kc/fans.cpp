#include "fans.h"
#include "WProgram.h"


//  pins
const uint8_t FAN_PWM[NumFans] = { 20,22,6,4, 21,5,3 };  // PWM outut pin
const uint8_t FAN_RPM[NumFans] = {  1, 2,7,0, 24,25,19 };  // RPM input pin


//  Init  -----------
Fan::Fan()
{
	lastRpmMS = millis();
}
Fans::Fans()
{
	prevMS = millis();
}


//  Setup pins  -----------
void Fan::Setup(uint8_t fanId)
{
	const uint8_t pwm = FAN_PWM[fanId], rpm = FAN_RPM[fanId];
	pinMode(pwm, OUTPUT);
	analogWriteFrequency(pwm, 10000);  // 10k
	analogWrite(pwm, 0);  // off

	noRpm = rpm >= NoRPM;
	if (noRpm)
		return;

	pinMode(rpm, INPUT_PULLUP);
}

void Fans::Init()
{
	for (int i=0; i < NumFans; ++i)
		fan[i].Setup(i);
}


//  Update  -----------
void Fans::Check()
{
	for (int i=0; i < NumFans; ++i)
	{
		const uint8_t rpm = FAN_RPM[i];
		auto& f = fan[i];
		uint8_t pin = digitalRead(rpm);
		
		if (!pin && f.oldPin)  // falling edge
			++f.pulses;
		f.oldPin = pin;
	}
}

void Fan::CalcRPM()
{
	auto ms = millis();
	if (pulses == 0)
		rpm = 0;
	else
	{	float dtMS = (ms - lastRpmMS) / 1000.f;
		float revPerMS = float(pulses) / 2.f / dtMS;
		rpm = revPerMS * 60.f;
	}
	lastRpmMS = ms;
	pulses = 0;

	//  add
	avgArr[avgCnt] = rpm;
	++avgCnt;
	if (avgCnt >= avgNum)
		avgCnt = 0;

	//  get avg
	int sum = 0;
	for (int i = 0; i < avgNum; ++i)
		sum += avgArr[i];

	rpmAvg = sum / avgNum;
}


void Fans::Update()
{
	if (millis() - prevMS > updateMS)
	{
		for (int i=0; i < NumFans; ++i)
			if (!fan[i].noRpm)
				fan[i].CalcRPM();

		prevMS = millis();  
	}
}

//  Change
void Fans::SetPWM(uint8_t i, uint16_t val)
{
	//fan[i].pwm = val;
	analogWrite(FAN_PWM[i], val);
}
