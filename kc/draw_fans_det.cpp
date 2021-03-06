#include "gui.h"
#include "kc_data.h"

#include "Ada4_ST7735.h"
#include "FreeSans9pt7b.h"


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
	case FD_PowerRpm:  // pwm, rpm
		DrawGraphR(0, W-1,  0, H-1,  false, id);  break;

	case FD_NameTemp:  // temp
		if (fd.tempId >= 0)
		DrawGraphT(0, W-1,  0, H-1,  false, id);  break;

	case FD_Auto:
		if (f.fd.a.on)
		{
			DrawAutoGraph(&f);
		
			d->setFont(0);  //  extra vars right
			d->setClr(24,24,28);
			d->setCursor(110, 60);  // %
			
			dtostrf(100.f * f.pwm / 4095.f, 3,1, b);
			sprintf(a,"%% %s", b);  d->print(a);

			if (TIdOk(fd.tempId))  // 'C
			{
				ClrByte(TempFtoB(fTemp[fd.tempId]));
				d->setCursor(110, 72);

				dtostrf(fTemp[fd.tempId], 4,1, b);
				sprintf(a,"\x01""C %s", b);  d->print(a);
		}	}
		break;

	case FD_Graphs:
		if (fd.tempId >= 0)  // rpm & temp
		{	DrawGraphR(0, W-1,  0,   H/2,  true, id);
			DrawGraphT(0, W-1,  H/2, H-1,  true, id);
		}else  // rpm
			DrawGraphR(0, W-1,  0,   H-1,  true, id);
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
		case FD_PowerRpm:  // page 1  %, rpm
			switch (i)
			{
			case 0:
			{	auto p = fd.a.on ? f.pwm : fd.pwm;
				dtostrf(100.f * p / 4095.f, 3,1, b);  h = 2;
				if (fd.a.on)
					sprintf(a," Auto%% %s", b);
				else
					sprintf(a,"Power%% %s", b);
			}	break;
			case 1:
				dtostrf(100.f * tFanAdd[par.iFanAdd] / 4095.f, 3,1, b);
				sprintf(a,"  add%% %s", b);  h = 4;  break;
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
				RpmClr(&f);
				sprintf(a,"Rpm:  %4d", f.rpm);  h = 2;  break;
			}	break;
		
		case FD_NameTemp:  // page 2  name, temp
			switch (i)
			{
			case 0:
				sprintf(a,"Name: %s", fanNames[fd.name]);  h = 2;  break;
			case 1:
				sprintf(a,"Number: %d", fd.number);  h = 6;  break;
			case 2:
				if (TIdOk(fd.tempId))
					sprintf(a,"Temp id: %d", fd.tempId);
				else
					sprintf(a,"Temp id: off");
				break;
			case 3:
				if (TIdOk(fd.tempId))
				{
					ClrByte(TempFtoB(fTemp[fd.tempId]));
					
					dtostrf(fTemp[fd.tempId], 4,1, b);
					sprintf(a,"Temp \x01""C: %s", b);
				}else
					sprintf(a,"Temp \x01""C: off");
				break;
			}	break;

		case FD_Auto:  // page 3  auto %
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
				dtostrf(fd.a.exp /100.f, 3,2, b);
				sprintf(a,"exp ^: %s", b);  break;
			}	break;

		case FD_Guard:  // page 4  rpm guard
			switch (i)
			{
			case 0:
				sprintf(a,"Guard: %s  %s", fd.g.on ? "on" : "off",
					f.tmLeft > 0 ? "ON" : "");  break;
			case 1:
				sprintf(a,"Rpm min: %d", fd.g.rpmMin);  break;
			case 2:
				sprintf(a,"On ms: %d", fd.g.msOn);  h = 2;  break;
			case 3:
				dtostrf(100.f * fd.g.pwmOn / 4095.f, 3,1, b);
				sprintf(a,"On %%: %s", b);  break;
			}	break;

		case FD_Advanced:  // page 5  advanced
			switch (i)
			{
			case 0:
				sprintf(a,"Averages: %d", fd.avgNum);  break;
			case 1:
				sprintf(a,"PWM Freq.: %d kHz", fd.freq);  break;
			}	break;

		case FD_Graphs:  // page 6  graphs
			a[0]=0;
			break;

		}
		d->print(a);  y += h+8;
	}
}
