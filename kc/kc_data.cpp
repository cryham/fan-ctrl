#include "kc_data.h"
#include "matrix.h"
#include "WProgram.h"

KC_Params par;
extern uint32_t tmKeyPress;


//  KCupdate
//------------------------------------------------------------------------
void KC_Main::Update(uint32_t ms)
{
	//  brightness set, lcd led pwm
	int bri = par.brightness;
	if (par.timeOff > 4)
	{
		uint32_t d = ms - tmKeyPress, dt = tTmOff(par) + 1;
		if (d > dt || ms < tmKeyPress)
			bri = par.brightOff;
		else
		{	//  fade out  last 1/4 time
			float f = float(d) / dt;
			f = 4.f - 4.f*f;
			f = min(1.f, max(0.f, f));
			bri = f * (par.brightness - par.brightOff) + par.brightOff;
	}	}
	const int minBri = 0;  // par
	int val = bri == 0 ? 0 : bri * (4095 - minBri) / 100 + minBri;
	analogWrite(LCD_LED, val);
}
