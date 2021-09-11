#include "gui.h"
#include "fans.h"
#include "Ada4_ST7735.h"

extern Fans fans;


//  Setup
//....................................................................................
void Gui::DrawFans()
{
	//  title
	d->setClr(21,23,25);
	d->print(strMain[ym]);  d->setFont(0);
	d->setClr(18,22,26);

	char a[64];
	int16_t y = 32;

	//  legend
	d->setCursor(2,y);
	sprintf(a,"  PWM %%  Rpm  rps");
	d->print(a);
	y += 12;

	//-----------------------------------------------------
	for (int i=0; i < NumFans; ++i)
	{
		d->setCursor(2,y);
		int c = abs(i - ym2Fan);
		if (!c)
		{	d->setClr(10,30,30);
			d->fillRect(0, y-1, W-1, 10, RGB(3,5,5));
			d->print("\x10 ");  // >
		}else
			d->print("  ");

		FadeClr(C_Setup, 4, c, 2);

		//  fam lines
		const Fan& f = fans.fan[i];
		sprintf(a,"%3d %%  %d  %d  %d",
			100 * f.pwm / 4095, f.rpmAvg/60, f.rpmAvg, f.rpm);

		d->print(a);  y += 8+2;
	}
}
