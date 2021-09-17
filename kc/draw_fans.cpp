#include "gui.h"
#include "kc_data.h"
#include "Ada4_ST7735.h"


//  Fans
//....................................................................................
void Gui::DrawFans()
{
	//  title
	//d->setClr(20,20,26);
	//d->print(strMain[ym]);  //d->setFont(0);
	//d->setClr(18,18,24);

	char a[64], b[32];
	int16_t y = 16+4; //32;

	//-----------------------------------------------------
	for (int i=0; i < NumFans; ++i)
	{
		d->setCursor(2,y);
		int c = abs(i - ym2Fan);
		if (!c)
		{	d->setClr(10,30,30);
			d->fillRect(0, y-1, W-1, 16, RGB(3,5,5));
			//d->print("\x10 ");  // >
		}//else
			//d->print("  ");

		FadeClr(C_Setup, 4, c, 2);

		//  fan line
		const Fan& f = kc.fans.fan[i];

		int p = 100 * f.fd.pwm / 4095;

		if (f.fd.off)
			sprintf(b,"off");
		else
			sprintf(b,"%3d", p);

		if (f.noRpm)
			sprintf(a,"%s", b);
		else
			sprintf(a,"%s  %2d %4d", b, f.rpmAvg/60, f.rpmAvg);
			//sprintf(a,"%3d %lu %lu %d", p, f.rpmAvg/60, f.rpmAvg, f.pulses);
			//sprintf(a,"%3d %% %2d %4d  %d", p, f.rpmAvg/60, f.rpmAvg, f.rpm);

		d->print(a);  y += 15; //8+2;
	}

	//  legend
	//d->setFont(0);
	d->setClr(10,16,22);
	d->setCursor(0,0);
	sprintf(a,"Fan%%  rps  Rpm");
	d->print(a);

	//  mode
	d->setFont(0);
	d->setClr(18,18,24);
	d->setCursor(W-7*6,4);
	d->print(fansFine ? "fine":"normal");
}
