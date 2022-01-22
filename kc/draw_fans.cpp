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

		if (hid)  //  fan line  ---
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
					RpmClr(&f);
					d->setCursor(79, y);  d->print(a);
				}
				//  line --
				/*auto cl = d->getClr();
				d->drawFastHLine(0, y+ya-1, f.pwm/4095.f*(W-1), cl);*/

				//  temp 'C
				if (TIdOk(f.fd.tempId))
				{
					float fT = fTemp[f.fd.tempId];
					if (fT > 0.1f)
					{
						ClrByte(TempFtoB(fT));
						dtostrf(fT, 4,1, a);
						d->setCursor(123, y);  d->print(a);
				}	}
		}	}
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


void Gui::RpmClr(const Fan* f)
{
	if (f->tmLeft > 0)  // guard triggered
		d->setClr(25,27,30);
	else
		ClrByte(f->Rpm2B(f->rpmAvg));
}
