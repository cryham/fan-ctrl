#include "gui.h"
#include "Ada4_ST7735.h"
#include "matrix.h"
#include "kc_data.h"


//  color from value
void Gui::ClrTemp(int tm)
{
	if (tm >224)  d->setClr(31, 10,10);  else
	if (tm >192)  d->setClr(31, 18, 4);  else
	if (tm >160)  d->setClr(30, 30, 2);  else
	if (tm >128)  d->setClr(16, 29, 2);  else
	if (tm > 96)  d->setClr( 6, 27, 6);  else
	if (tm > 64)  d->setClr( 6, 26,26);  else
	if (tm > 32)  d->setClr(10, 20,30);  else
	if (tm > 16)  d->setClr( 6, 16,29);  else
	              d->setClr(16,  6,29);
}

//  Display
//....................................................................................
void Gui::DrawGraphs()
{
	char a[64];


	//  time  ---
	unsigned long t = rtc_get();
	int h = t / 3600 % 24, m = t / 60 % 60, s = t % 60;
	//to = t - kc.tm_on;

	// bool date = yr >= 2019; // rtc set
	// bool leap = isLeap(yr);

	//  x,y pos~
	int16_t x, y;
	const int16_t //x0 = W / 2,
		yt = 32, yu = H - 20, // time, uptime, date
		yp = yt + 22;


	//  title
	d->setClr(12, 14, 17);
	d->print(strMain[ym]);


	//  Graphs  ~~~~~~~~~~~~~~~~
	/*int v, y0, ii, i;
	//if (ext)
	for (i=0; i <= W-1; ++i)
	{
		ii = kc.grRpos + i - (W-1) + W;
		v = kc.grRpm[ym2Fan][ii % W];
		if (v > 0)
		{
			ClrTemp(v);  uint16_t c = d->getClr();
			y0 = yt - v / 3;  // 96 is max  //par scale-
			if (y0 < 0)  y0 = 0;
			d->drawPixel(i,y0, c);
	}	}
	*/

	//if (pgGraph == Cl_Graphs)
	{
		DrawGraph();
		//return;
	}

	x = 60;  y = 42;


	//  Uptime, since on  --------
	{
		y = yu;
		d->setCursor(x, y);
		d->setClr(18, 22, 28);
		sprintf(a, "%d:%02d:%02d", h, m, s);  d->print(a);
	}


	//  Temp'C  val  --------
	#ifdef TEMP_PIN
	// if (tempInit > TI_DONE)  // 'C
	{
		d->setCursor(6 +10, 40 -10);
		d->print("\x01""C  ");

		d->print('0'+tempCount);
		d->print("x  ");

		const static char* strTempInit[] =
		{  "fail", "search", "done", ""/*"read"*/  };
		d->print(strTempInit[tempInit]);
	}

	for (int i=0; i < tempCount; ++i)
	{
		char f[32];
		dtostrf(fTemp[i], 4,2, f);
		
		d->setClr(18,22,26);
		d->setCursor(6, 46+ i*10);

		if (fTemp[i] > 0.f)
			d->print(f);
	}
	#endif


	d->setFont(0);


	//  small font  ----------------------


	//  page / all  ---
	d->setClr(12, 16, 22);
	//if (!stats)
	{
		d->setCursor(W - 1 - 3 * 6, 4);
		sprintf(a, "%d/%d", pgGraph + 1, G_All);  d->print(a);
	}


	//  press / 1min
	x = 6;  y = yp;
	d->setCursor(x, y);

	//int m1 = kc.min1_Keys;
	//ClrTemp(m1);
	//dtostrf(m1, 3,0, f);  d->print(f);
}
