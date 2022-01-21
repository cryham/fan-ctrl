#include "gui.h"
#include "kc_data.h"

#include "Ada4_ST7735.h"
#include "FreeSans9pt7b.h"


//  Fans
//....................................................................................
void Gui::DrawFans()
{
	char a[64];
	int16_t y = 16+4;

	for (int i=0; i < NumFans; ++i)
	{
		const Fan& f = kc.fans.fan[i];
		bool hid =  f.fd.mode == FM_Hide
				|| (f.fd.mode == FM_ExtOn && !kc.fans.ext_on)
				|| (f.fd.mode == FM_ExtOff && kc.fans.ext_on);
		int ya = hid ? 6 : 16;

		//  cur
		int c = abs(i - ym2Fan);
		if (!c)
		{	d->setClr(10,22,30);
			d->fillRect(0, y-1, W-1, ya, RGB(4,4,6));
		}
		FadeClr(C_Fans, 4, c, 1);

		if (hid)  //  ---  fan line  ---
		{
			d->drawFastHLine(2, y+1, 12, d->getClr());
		}else
		{
			//  name
			f.GetFanName(a);
			d->setCursor(2, y);  d->print(a);

			//  mode ..
			if (f.fd.mode == FM_Off && f.rpmAvg == 0)
			{
				d->setCursor(58, y);  d->print("off");
			}else
			{	//  pwm %
				/*if (f.fd.pwm < 400)  // 10%
					dtostrf(100.f * f.fd.pwm / 4095.f, 3,1, a);
				else*/
					sprintf(a,"%2d", 100 * f.pwm / 4095);
				
				d->setCursor(50, y);  d->print(a);

				//  rpm
				if (f.hasRpm)
				{
					sprintf(a,"%4d", f.rpmAvg);
					//sprintf(a,"%2d", f.rpmAvg/60);  // rps  f.rpm);
					d->setCursor(79, y);  d->print(a);
				}
				//  line --
				/*auto cl = d->getClr();
				d->drawFastHLine(0, y+ya-1, f.bar/4095.f*(W-1), cl);*/

				//  temp 'C
				if (f.fd.tempId >= 0)
				{
					dtostrf(fTemp[f.fd.tempId], 4,1, a);
					d->setCursor(123, y);  d->print(a);
				}
			}
		}
		y += ya-1;
	}

	//  header, legend
	d->setFont(0);
	d->setClr(10,16,22);
	d->setCursor(0,0);  // todo: rps/rpm?
	//sprintf(a,"Fan Name  Pwr%%  rps  Rpm");
	//sprintf(a," Fan  Power %%  Rpm  Temp\x01""C");
	sprintf(a," Fan      %%    Rpm    \x01""C");
	d->print(a);
}


