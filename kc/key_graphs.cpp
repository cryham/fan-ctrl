#include "gui.h"
#include "kc_params.h"
#include "kc_data.h"
extern KC_Main kc;


//  Clock
//....................................................................................
void Gui::KeysGraph(int d)
{
	if (d)  // y
		par.pgGraph = RangeAdd(par.pgGraph, d, 0, G_All-1, 1);
	#if 0
	else
	//  cursor move  ---
	if (pgGraph == G_Graphs)
	{
		if (kPgUp)
			par.xCur = W-1;  // to end
		int a = kRight * 10;
		if (a)
			par.xCur = RangeAdd(par.xCur, a, 0, W-1, 1);
	}
	#endif
}
