#include "WProgram.h"

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

Gui gui;
KC_Main kc;
extern void ParInit();


//  timer event,  scan
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


	kc.fans.Check();  // pins for rpm

	//  keys scan  slower for demos
	if (gui.ym != M_Demos || scan_n % 4==0)
	{
		Matrix_scan(0);
		gui.KeyPress();
	}

	kc.Update(ms);  // add to graphs

	us_scan = micros() - us;
}


//  main
//-------------------------------------------------------------------------
int main()
{
	ParInit();  // par defaults


	//  Init extra pins  --------
	//  PWM brightness to display LED
	pinMode(LCD_LED, OUTPUT);
	analogWriteResolution(12);
	analogWrite(LCD_LED, 1000);  // 0-4095


	//  Fans
	kc.fans.Init();

	//analogWriteRes(12);
	//analogWriteDAC0(0);


	Ada4_ST7735 tft;
	tft.begin();

	gui.Init(&tft);
	tft.clear();
	tft.display();  // black


	//  load set from ee
	kc.Load();
	//gui.SetScreen(ST_Demos2 + D_Plasma);
	gui.SetScreen(ST_Fans);


	//  keys
	Matrix_setup();

	//  48 MHz/50 000 = 960 Hz   display: 76 fps
	Periodic_init( par.scanFreq * 1000 );
	Periodic_function( &main_periodic );

	#ifdef LED
	pinMode(LED, OUTPUT);
	digitalWrite(LED, gui.led ? LOW : HIGH);
	#endif


	while(1)
	{
		kc.fans.Update();

		gui.Draw();

		gui.DrawEnd();

		//  temp get  --------
		#ifdef TEMP_PIN  // 18B20  Temp'C
		gui.GetTemp();
		#endif
	}
}
