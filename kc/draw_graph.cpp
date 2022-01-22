#include "gui.h"
#include "kc_data.h"

#include "Ada4_ST7735.h"
#include "TomThumb3x5.h"

#ifdef TEMP_PIN

//  Temp val
int Gui::TempFtoB(float tm)
{
	int t = 255.f * (tm - par.minTemp) / (par.maxTemp - par.minTemp);
	t = t > 255 ? 255 : (t < 1 ? 1 : t);  // 0 not measured
	return t;
}
float Gui::TempBtoF(uint8_t b)
{
	return b / 255.f * (par.maxTemp - par.minTemp) + par.minTemp;
}

//  auto range Temp  get min,max
void Gui::AutoRange(int d)
{
	uint8_t tmin = 255, tmax = 0;
	for (int i=0; i <= W-1; ++i)
	{
		uint8_t t = grTemp[d][i];
		if (t > 0)  // measured
		{
			if (t > tmax)  tmax = t;
			if (t < tmin)  tmin = t;
		}
	}
	if (tmin > tmax)  // none yet
	{
		grFmin[d] = 20;  grBmin[d] = TempFtoB(grFmin[d]);
		grFmax[d] = 23;  grBmax[d] = TempFtoB(grFmax[d]);
	}else
	{	grFmin[d] = floor(TempBtoF(tmin));
		grFmax[d] =  ceil(TempBtoF(tmax));  // in 'C
		if (grFmin[d] + 3 >= grFmax[d])  // min range 3'C
		{	--grFmin[d];  ++grFmax[d];  }
		
		grBmin[d] = TempFtoB(grFmin[d]);
		grBmax[d] = TempFtoB(grFmax[d]);
		if (grBmin[d] == grBmax[d])
			++grBmax[d];
	}
}

//  Graphs ~ ~
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void Gui::DrawGraph(int16_t xMin, int16_t xMax, int16_t yMin, int16_t yMax,
	bool temp, bool legend, int id)
{
	const int16_t xLen = xMax - xMin, yLen = yMax - yMin,
		yF = 13, left = 22, top = legend ? 10 : 6;  // marg

	const Fan& f = kc.fans.fan[id];
	const FanData& fd = f.fd;
	if (temp)  id = fd.tempId;
	if (id < 0)  return;

	int i,x,y, v;  // var
	char a[64];


	int xc = W-1; //par.xCur;
	bool cursor = legend && xc < W;
	auto cur = [&](auto i) {  return cursor && i == xc;  };

	//  x time and val
	auto getVal = [&](auto i){
		int ii = (temp ? grTpos : kc.grRpos-1) + i + W;
		return temp ? grTemp[id][ii % W] : kc.grRpm[id][ii % W];  };
	auto x4tm = [&](auto tm){
		return xMax - (tm * 60000 / (temp ? tTgraph(par) : tRpm(par)));  };


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
		//if (show || legend)
		{
			if (y + 10 >= yMax)  y = yMax - 10;
			d->setCursor(xMin, y + 2);  d->setColor(clr);
			sprintf(a,"%d", val);  d->print(a);  // txt val
		}
	};

	d->setFont();
	// d->setFont(&TomThumb);  // mini


	//------------------------------------------------------------------------
	if (temp)
	if (grTempUpd[id])
	{	grTempUpd[id] = 0;
		AutoRange(id);
	}

	//  grid  vertical  |||
	{
		GridLineV(x4tm(  1),  8, "1" );
		//GridLineV(x4tm(  2),  9, "2" );
		GridLineV(x4tm(  5), 10, "5" );
		GridLineV(x4tm( 10), 15, "10");  // m
		GridLineV(x4tm( 30), 13, "30");
		GridLineV(x4tm( 60), 18, "1h");  // h
		GridLineV(x4tm(120), 16, "2h");
		GridLineV(x4tm(240), 16, "4h");
		GridLineV(x4tm(480), 16, "8h");
	}

	//  grid  horizontal  ===
	if (temp && grFmax[id] > grFmin[id])
	for (i = grFmin[id]; i <= grFmax[id]; ++i)  // 'C
	{
		y = yMax - yLen * (float(i) - grFmin[id]) / (grFmax[id] - grFmin[id]);
		GridLineH(y, i%5==0 ? 13 : i%2==0 ? 11 : 9, i);
	}

	if (!temp)
	for (i = 0; i <= 6; ++i)  // rpm
	{
		y = yMax - yLen * float(i*20) / 128.f;
		GridLineH(y, i%3==0 ? 13 : i%2==0 ? 11 : 9, i*200);
	}


	//  graph pixels  ~~~
	if (grBmax > grBmin)
	for (i = xMin; i <= xMax; ++i)
	{
		v = getVal(i - xMax);
		if (v > 0)
		{	ClrByte(temp ? v : v*2);  // color max 1270 rpm

			if (temp)
				y = yMax - yLen * (float(v) - grBmin[id]) / (grBmax[id] - grBmin[id]);
			else
				y = yMax - yLen * float(v) / 128.f;
			
			if (y >= yMin && y <= yMax)
				d->drawPixel(i,y, cur(i) ? RGB(31,31,31) : d->getClr());
	}	}

	d->setFont();

	//  legend values, cursor
	if (legend)
	{
		x = xMin + xLen/2 -12; // + left;
		y = yMin + top;
		d->setCursor(x,y);
		d->setClr(21, 24, 27);
		y += yF;

		if (cursor)
		{
			d->drawPixel(xc, yMin+1, RGB(29,29,29));  // :
			d->drawPixel(xc, yMax, RGB(29,29,29));

			if (temp)
				dtostrf(fTemp[id], 4,1,a);
			else
				sprintf(a,"%d", f.rpmAvg);
			
			d->setCursor(x,y);  d->print(a);  y += yF;
			if (temp)
				d->print(" \x01""C");

			/*d->setCursor(x,y);  PrintInterval(
				(temp ? tTgraph(par) : tRpm(par)) * (W-1-xc));  y += yF;*/
		}
	}
}
#endif


