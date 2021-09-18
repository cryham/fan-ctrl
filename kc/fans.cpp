#include "fans.h"
#include "WProgram.h"


//  pins setup
const uint8_t FAN_PWM[NumFans] = { 20,22,6,4, 21, 5, 3 };  // PWM outut pin
const uint8_t FAN_RPM[NumFans] = {  1, 2,7,0, 24,25,19 };  // RPM input pin

//  const
const char *fanNames[FN_All] = {
	"",	"CPU", "GPU", "PSU", "RAM",	"MB",  "Case", "Cool", "Rad", "Ext", "New", };

const char *fanModes[FM_All] = {
	"Hide", "Off", "On", /*"Auto",*/ };


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
void Fan::Init(uint8_t fanId)
{
	const uint8_t pwm = FAN_PWM[fanId], rpm = FAN_RPM[fanId];
	pinMode(pwm, OUTPUT);
	analogWriteFrequency(pwm, 10000);  // par 10k
	analogWrite(pwm, 0);  // off

	noRpm = rpm >= NoRPM;
	if (noRpm)
		return;

	pinMode(rpm, INPUT_PULLUP);
}

void Fans::Init()
{
	for (int i=0; i < NumFans; ++i)
		fan[i].Init(i);
}


//  check rpm pin  -----------
void Fans::Check()
{
	for (int i=0; i < NumFans; ++i)
	{
		const uint8_t rpm = FAN_RPM[i];
		Fan& f = fan[i];
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


	//  avg add
	avgArr[avgCnt] = rpm;
	++avgCnt;
	if (avgCnt >= avgNum)
		avgCnt = 0;

	//  avg get
	int sum = 0;
	for (int i = 0; i < avgNum; ++i)
		sum += avgArr[i];

	rpmAvg = sum / avgNum;
}


void Fans::Update()
{
	uint32_t dt = millis() - prevMS;
	if (dt > updateMS)
	{
		for (int i=0; i < NumFans; ++i)
			Update(i, dt);

		prevMS = millis();  
	}
}


//  Update  -----------
void Fans::Update(uint8_t i, uint32_t dt)
{
	Fan& f = fan[i];
	bool rpm = !f.noRpm;
	
	if (rpm)
		f.CalcRPM();
	
	bool on = f.fd.mode >= FM_On;
	uint16_t pwm = on ? f.fd.pwm : 0;
	// todo: auto pwm from Temp'C..

	//  turned on
	if (on && !f.oldOn)
		f.tmMax = msMax;
	f.oldOn = on;

	//  rpm stop prevention
	if (on && rpm && f.rpmAvg == 0)
		f.tmMax = msMax;

	//  short max pwm to start
	if (f.tmMax > 0)
	{
		f.tmMax -= dt;
		pwm = 4095;  // par
	}

	analogWrite(FAN_PWM[i], pwm);
}
