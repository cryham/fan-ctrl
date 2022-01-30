#include "fans.h"
#include "WProgram.h"


//  Fan pins setup
const uint8_t FAN_PWM[NumFans] =
	{ 20,22,6,4, 21, 5, 3 };  // PWM outut pin
const uint8_t FAN_RPM[NumFans] =
	{  1, 2,7,0, 24,25,19 };  // RPM input pin  use NoRPM if not present

 
//  const
const char *fanNames[FNames_All] = {
	"",	"CPU", "GPU", "PSU", "RAM",	"MB", "HDD",
	"Case", "Cool", "Rad", "Ext", "Wtr", "New", };

const char *fanModes[FModes_All] = {
	"Hide", "Off", "On", "Ext1:", "Ext0:" };


//  Init  -----------
Fan::Fan()
{
	lastRpmMS = millis();
}
Fans::Fans()
{
	prevMS = millis();
}

void Fan::GetFanName(char* s) const
{
	if (fd.name >= FNames_All)
	{	s[0] = 0;
		return;
	}

	if (fd.number > 0)
		sprintf(s,"%s%d", fanNames[fd.name], fd.number);
	else
		sprintf(s,"%s", fanNames[fd.name]);
}


//  Setup pins  -----------
void Fan::Init(uint8_t fanId)
{
	const uint8_t pwm = FAN_PWM[fanId], rpm = FAN_RPM[fanId];
	pwmPin = pwm;
	pinMode(pwm, OUTPUT);
	UpdFreq();
	analogWrite(pwm, 0);  // off

	hasRpm = rpm < NoRPM;
	if (hasRpm)
		pinMode(rpm, INPUT_PULLUP);
}
void Fan::UpdFreq()
{
	if (fd.freq < 1)
		fd.freq = 10;
	analogWriteFrequency(pwmPin, fd.freq * 1000);
}

void Fans::Init(float* pfTemp)
{
	fTemp = pfTemp;
	for (int i=0; i < NumFans; ++i)
		fan[i].Init(i);
}


//  check rpm pin  -----------
void Fans::CheckRPM()
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
	//  rpm compute
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


	//  new average val add
	if (!fd.avgNum)
	{
		rpmAvg = rpm;
		return;
	}
	avgArr[avgCnt] = rpm;
	++avgCnt;
	if (avgCnt >= fd.avgNum)
		avgCnt = 0;

	//  average rpm compute
	int sum = 0;
	for (int i = 0; i < fd.avgNum; ++i)
		sum += avgArr[i];

	rpmAvg = sum / fd.avgNum;
}


//  update all
void Fans::Update()
{
	uint32_t dt = millis() - prevMS;
	if (dt > RpmUpdateMS)
	{
		for (int i=0; i < NumFans; ++i)
		{
			Fan& f = fan[i];

			if (f.hasRpm)
				f.CalcRPM();
		}
		prevMS = millis();
	}

	#ifdef EXT_ON
	ext_on = digitalRead(EXT_ON);
	#endif

	int cnt = 0;
	for (int i=0; i < NumFans; ++i)
	{
		Update(i, dt);

		if (fan[i].on)
			++cnt;
	}
	onFans = cnt;
}


//  Update 1  ----------------------
void Fans::Update(uint8_t i, uint32_t dt)
{
	Fan& f = fan[i];

	f.on = f.GetOn(ext_on);
	
	uint16_t pwm = f.GetPWM(fTemp);

	f.Guard(dt, pwm);

	//  set fan pwm
	analogWrite(FAN_PWM[i], pwm);
	f.pwm = pwm;
}


//  on
bool Fan::GetOn(bool ext_on) const
{
	bool on = fd.mode >= FM_On;
	
	//  external on/off pin
	#ifdef EXT_ON
	if (fd.mode == FM_ExtOn)
		on = ext_on;
	else if (fd.mode == FM_ExtOff)
		on = !ext_on;
	#endif

	return on;
}


//  get pwm value
uint16_t Fan::GetPWM(float* fTemp) const
{
	uint16_t pwm = on ? fd.pwm : 0;
	
	//  auto pwm from temp'C
	auto id = fd.tempId;
	if (fd.a.on && id >= 0 && id < MaxTemp && fTemp)
	{
		float fT = fTemp[id];
		pwm = GetPWMAuto(fT);
	}
	return pwm;
}

//  Auto rpm  from temp
uint16_t Fan::GetPWMAuto(float fT) const
{
	const float fTMin = fd.a.tempMin *0.1f;
	if (fT < fTMin)
		return 0;  // off

	const float fTMax = fd.a.tempMax *0.1f;
	if (fT > fTMax)
		return fd.a.pwmMax;
	
	//  linear _/^
	const float fTempMul = (fT - fTMin) / (fTMax - fTMin);
	const int pwmMul = fd.a.pwmMax - fd.a.pwmMin;

	const float ftmp = fd.a.exp == 100 ?
		fTempMul :
		powf(fTempMul, fd.a.exp / 100.f);
	return fd.a.pwmMin + pwmMul * ftmp;
}


//  rpm Guard, stop prevention
void Fan::Guard(uint32_t dt, uint16_t& pwm)
{
	if (!fd.g.on ||
		(fd.a.on && pwm == 0))  // auto off
	{	tmLeft = 0;
		return;  // no guard
	}

	//  just turned on
	if (on && !oldOn)
		tmLeft = fd.g.msOn;
	oldOn = on;

	if (on && hasRpm && rpmAvg == 0)
		tmLeft = fd.g.msOn;  // trigger

	//  short max pwm to start
	if (tmLeft > 0)
	{	tmLeft -= dt;
		pwm = fd.g.pwmOn;
	}
}
