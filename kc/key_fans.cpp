#include "gui.h"
#include "kc_data.h"


//  Keys Fans
//....................................................................................
void Gui::KeysFans()
{
	Fan& f = kc.fans.fan[ym2Fan];
	auto& p = f.fd.pwm;

	if (mlevel == 2)
	{
		if (kUp)
			yFanDet = RangeAdd(yFanDet, kUp, 0, NumFanDet, 1);
		if (kPgUp)
			kFanAdd = RangeAdd(kFanAdd, kPgUp * 5, 0, 40*40, 1);  // add adjust []?

		switch (yFanDet)
		{
		case 0:
			f.fd.name = RangeAdd(f.fd.name, kRight, 0, FN_All-1, 1);  break;
		case 1:
			f.fd.number = RangeAdd(f.fd.number, kRight, 0, 9, 1);  break;

		case 2:
			f.fd.mode = RangeAdd(f.fd.mode, kRight, 0, FM_All-1, 1);  break;
		case 3:
			p = RangeAdd(p, kRight * kFanAdd, 0, 4095, 1);  break;  // add
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
		f.fd.mode = RangeAdd(f.fd.mode, 1, 0, FM_All, 1);
}
