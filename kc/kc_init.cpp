#include "kc_data.h"
//#include "usb_keyboard.h"
#include "matrix.h"
#include "kc_params.h"
#include "WProgram.h"  // rtc, Mouse


//  ctor  Main
//------------------------------------------------
KC_Main::KC_Main()
{
	err = E_ok;  memSize = 0;

	//  rtc set
	unsigned long t = rtc_get();

	int yr = t/3600/24/365;
	if (yr == 0)  // set date if none
	{
		//  whatever in 2021, 614736000
		t = 21 * 365 + 8 * 30;  t *= 24 * 3600;
		t += 16 * 3600;
		rtc_set(t);
	}
	ResetStats(true);
}


//  Reset stats, rtc, times
//------------------------------------------------
void KC_Main::ResetStats(bool rtc)
{
	// rtc
	unsigned long t = rtc_get();
	if (rtc)
		tm_on = t;

	cnt_press = 0;  // matrix.h
}
