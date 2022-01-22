#include "gui.h"
#include "kc_data.h"
#include "Ada4_ST7735.h"


const char* sPgDisplay[Di_All] = {
	"Bright", "Gui keys", "Graph", "Temp""\x01""C" };


//  Display
//....................................................................................
void Gui::DrawDisplay()
{		
	char a[64],b[32];
	d->setClr(29,28,6);
	d->print(strMain[ym]);  d->setFont(0);

	//  subtitle
	d->setCursor(W/2, 4);
	d->setClr(30,22,12);
	d->print(sPgDisplay[pgDisp]);

	//  page
	d->setCursor(W-1 -3*6, 4);
	d->setClr(28,22,10);
	sprintf(a,"%d/%d", pgDisp+1, Di_All);
	d->print(a);


	//  par values  ---
	int pg = DispPages[pgDisp];
	int16_t y = 32;


	switch (pgDisp)
	{
	case Di_Bright:
	for (int i=0; i <= pg; ++i)
	{
		DrawDispCur(i, y);
		int8_t h = 4;
		switch(i)
		{
		case 0:
			sprintf(a,"Brightness:  %d %%", par.brightness);  break;
		case 1:
			sprintf(a,"Bright Off:  %d %%", par.brightOff);  h = 2;  break;
		case 2:
			d->print("  Time Off:  ");
			if (par.timeOff > 4)
				PrintInterval(tTmOff(par));
			else  d->print("always on");  break;
		case 3:
			StrScreen(par.startScreen, b);
			sprintf(a,"Start with:  %s", b);  break;
		}
		if (i!=2)
			d->print(a);  y += h+8;
	}	break;

	case Di_Key:
	for (int i=0; i <= pg; ++i)
	{
		DrawDispCur(i, y);
		int8_t h = 4;
		switch(i)
		{
		case 0:
			d->print("Frames per sec: ");
			switch(demos.iFps)
			{
			case 0:  strcpy(a,"off");  break;
			case 1:  strcpy(a,"in demos");  break;
			case 2:  strcpy(a,"always");  break;
			}	break;
		case 1:
			sprintf(a,"Key delay:  %d ms", par.krDelay*5);  h = 2;  break;
		case 2:
			sprintf(a,"Key repeat: %d ms", par.krRepeat*5);  break;
		}
		d->print(a);  y += h+8;
	}	break;

	case Di_Graph:
	for (int i=0; i <= pg; ++i)  // extra
	{
		DrawDispCur(i, y);
		int8_t h = 4;
		switch(i)
		{
		case 0:
			d->print("Time Rpm add:  ");  PrintInterval(tRpm(par));  h = 2;  break;
		case 1:  // extra
			d->print(" Graph total: ");  PrintInterval(W * tRpm(par));  break;
			//sprintf(a,"Time for 1min:  %dm%02ds", t1min(par)/60, t1min(par)%60);  break;
		case 2:
			d->print("   Temp read:  ");  PrintInterval(tTemp(par));  h = 2;  break;
		case 3:
			d->print("   Graph add:  ");  PrintInterval(tTgraph(par));  h = 2;  break;
		case 4:  // extra, same as rpm?
			d->print(" Graph total: ");  PrintInterval(W * tTgraph(par));  break;
		}
		y += h+8;
	}	break;

	case Di_Temp:
	for (int i=0; i <= pg; ++i)
	{
		DrawDispCur(i, y);
		int8_t h = 4;
		switch(i)
		{
		case 0:
			d->print("Reset sensors");  break;
		case 1:
			d->print("Temp offset: ");
			dtostrf(0.03f * par.tempOfs, 4,2, a);
			d->print(a);  d->print(" ""\x01""C");  break;
		case 2:
			sprintf(a,"T min:  %d ""\x01""C", par.minTemp);  d->print(a);  h = 2;  break;
		case 3:
			sprintf(a,"T max:  %d ""\x01""C", par.maxTemp);  d->print(a);  break;
		}
		y += h+8;
	}	break;
	}
}
