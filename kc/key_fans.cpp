#include "gui.h"
#include "kc_data.h"


//  Keys Fans
//....................................................................................
void Gui::KeysFans()
{
	Fan& f = kc.fans.fan[ym2Fan];

	if (kUp)
		ym2Fan = RangeAdd(ym2Fan, kUp, 0, NumFans-1, 1);
	else
	if (kMid)
	{
		f.fd.off = 1 - f.fd.off;
	}
	else if (kRight)
	{
		int sp = fansFine ? 20 : 40 * 4;
		auto& i = f.fd.pwm;
		i = RangeAdd(i, kRight * sp, 0, 4095, 1);
		//kc.fans.SetPWM(ym2Fan, i);  // always
	}
	else if (kEnt)  // toggle fine adjust
		fansFine = !fansFine;
}
