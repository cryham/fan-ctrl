#include "gui.h"
#include "Ada4_ST7735.h"
#include "matrix.h"
#include "kc_data.h"


//  Display
//....................................................................................
void Gui::DrawGraphs()
{
	char a[64];


	//  Graphs  ~~
	int onFans = kc.fans.onFans;

	auto h = [](int v) {  return min(H-1, v);  };
	int y = 0;
	
	switch (par.pgGraph)
	{
	case G_Temp:
	{	if (!tempCount)  break;
		const int yT = H / tempCount;
		
		for (int i=0; i < tempCount; ++i)
			DrawGraphTi(0, W-1,  h(i*yT), h( (i+1)*yT -1 ),  true, i);
	}	break;
	
	case G_Rpm:
	{	if (!onFans)  break;
		const int yR = H / onFans;
		
		for (int i=0; i < NumFans; ++i)
		if (kc.fans.fan[i].on)
		{	DrawGraphR(0, W-1,  h(y*yR), h( (y+1)*yR -1 ),  true, i);  ++y;  }
	}	break;

	case G_Both:
	{	if (!tempCount || !onFans)  break;
		const int yT = H / tempCount;
		
		for (int i=0; i < tempCount; ++i)
			DrawGraphTi(0, W/2,  h(i*yT), h( (i+1)*yT-1 ),  true, i);

		const int yR = H / onFans;
		
		for (int i=0; i < NumFans; ++i)
		if (kc.fans.fan[i].on)  // todo: fan params rpm,temp on graphs..
		{	DrawGraphR(W/2, W-1,  h(y*yR), h( (y+1)*yR -1 ),  true, i);  ++y;  }
		break;
	}
	}


	//  page / all  ---
	d->setClr(12, 16, 22);
	d->setCursor(W-1 - 3*6, 4);
	sprintf(a, "%d/%d", par.pgGraph + 1, G_All);  d->print(a);


	//  test Temp'C  init, vals  --------
	#if 0
	// if (tempInit > TI_DONE)  // 'C
	{
		d->setCursor(W/2 -16, 4);
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
}


//  color from byte value  . . . . . . . . . . . . . .
void Gui::ClrByte(int val)
{
	const static uint8_t rgb[16][3] = {
		{18, 16,29},  // viol
		{ 8, 16,29},  // blue
		{10, 20,30},  // skyblue
		{ 8, 23,25},
		{ 6, 26,26},  // cyan
		{ 6, 27,16},
		{ 4, 27, 4},  // green
		{11, 28, 4},
		{16, 29, 2},  // lime
		{24, 30, 2},
		{30, 30, 1},  // yellow
		{30, 22, 2},
		{31, 18, 4},  // orange
		{31, 14, 7},
		{31, 10,10},  // red
		{31, 20,25},
	};
	int b = val / 16;
	b = min(15, b);
	d->setClr(rgb[b][0], rgb[b][1], rgb[b][2]);
}
