#include "gui.h"
#include "Ada4_ST7735.h"
#include "usb_keyboard.h"
#include "matrix.h"
#include "kc_data.h"


//  Info use,ver
//....................................................................................
void Gui::DrawConfig()
{
	//  menu
	if (mlevel == 1)
	{
		d->setClr(22,20,26);
		d->print(strMain[ym]);  d->setFont(0);

		DrawMenu(C_All,strConfig, C_Config,RGB(22,20,28),RGB(4,4,8), 10, -1);
		return;
	}
	char a[64];
	int16_t y = 32;


	if (yy == C_Version)
	{	demos.Version();  return;  }

	//  title
	d->setClr(17,17,22);
	d->print(strConfig[yy]);
	d->setFont(0);
	d->setClr(21,21,26);


	//int ii = InfoPages[yy];
	switch (yy)
	{
	//-----------------------------------------------------
	case C_Use:  // use
	{
		y -= 4;
		d->setClr(15,23,30);
		d->setCursor(0, y);
		d->print("\x10 ");  // >

		d->setClr(16,20,24);
		sprintf(a,"Save counter: %d", par.verCounter);
		d->print(a);  y += 14;

	}	break;
	}
}
