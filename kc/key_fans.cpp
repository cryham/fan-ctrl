#include "gui.h"
#include "kc_data.h"


int8_t Gui::FanDetLines[Gui::FanDetPages] = {2,2};  // lines on each fan details page
const uint16_t Gui::tFanAdd[NumFanAdd] = {
	1, 2, 4, 8, 16, 24, 32, 40, 60, 80, 120, 160, 200, 240, 320, 400, 512,	};

//  Keys Fans
//....................................................................................
void Gui::KeysFans()
{
	Fan& f = kc.fans.fan[ym2Fan];
	auto& p = f.fd.pwm;

	if (mlevel == 2)
	{
		if (kUp)
			yFanDet[pgDet] = RangeAdd(yFanDet[pgDet], kUp, 0, FanDetLines[pgDet], 1);
		if (kPgUp)
			pgDet = RangeAdd(pgDet, kPgUp, 0, FanDetPages-1, 1);

		auto yy = yFanDet[pgDet];
		switch (pgDet)
		{
		case 0:
			switch (yy)  // page 0
			{
			case 0:
				p = RangeAdd(p, kRight * tFanAdd[par.iFanAdd], 0, 4095, 1);  break;  // add
			case 1:
				par.iFanAdd = RangeAdd(par.iFanAdd, kRight, 0, NumFanAdd-1, 1);  break;  // add speed
			case 2:
				f.fd.mode = RangeAdd(f.fd.mode, kRight, 0, FModes_All-1, 1);  break;
			}	break;
		case 1:
			switch (yy)  // page 1
			{
			case 0:
				f.fd.name = RangeAdd(f.fd.name, kRight, 0, FNames_All-1, 1);  break;
			case 1:
				f.fd.number = RangeAdd(f.fd.number, kRight, 0, 9, 1);  break;
			#ifdef TEMP_PIN
			case 2:
				f.fd.temp = RangeAdd(f.fd.temp, kRight, -1, tempCount-1, 1);  break;
			#endif
			}	break;
		}
		return;
	}

	if (kUp)
		ym2Fan = RangeAdd(ym2Fan, kUp, 0, NumFans-1, 1);
	else
	if (kRight)
	{
		p = RangeAdd(p, kRight * 40 * 4, 0, 4095, 1);  // add normal
	}
	else if (kMid)
		f.fd.mode = RangeAdd(f.fd.mode, 1, 0, FModes_All, 1);
}
