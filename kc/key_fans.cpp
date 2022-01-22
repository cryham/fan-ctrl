#include "gui.h"
#include "kc_data.h"


int8_t Gui::FanDetLines[Gui::FanDetPages] =
	{2, 2, 5, 4,-1};  // param lines on each fan details page
int8_t Gui::FanDetLinExt[Gui::FanDetPages] =
	{2, 1, 0, 0, 0};  // extra lines, info not param

const uint16_t Gui::tFanAdd[NumFanAdd] = {  // pwm add speeds
	1, 2, 4, 9, 17, 25, 33, 41, 62, 82, 123, 164, 205, 246, 327, 409, 512,	};


//  Keys Fans
//....................................................................................
void Gui::KeysFans()
{
	Fan& f = kc.fans.fan[ym2Fan];
	FanData& fd = f.fd;
	auto& p = fd.pwm;
	auto add = tFanAdd[par.iFanAdd];

	if (mlevel == 2)
	{
		if (kUp)
			yFanDet[pgDet] = RangeAdd(yFanDet[pgDet], kUp, 0, FanDetLines[pgDet], 1);
		if (kPgUp)
			pgDet = RangeAdd(pgDet, kPgUp, 0, FanDetPages-1, 1);

		auto yy = yFanDet[pgDet];
		switch (pgDet)
		{
		case FD_PowerRpm:
			switch (yy)  // page 1  %, mode
			{
			case 0:
				p = RangeAdd(p, kRight * add, 0, 4095, 1);  break;  // pwm
			case 1:
				par.iFanAdd = RangeAdd(par.iFanAdd, kRight, 0, NumFanAdd-1, 1);  break;
			case 2:
				fd.mode = RangeAdd(fd.mode, kRight, 0, FModes_All-1, 1);  break;
			}	break;

		case FD_NameTemp:
			switch (yy)  // page 2  name, temp
			{
			case 0:
				fd.name = RangeAdd(fd.name, kRight, 0, FNames_All-1, 1);  break;
			case 1:
				fd.number = RangeAdd(fd.number, kRight, 0, 9, 1);  break;
			#ifdef TEMP_PIN
			case 2:
				if (kRight < 0 && fd.tempId == 0)
					fd.tempId = -1;  // off
				else
					fd.tempId = RangeAdd(fd.tempId, kRight, -1, tempCount-1, 1);  break;
			#endif
			}	break;

		case FD_Auto:
			switch (yy)  // page 3  auto %
			{
			case 0:
				fd.a.on = RangeAdd(fd.a.on, kRight, 0, 1);  break;
			case 1:
				fd.a.tempMin = RangeAdd(fd.a.tempMin, kRight*10, 0, 900, 1);  break;
			case 2:
				fd.a.pwmMin = RangeAdd(fd.a.pwmMin, kRight * add, 0, 4095, 1);  break;
			case 3:
				fd.a.tempMax = RangeAdd(fd.a.tempMax, kRight*10, 0, 900, 1);  break;
			case 4:
				fd.a.pwmMax = RangeAdd(fd.a.pwmMax, kRight * add, 0, 4095, 1);  break;
			case 5:
				fd.a.exp = RangeAdd(fd.a.exp, kRight*4, 10, 255, 1);  break;
			}	break;

		case FD_Guard:
			switch (yy)  // page 4  rpm guard
			{
			case 0:
				fd.avgNum = RangeAdd(fd.avgNum, kRight, 0, avgMax-1);  break;
			case 1:
				fd.g.on = RangeAdd(fd.g.on, kRight, 0, 1);  break;
			case 2:
				fd.g.rpmMin = RangeAdd(fd.g.rpmMin, kRight * 10, 0, 600);  break;
			case 3:
				fd.g.msOn = RangeAdd(fd.g.msOn, kRight * 10, 0, 2000);  break;
			case 4:
				fd.g.pwmOn = RangeAdd(fd.g.pwmOn, kRight * add, 0, 4095);  break;
			}	break;

		case FD_Graphs:
			switch (yy)  // page 5  graphs
			{	//?
			}	break;
		}
		return;
	}

	if (kUp)
		ym2Fan = RangeAdd(ym2Fan, kUp, 0, NumFans-1, 1);
	else
	if (kRight)
		p = RangeAdd(p, kRight * add, 0, 4095, 1);  // add normal
	else if (kPgUp)
		p = RangeAdd(p, kPgUp * 40 * 4, 0, 4095, 1);  // add page
}
