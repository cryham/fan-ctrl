#include "gui.h"
#include "kc_params.h"
#include "kc_data.h"
extern KC_Main kc;


//  Clock
//....................................................................................
void Gui::KeysGraph()
{
/*	if (kUp)  // y
	{	ym2Clock = RangeAdd(ym2Clock, kUp, 0, ClockVars(pgClock), 1);  }
	else
	if (kPgUp)  // pg
	{	pgClock = RangeAdd(pgClock, kPgUp, 0, Cl_All-1, 1);
		ym2Clock = RangeAdd(ym2Clock, 0, 0, ClockVars(pgClock), 1);
	}
	else
	//  adjust time  ---
	if (kRight && pgClock == Cl_Adjust)
	{
		unsigned long tm = rtc_get(), td = 0;
		int a = kRight * (kCtrl ? 10 : 1);
	}
	//  graphs  cursor move  ---
	else if (pgClock == Cl_Graphs)
	{
		if (kEnd)
			par.xCur = W-1;  // to end
		if (kMul)
		{	if (par.xCur == W)  par.xCur = W-1;  // show
			else  par.xCur = W;  // hide
		}
		int a = kRight * (kCtrl ? 30 : kSh ? 1 : 10);
		if (a)
			par.xCur = RangeAdd(par.xCur, a, 0, W-1, 1);
	}

	if (kBack)  --mlevel;

	if (pgClock == Cl_Adjust || pgClock == Cl_Graphs)
	{
		if (kSave)  Save();
		if (kLoad)  Load(kCtrl);
	}
*/
}
