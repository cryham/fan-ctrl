#include "gui.h"
#include "kc_data.h"
#include "Ada4_ST7735.h"


//  Fans
//....................................................................................
void Gui::DrawFans()
{
	char a[64];
	int16_t y = 16+4;

	for (int i=0; i < NumFans; ++i)
	{
		const Fan& f = kc.fans.fan[i];
		bool hid = f.fd.mode == FM_Hide;
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
			if (f.fd.name < FN_All)  
			{	if (f.fd.number > 0)
					sprintf(a,"%s%d", fanNames[f.fd.name], f.fd.number);
				else
					sprintf(a,"%s", fanNames[f.fd.name]);
				d->setCursor(2, y);  d->print(a);
			}

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
	//  title
	d->setClr(18,18,24);
	d->setCursor(0,0);
	sprintf(a,"Fan %d", ym2Fan+1);
	d->print(a);
	d->setFont(0);

	//  param values  ---
	int pg = NumFanDet + 2;  // extra
	int16_t y = 32;

	//  rpm graph -
	DrawGraph();

	//  add speed
	d->setClr(14,14,20);
	d->setCursor(82, 4);
	dtostrf(100.f * kFanAdd / 4095.f, 4,1, b);
	sprintf(a,"Add: %s%%", b);
	d->print(a);

	for (int i=0; i <= pg; ++i)
	{
		Fan& f = kc.fans.fan[ym2Fan];

		d->setCursor(2, y);
		int c = abs(i - yFanDet);  // dist dim
		if (!c)
		{	//d->fillRect(0, y-1, W/2-1, 10, RGB(2,4,6));
			d->setClr(6,22,31);
			d->print("\x10 ");  // >
		}else
			d->print("  ");

		FadeClr(C_FanDet, 4, c, 1);

		int8_t h = 4;
		switch(i)
		{
		case 0:
			sprintf(a,"Name: %s", fanNames[f.fd.name]);  h = 2;  break;
		case 1:
			sprintf(a,"Number: %d", f.fd.number);  break;

		case 2:
			sprintf(a,"Mode: %s", fanModes[f.fd.mode]);  h = 2;  break;
		case 3:
			dtostrf(100.f * f.fd.pwm / 4095.f, 4,1, b);
			sprintf(a,"PWM%%: %s", b);  h = 6;  break;
		
		case 4:  // extra
			dtostrf(f.rpm / 60.f, 4,1, b);
			sprintf(a,"rps:  %s", b);  h = 4;  break;
		case 5:
			sprintf(a,"Rpm:  %4d", f.rpm);  h = 2;  break;
		}
		d->print(a);  y += h+8;
	}
}
