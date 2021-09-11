#include "WProgram.h"
#include "RamMonitor.h"

#include "matrix.h"
#include "periodic.h"
#include "kc_data.h"

#include "Ada4_ST7735.h"
#include "gui.h"
#include "fans.h"


//  scan counter, freq
uint scan_cnt = 0, scan_freq = 0;
uint32_t us_scan = 0, ms_scan = 0;
uint8_t scan_n = 0;

RamMonitor ram;
Gui gui;
KC_Main kc;
extern void ParInit();
Fans fans;


//  kbd  timer event,  scan, send
//------------------------------------------------
void main_periodic()
{
	uint32_t us = micros();

	//  freq info
	uint32_t ms = millis(),
		ms_diff = ms - ms_scan;
	if (ms_diff >= 1000)  // 1s
	{
		ms_scan = ms;
		scan_freq = scan_cnt;  // Hz
		scan_cnt = 0;
	}
	++scan_cnt;
	++scan_n;


	//  kbd scan
	bool bsc = false;
	if (  // slower for demos
		gui.ym != M_Demos || scan_n % 4==0)
	{	Matrix_scan(0);  bsc = true;  }  // K

	//  gui
	if (bsc)
		gui.KeyPress();

	//  keyboard
	kc.Update(ms);

	us_scan = micros() - us;
}


//  main
//-------------------------------------------------------------------------
int main()
{
	ram.initialize();
	ParInit();  // par defaults


	//  Init extra pins  --------
	//  PWM brightness to display LED
	pinMode(LCD_LED, OUTPUT);
	analogWriteResolution(12);
	analogWrite(LCD_LED, 1000);  // 0-4095

	//  fans
	fans.Init();

	// analogWriteRes(12);
	// analogWriteDAC0(0);


	Ada4_ST7735 tft;
	tft.begin();

	gui.Init(&tft);
	tft.clear();
	tft.display();  // black


	//  load set from ee
	kc.Load();
	//gui.SetScreen(ST_Demos2 + D_Plasma);
	//gui.SetScreen(ST_Clock + Cl_Stats);
	gui.SetScreen(ST_Main0);
	//gui.SetScreen(ST_Displ);
	//kc.Save();


	//  kbd
	Matrix_setup();

	//  48 MHz/50 000 = 960 Hz   d: 52 fps
	Periodic_init( par.scanFreq * 1000 );
	Periodic_function( &main_periodic );

	#ifdef LED
	pinMode(LED, OUTPUT);
	digitalWrite(LED, gui.led ? LOW : HIGH);
	#endif

	while(1)
	{
		ram.run();

		fans.Update();

		gui.Draw();

		gui.DrawEnd();

		//  temp get  --------
		#ifdef TEMP1  // 18B20  Temp'C
		gui.GetTemp();
		#endif
	}
}
