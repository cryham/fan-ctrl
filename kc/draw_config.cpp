#include "gui.h"
#include "Ada4_ST7735.h"
#include "usb_keyboard.h"
#include "matrix.h"
#include "kc_data.h"


//  Info use,ver
//....................................................................................
void Gui::DrawConfig()
{
	char a[64];

	//  menu
	if (mlevel == 1)
	{
		//  title
		d->setClr(22,20,26);
		d->print(strMain[ym]);


		//  uptime  ---
		unsigned long t = rtc_get() - kc.tm_on;
		int h = t / 3600 % 24, m = t / 60 % 60, s = t % 60;

		d->setCursor(36, H - 20);
		d->setClr(16, 16, 21);
		sprintf(a, "%d:%02d:%02d", h, m, s);  d->print(a);


		//  menu
		d->setFont(0);
		DrawMenu(C_All,strConfig, C_Config,RGB(22,20,28),RGB(4,4,8), 10, -1);


		//  counter
		d->setClr(15,23,30);
		d->setCursor(64, 32 + 2*10);

		sprintf(a,"%d", par.verCounter);
		d->print(a);
	}
	else if (yy == C_Version)
		demos.Version();
}
