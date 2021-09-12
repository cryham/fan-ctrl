#include "gui.h"
#include "kc_params.h"
#include "kc_data.h"
extern KC_Main kc;


//  Clock
//....................................................................................
void Gui::KeysGraph()
{
	if (kUp)  // y
		pgGraph = RangeAdd(pgGraph, kPgUp, 0, G_All-1, 1);
	else
	//  graphs  cursor move  ---
	if (pgGraph == G_Graphs)
	{
		if (kPgUp)
			par.xCur = W-1;  // to end
		/*if (kMul)
		{	if (par.xCur == W)  par.xCur = W-1;  // show
			else  par.xCur = W;  // hide
		}*/
		int a = kRight * 10;  //(kCtrl ? 30 : kSh ? 1 : 10);
		if (a)
			par.xCur = RangeAdd(par.xCur, a, 0, W-1, 1);
	}
}
