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
		auto& i = fans.fan[ym2Fan].pwm;
		i = RangeAdd(i, kRight * 40, 0, 4095);
		fans.SetPWM(ym2Fan, i);
	}
}