//  Fan Details
//....................................................................................
void Gui::DrawFanDetails()
{
	char a[64],b[32];
	const Fan& f = kc.fans.fan[ym2Fan];
	const FanData& fd = f.fd;


	//~~~  graph  backgr big  ~~~
	int id = ym2Fan;
	switch (pgDet)
	{
	case 0:  // pwm, rpm
		DrawGraph(0, W-1,  0, H-1,  0, false, id);  break;

	case 1:  // temp
		if (fd.tempId >= 0)
		DrawGraph(0, W-1,  0, H-1,  1, false, id);  break;

	case FanDetPages-1:
		if (fd.tempId >= 0)  // rpm & temp
		{	DrawGraph(0, W-1,  0,   H/2,  0, true, id);
			DrawGraph(0, W-1,  H/2, H-1,  1, true, id);
		}else  // rpm
			DrawGraph(0, W-1,  0, H-1,  0, false, id);  break;
		break;
	}


	//  title
	d->setClr(18,18,24);
	d->setCursor(32,0);
	d->setFont(&FreeSans9pt7b);

	f.GetFanName(b);
	sprintf(a,"%d: %s", ym2Fan+1, b);

	d->print(a);
	d->setFont(0);

	
	//  page / all
	d->setClr(21,21,27);
	d->setCursor(W-1 -3*6, 4);
	sprintf(a,"%d/%d", pgDet+1, FanDetPages);
	d->print(a);


	//---  param values  ---
	int ln = FanDetLines[pgDet] + FanDetLinExt[pgDet];
	int16_t y = 32;

	for (int i=0; i <= ln; ++i)
	{
		d->setCursor(22, y);
		int c = abs(i - yFanDet[pgDet]);  // dist dim
		if (!c)
		{	//d->fillRect(0, y-1, W/2-1, 10, RGB(2,4,6));
			d->setClr(6,22,31);
			d->print("\x10 ");  // >
		}else
			d->print("  ");

		FadeClr(C_FanDet, 4, c, 1);

		int8_t h = 4;
		switch (pgDet)
		{
		case 0:  // page 1  %, rpm
			switch (i)
			{
			case 0:
			{	auto p = fd.a.on ? f.pwm : fd.pwm;
				dtostrf(100.f * p / 4095.f, 3,1, b);
				if (fd.a.on)
					sprintf(a,"Auto %% %s", b);
				else
					sprintf(a,"Power %% %s", b);
				h = 2;
			}	break;
			case 1:
				dtostrf(100.f * tFanAdd[par.iFanAdd] / 4095.f, 3,1, b);
				sprintf(a,"add %% %s", b);  h = 4;  break;
			case 2:
				if (fd.mode == FM_ExtOn)
					sprintf(a,"Mode: %s %s", fanModes[fd.mode],
						kc.fans.ext_on ? "on" : "off");
				else if (fd.mode == FM_ExtOff)
					sprintf(a,"Mode: %s %s", fanModes[fd.mode],
						!kc.fans.ext_on ? "on" : "off");
				else
					sprintf(a,"Mode: %s", fanModes[fd.mode]);  h = 6;  break;
			
			case 3:  // extra  // todo: set rpm, pwm auto ?
				dtostrf(f.rpm / 60.f, 4,1, b);
				sprintf(a,"rps:  %s", b);  h = 4;  break;
			case 4:
				sprintf(a,"Rpm:  %4d", f.rpm);  h = 2;  break;
			}	break;
		
		case 1:  // page 2  name, temp
			switch (i)
			{
			case 0:
				sprintf(a,"Name: %s", fanNames[fd.name]);  h = 2;  break;
			case 1:
				sprintf(a,"Number: %d", fd.number);  break;
			case 2:
				if (fd.tempId >= 0 && fd.tempId < tempCount)
					sprintf(a,"Temp id: %d", fd.tempId);
				else
					sprintf(a,"Temp id: off");
				break;
			#ifdef TEMP_PIN
			case 3:
				if (fd.tempId >= 0 && fd.tempId < tempCount)
				{
					dtostrf(fTemp[fd.tempId], 4,1, b);
					sprintf(a,"Temp \x01""C: %s", b);
				}else
					sprintf(a,"Temp \x01""C: off");
				break;
			#endif
			}	break;

		case 2:  // page 3  auto %
			switch (i)
			{
			case 0:
				sprintf(a,"Auto: %s", fd.a.on ? "on" : "off");  break;
			case 1:
				sprintf(a,"min \x01""C: %d", fd.a.tempMin/10);  h = 2;  break;
			case 2:
				dtostrf(100.f * fd.a.pwmMin / 4095.f, 3,1, b);
				sprintf(a,"min %%: %s", b);  break;
			case 3:
				sprintf(a,"max \x01""C: %d", fd.a.tempMax/10);  h = 2;  break;
			case 4:
				dtostrf(100.f * fd.a.pwmMax / 4095.f, 3,1, b);
				sprintf(a,"max %%: %s", b);  break;
			case 5:
				sprintf(a,"exp: %d", fd.a.exp);  break;
			}	break;

		case 3:  // page 4  rpm guard
			switch (i)
			{
			case 0:
				sprintf(a,"Avg: %d", fd.avgNum);  break;
			case 1:
				sprintf(a,"Guard: %s", fd.g.on ? "on" : "off");  break;
			case 2:
				sprintf(a,"Rpm min: %d", fd.g.rpmMin);  break;
			case 3:
				sprintf(a,"On ms: %d", fd.g.msOn);  h = 2;  break;
			case 4:
				dtostrf(100.f * fd.g.pwmOn / 4095.f, 3,1, b);
				sprintf(a,"On %%: %s", b);  break;
			}	break;

		case 4:  // page 5  graphs
			a[0]=0;
			switch (i)
			{
			}	break;

		}
		d->print(a);  y += h+8;
	}
}
