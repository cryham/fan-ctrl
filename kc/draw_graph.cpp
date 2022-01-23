#include "gui.h"
#include "kc_data.h"

#include "Ada4_ST7735.h"
#include "TomThumb3x5.h"


void Gui::DrawGraphT(int16_t xMin, int16_t xMax, int16_t yMin, int16_t yMax, bool legend, uint8_t id)
{
	const Fan& f = kc.fans.fan[id];
	DrawGraph(xMin, xMax, yMin, yMax, true, legend, f.fd.tempId);
}
void Gui::DrawGraphTi(int16_t xMin, int16_t xMax, int16_t yMin, int16_t yMax, bool legend, uint8_t id)
{
	DrawGraph(xMin, xMax, yMin, yMax, true, legend, id);
}
void Gui::DrawGraphR(int16_t xMin, int16_t xMax, int16_t yMin, int16_t yMax, bool legend, uint8_t id)
{
	DrawGraph(xMin, xMax, yMin, yMax, false, legend, id);
}

//  Graph  ~ ~
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void Gui::DrawGraph(int16_t xMin, int16_t xMax, int16_t yMin, int16_t yMax,
	bool temp, bool legend, int id)
{
	if (id < 0)  return;
	const int16_t xLen = xMax - xMin, yLen = yMax - yMin,
		yF = 13, left = 22, top = 1;  // legend ? 10 : 6;  // margin
	bool small = yLen < H/2 -8;  // par

	int i,x,y, v;  // var
	char a[64];


	//  x time and val
	auto getVal = [&](auto i)  {
		int ii = (temp ? grTpos : grRpos) + i + W;
		return temp ? grTemp[id][ii % W] : grRpm[id][ii % W];
	};
	auto x4tm = [&](auto tm)  {
		return xMax - (tm * 60000 / (temp ? tTgraph(par) : tRpm(par)));
	};


	//  draw grid line with text value
	auto GridLineV = [&](int x, int n/*clr*/, const char* s)
	{
		if (x < xMin || x > xMax)  return;
		if (!legend)  n -= 7;

		uint16_t clr = RGB(n,n,n);
		d->drawFastVLine(x, yMin + top, yLen - top, clr);  // |
		
		x -= 6;
		if (/*legend ||*/ x < xMin + left || x >= xMax-10)  return;
		n += 3;  clr = RGB(n,n,n);

		d->setCursor(x, yMin + 2);  d->setColor(clr);
		d->print(s);  // txt
	};

	auto GridLineH = [&](int y, int n/*clr*/, int val) //, bool show=false)
	{
		if (y < yMin || y > yMax)  return;
		if (!legend)  n -= 5;
		
		uint16_t clr = RGB(n,n,n);
		d->drawFastHLine(xMin, y, xLen, clr);  // --
		
		n += 4;  clr = RGB(n,n,n);
		if (!small)  // txt val
		{
			if (y + 10 >= yMax)  y = yMax - 10;
			d->setCursor(xMin, y + 2);  d->setColor(clr);
			sprintf(a,"%d", val);  d->print(a);
		}
	};

	if (small)
		d->setFont(&TomThumb);  // mini
	else
		d->setFont();


	//------------------------------------------------------------------------
	if (temp && grTempUpd[id])
	{			grTempUpd[id] = 0;	AutoRangeTemp(id);	}
	if (!temp && grRpmUpd[id])
	{			 grRpmUpd[id] = 0;	AutoRangeRpm(id);	}

	//  grid  vertical  |||
	{
		GridLineV(x4tm(  1),  8, "1" );
		GridLineV(x4tm(  2),  9, "2" );
		GridLineV(x4tm(  5), 10, "5" );
		GridLineV(x4tm( 10), 15, "10");  // m
		GridLineV(x4tm( 30), 13, "30");
		GridLineV(x4tm( 60), 18, "1h");  // h
		GridLineV(x4tm(120), 16, "2h");
		GridLineV(x4tm(240), 16, "4h");
		//GridLineV(x4tm(480), 16, "8h");
	}

	//  Temp 'C
	if (temp && grFmax[id] > grFmin[id])
	{
		// grid  horizontal  ===
		bool all = small ? false : grFmax[id] - grFmin[id] < 6;  // par
		for (i = grFmin[id]; i <= grFmax[id]; ++i)  // 'C
		{
			y = yMax - yLen * (float(i) - grFmin[id]) / (grFmax[id] - grFmin[id]);
			if (all || i%2==0)
				GridLineH(y, i%5==0 ? 13 : i%2==0 ? 11 : 8, i);
		}
		//  graph pixels  ~~~
		for (i = xMin; i <= xMax; ++i)
		{
			v = getVal(i - xMax);
			if (v > 0)
			{	ClrByte(v);

				y = yMax - yLen * (float(v) - grBmin[id]) / (grBmax[id] - grBmin[id]);
		
				if (y >= yMin && y <= yMax)
					d->drawPixel(i,y, d->getClr());
		}	}
	}

	//  Rpm
	if (!temp && grRFmax[id] > grRFmin[id])
	{
		// grid  horizontal  ===
		bool all = small ? false : grRFmax[id] - grRFmin[id] < 600;  // par
		for (i = grRFmin[id]; i <= grRFmax[id]; i+=100)  // rpm
		{
			y = yMax - yLen * (float(i) - grRFmin[id]) / (grRFmax[id] - grRFmin[id]);
			if (all || i%200==0)
				GridLineH(y, i%300==0 ? 13 : i%200==0 ? 11 : 8, i);
		}
		//  graph pixels  ~~~
		for (i = xMin; i <= xMax; ++i)
		{
			v = getVal(i - xMax);
			if (v > 0)
			{	ClrByte(v);

				y = yMax - yLen * (float(v) - grRBmin[id]) / (grRBmax[id] - grRBmin[id]);
				
				if (y >= yMin && y <= yMax)
					d->drawPixel(i,y, d->getClr());
		}	}
	}


	d->setFont();

	//  legend values, cursor
	if (legend)
	{
		x = xMin + xLen/2 -12; // + left;
		y = yMin + top;
		d->setCursor(x,y);
		d->setClr(22, 25, 28);
		y += yF;

		const Fan& f = kc.fans.fan[id];
		if (temp)
			dtostrf(fTemp[id], 4,1,a);
		else
			sprintf(a,"%d", f.rpmAvg);
		
		d->setCursor(x,y);  d->print(a);  y += yF;
		if (temp)
			d->print(" \x01""C");
	}
}
