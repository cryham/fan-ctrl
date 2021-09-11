#include "gui.h"
#include "fans.h"
#include "Ada4_ST7735.h"

extern Fans fans;


//  Fans
//....................................................................................
void Gui::DrawFans()
{
	//  title
	d->setClr(20,20,26);
	d->print(strMain[ym]);  d->setFont(0);
	d->setClr(18,18,24);

	char a[64];
	int16_t y = 32;

	//  legend
	d->setCursor(2,y);
	sprintf(a,"  PWM %%  rps  avg  Rpm");
	d->print(a);
	y += 12;

	d->setCursor(W/2,4);
	d->print(fansFine ? "fine":"normal");

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
		int p = 100 * f.pwm / 4095;
		if (f.noRpm)
			sprintf(a,"%3d %%", p);
		else
			//sprintf(a,"%3d %lu %lu %d", p, f.rpmAvg/60, f.rpmAvg, f.pulses);
			//sprintf(a,"%3d %% %2d %4d  %d", p, f.rpmAvg/60, f.rpmAvg, f.rpm);
			sprintf(a,"%3d %% %2d %4d", p, f.rpmAvg/60, f.rpmAvg);

		d->print(a);  y += 8+2;
	}
}