//**********************************************************************
void Gui::DrawAutoGraph(const Fan* f)
{
	//  Temp'C lines ||
	const float fTmax = f->fd.a.tempMax *0.1f;
	const float fTmin = f->fd.a.tempMin *0.1f;
	const float fTlen = fTmax - fTmin;
	for (int x=fTmin; x < fTmax+1; ++x)
	{
		int n = x % 10==0 ? 18 : x % 5 == 0 ? 14 : 9;
		int xt = (float(x) - fTmin) / fTlen * (W-1);
		d->drawFastVLine(xt, 0, H-1, RGB(n,n,n));
	}

	//  Power % lines ==
	const int Pmax = f->fd.a.pwmMax, pmax = 100.f * Pmax / 4095.f;
	const int Pmin = f->fd.a.pwmMin, pmin = 100.f * Pmin / 4095.f;
	const int Plen = Pmax - Pmin,    plen = pmax - pmin;
	for (int y=pmin; y < pmax+1; ++y)
	{
		int n = y % 10==0 ? 18 : y % 5 == 0 ? 14 : y % 2 == 0 ? 11 : 8;
		int yt = (float(y) - pmin) / plen * (H-1);
		d->drawFastHLine(0, H-1 -yt, W-1, RGB(n,n,n));
	}

	//  exp Auto pwm graph _/
	//const uint16_t clr = RGB(20,20,26);
	for (int x=0; x < W; ++x)
	{
		float fT = float(x)/(W-1) * fTlen + fTmin;
		auto pwm = f->GetPWMAuto(fT);
		// float p = 100.f * pwm / 4095.f;
		// int y = float(H-1) * (p - pmin) / pmax;
		
		float fP = float(H-1) * (pwm - Pmin) / float(Plen);
		int y = max(0, H-1 - int(fP));
		ClrByte(255 - y*2);
		d->drawPixel(x, H-1 -y, d->getClr());
	}
}
