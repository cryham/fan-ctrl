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
		const int minBri = 100;  //par ?
		int val = bri == 0 ? 0 : bri * (4095 - minBri) / 100 + minBri;
		analogWrite(LCD_LED, val);
	}

	//  add graph stats  ---
	if (par.timeRpm)
	if (ms - msMin1 > 1000 * t1min(par) || ms < msMin1)
	{
		msMin1 = ms;
		min1_Keys = cnt_press1min * 60 / t1min(par);
		cnt_press1min = 0;

		//  graph inc pos
		++grPpos;
		if (grPpos >= W)  grPpos = 0;
		//  add to graph
		grPMin[grPpos] = min1_Keys > 255 ? 255 : min1_Keys;
	}

}
