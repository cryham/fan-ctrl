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
		bool hid = f.fd.mode == FM_Hide || (
				   f.fd.mode == FM_ExtOn && !kc.fans.ext_on);
		int ya = hid ? 6 : 16;

		//  cur
		int c = abs(i - ym2Fan);
		if (!c)
		{	d->setClr(10,30,30);
			d->fillRect(0, y-1, W-1, ya, RGB(3,5,5));
		}
		FadeClr(C_Keys, 4, c, 2);

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
			{	//  pwm
				/*if (f.fd.pwm < 400)  // 10%
					dtostrf(100.f * f.fd.pwm / 4095.f, 3,1, a);
				else*/
					sprintf(a,"%2d", 100 * f.fd.pwm / 4095);
				
				d->setCursor(58, y);  d->print(a);

				//  rps rpm
				if (!f.noRpm)
				{
					sprintf(a,"%2d %4d", f.rpmAvg/60, f.rpmAvg);  //f.pulses, f.rpm);
					d->setCursor(90, y);  d->print(a);
			}	}
		}
		y += ya-1;
	}

	//  legend
	d->setFont(0);
	d->setClr(10,16,22);
	d->setCursor(0,0);
	sprintf(a,"Fan Name  PWM%%  rps  Rpm");
	d->print(a);
}


//  Fan Details
//....................................................................................
void Gui::DrawFanDetails()
{
	char a[64],b[32];
	Fan& f = kc.fans.fan[ym2Fan];

	//  graph  backgr big
	if (pgDet == 0)		//  rpm graph ~
		DrawGraph(0, W-1,  0, H-1,  0, false, ym2Fan);
	else
	if (f.fd.temp >= 0)	//  temp graph ~
		DrawGraph(0, W-1,  0, H-1,  1, false, f.fd.temp);


	//  title
	d->setClr(18,18,24);
	d->setCursor(0,0);
	d->setFont(&FreeSans9pt7b);

	f.GetFanName(b);
	sprintf(a,"%d: %s", ym2Fan+1, b);

	d->print(a);
	d->setFont(0);

	
	//  page
	d->setClr(18,18,24);
	d->setCursor(W-1 -3*6, 4);
	sprintf(a,"%d/%d", pgDet+1, FanDetPages);
	d->print(a);


	//  param values  ---
	int ln = FanDetLines[pgDet]
		+ (pgDet==0 ? 2: 1);  // extra
	int16_t y = 32;

	for (int i=0; i <= ln; ++i)
	{
		d->setCursor(2, y);
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
		case 0:  // page 0  pwm, rpm
			switch (i)
			{
			case 0:
				dtostrf(100.f * f.fd.pwm / 4095.f, 3,1, b);
				sprintf(a,"PWM%%: %s", b);  h = 2;  break;
			case 1:
				dtostrf(100.f * tFanAdd[par.iFanAdd] / 4095.f, 3,1, b);
				sprintf(a,"add%%: %s", b);  h = 4;  break;
			case 2:
				if (f.fd.mode == FM_ExtOn)
					sprintf(a,"Mode: %s %s", fanModes[f.fd.mode],
						kc.fans.ext_on ? "on" : "off");
				else
					sprintf(a,"Mode: %s", fanModes[f.fd.mode]);  h = 6;  break;
			
			case 3:  // extra  // todo: set rpm, pwm auto ?
				dtostrf(f.rpm / 60.f, 4,1, b);
				sprintf(a,"rps:  %s", b);  h = 4;  break;
			case 4:
				sprintf(a,"Rpm:  %4d", f.rpm);  h = 2;  break;
			}	break;
		
		case 1:  // page 1  name
			switch (i)
			{
			case 0:
				sprintf(a,"Name: %s", fanNames[f.fd.name]);  h = 2;  break;
			case 1:
				sprintf(a,"Number: %d", f.fd.number);  break;
			case 2:
				sprintf(a,"Temp id: %d", f.fd.temp);  break;
			case 3:
				dtostrf(fTemp[f.fd.temp], 4,1, b);
				sprintf(a,"Temp \x01""C: %s", b);  break;
			}	break;

		// todo: lin  temp1min rpm1  temp2 rpm2max
		//case 2:  // page 2  auto
		}
		d->print(a);  y += h+8;
	}
}
