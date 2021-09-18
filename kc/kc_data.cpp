#include "kc_data.h"
#include "matrix.h"
#include "gui.h"
#include "WProgram.h"

KC_Params par;


//  KCupdate
//------------------------------------------------------------------------
void KC_Main::Update(uint32_t ms)
{

	//  brightness set lcd led pwm  ~~~
	if (setBright)
	{	setBright = 0;

		int bri = par.brightness;
		const int minBri = 50;  // par
		int val = bri == 0 ? 0 : bri * (4095 - minBri) / 100 + minBri;
		analogWrite(LCD_LED, val);
	}

	//  add graph stats  ---
	if (par.timeRpm)
	if (ms - msRpm > tRpm(par) || ms < msRpm)
	{
		msRpm = ms;
		for (int i=0; i < NumFans; ++i)
		{
			Fan& f = fans.fan[i];
			uint8_t val = f.rpmAvg / 10;  // par

			//  add to graph
			grRpm[i][grRpos] = val;
		}
		//  graph inc pos
		++grRpos;
		if (grRpos >= W)  grRpos = 0;
	}

}
