#include "gui.h"
#include "kc_data.h"

#include "Ada4_ST7735.h"
#include "TomThumb3x5.h"


//  Rpm vals in graphs
uint8_t Gui::RpmFtoB(uint16_t r)
{
	int t = 255.f * (r - par.minRpm) / (par.maxRpm - par.minRpm);
	t = t > 255 ? 255 : (t < 1 ? 1 : t);  // 0 not measured?
	return t;
}
uint16_t Gui::RpmBtoF(uint8_t b)
{
	return b / 255.f * (par.maxRpm - par.minRpm) + par.minRpm;
}

//  Temp vals in graphs
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
void Gui::AutoRangeTemp(int d)
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
	if (tmin > tmax)  // none yet, default
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

//  auto range Rpm  get min,max
void Gui::AutoRangeRpm(int d)
{
	uint8_t rmin = 255, rmax = 0;
	for (int i=0; i <= W-1; ++i)
	{
		uint8_t t = grRpm[d][i];
		if (t > 0)  // measured
		{
			if (t > rmax)  rmax = t;
			if (t < rmin)  rmin = t;
		}
	}
	if (rmin > rmax)  // none yet, default
	{
		grRFmin[d] = 300;  grRBmin[d] = TempFtoB(grRFmin[d]);
		grRFmax[d] = 900;  grRBmax[d] = TempFtoB(grRFmax[d]);
	}else
	{	grRFmin[d] = (RpmBtoF(rmin)/100)*100;
		grRFmax[d] = (RpmBtoF(rmax)/100)*100+100;  // in 
		if (grRFmin[d] + 300 >= grRFmax[d])  // min range 200
		{	grRFmin[d] -= 100;  grRFmax[d] += 100;  }
		
		grRBmin[d] = RpmFtoB(grRFmin[d]);
		grRBmax[d] = RpmFtoB(grRFmax[d]);
		if (grRBmin[d] == grRBmax[d])
			grRBmax[d] += 100;
	}
}


//  Auto graph  exp _/  pwm from temp
//**********************************************************************
void Gui::DrawAutoGraph(const Fan* f)
{
	//  Temp'C lines ||
	const float fTmax = f->fd.a.tempMax *0.1f;
	const float fTmin = f->fd.a.tempMin *0.1f;
	const float fTlen = fTmax - fTmin;

	for (int x=fTmin; x < fTmax+1; ++x)
	{
		const int n = x % 10==0 ? 18 : x % 5 == 0 ? 14 : 9;
		const int xt = (float(x) - fTmin) / fTlen * (W-1);
		d->drawFastVLine(xt, 0, H-1, RGB(n,n,n));
	}

	//  Power % lines ==
	const int Pmax = f->fd.a.pwmMax;
	const int Pmin = f->fd.a.pwmMin;
	const int Plen = Pmax - Pmin;
	int oy = -1;
	for (int p=Pmin; p < Pmax+1; ++p)
	{
		const int y = 100.f * p / 4095.f;
		if (oy != y)  // lines not bars
		{	oy = y;
			const int n = y % 10==0 ? 18 : y % 5 == 0 ? 14 : y % 2 == 0 ? 11 : 8;
			int yp = (float(p) - Pmin) / Plen * (H-1);
			if (yp > 0)
				d->drawFastHLine(0, H-1 -yp, W-1, RGB(n,n,n));
	}	}

	//  graph _/
	for (int x=0; x < W; ++x)
	{
		const float fT = float(x)/(W-1) * fTlen + fTmin;
		const auto pwm = f->GetPWMAuto(fT);
		
		const float fP = float(H-1) * (pwm - Pmin) / float(Plen);
		const int y = max(0, H-1 - int(fP));
		
		ClrByte(255 - y*2);  // rainbow
		d->drawPixel(x, y, d->getClr());
	}
}
