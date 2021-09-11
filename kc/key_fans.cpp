#include "gui.h"
#include "fans.h"

extern Fans fans;


//  Keys Fans
//....................................................................................
void Gui::KeysFans()
{
	if (kUp)
		ym2Fan = RangeAdd(ym2Fan, kUp, 0, NumFans-1, 1);
	else
	if (kRight)
	{
		int sp = fansFine ? 20 : 40 * 4;
		auto& i = fans.fan[ym2Fan].pwm;
		i = RangeAdd(i, kRight * sp, 0, 4095, 1);
		fans.SetPWM(ym2Fan, i);
	}
	else if (kEnt)  // toggle fine adjust
		fansFine = !fansFine;
}
