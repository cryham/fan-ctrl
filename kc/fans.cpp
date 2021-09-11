#include "fans.h"
#include "WProgram.h"


//  pins
const uint8_t FAN_PWM[NumFans] = { 3,4,5,6, 22,21,20 };  // PWM outut pin
const uint8_t FAN_RPM[NumFans] = { 0,1,2,7, NoRPM,NoRPM,NoRPM };  // RPM input pin


//  Init  -----------
Fan::Fan()
{
	lastRpmMS = millis();
}
Fans::Fans()
{
	prevMS = millis();
}

//  ISRs meh
extern Fans fans;

void Pulse0() {  ++fans.fan[0].pulses;  }
void Pulse1() {  ++fans.fan[1].pulses;  }
void Pulse2() {  ++fans.fan[2].pulses;  }
void Pulse3() {  ++fans.fan[3].pulses;  }
void Pulse4() {  ++fans.fan[4].pulses;  }
void Pulse5() {  ++fans.fan[5].pulses;  }
void Pulse6() {  ++fans.fan[6].pulses;  }
void (*PulseArr[NumFans])() = {
	Pulse0,Pulse1,Pulse2,Pulse3,Pulse4,Pulse5,Pulse6};


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
	attachInterrupt(digitalPinToInterrupt(rpm), PulseArr[fanId], FALLING);
}

void Fans::Init()
{
	for (int i=0; i < NumFans; ++i)
		fan[i].Setup(i);
}


//  Update  -----------
void Fan::CalcRPM()
{
	noInterrupts();
	if (pulses == 0)
		rpm = 0;
	else
	{	float dtMS = (millis() - lastRpmMS) / 1000.f;
		float revPerMS = float(pulses) / 2.f / dtMS;
		rpm = revPerMS * 60.f;  //?
	}
	lastRpmMS = millis();
	pulses = 0;
	interrupts();

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

//  change
void Fans::SetPWM(uint8_t i, uint16_t val)
{
	analogWrite(FAN_PWM[i], val); //bri * 40 + 40);
}
