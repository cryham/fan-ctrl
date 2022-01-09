#include "gui.h"
#include "matrix.h"
#include "kc_params.h"
#include "kc_data.h"
#include "periodic.h"


//  Key press
//....................................................................................
void Gui::KeyPress()
{
	uint32_t ti = millis();
	uint16_t dt = ti - oldti_kr;
	oldti_kr = ti;

	//  update keys press vars
	kRight = kr(5,dt) - kr(3,dt);
	kUp = kr(7,dt) - kr(1,dt);
	kBack = kr(0,dt);
	kEnt = Key(2);
	
	kSave = Key(6);  // save/menu?
	kPgUp = kr(8,dt) - kr(2,dt);
	kEnd = Key(8);  //-
	kMid = Key(4);
	
	int d = kUp + kRight;


	//  ---
	if (ym == M_Fans && mlevel >= 1)
	{
		KeysFans();
	}
	else if (ym == M_Keys && mlevel == 2)
	{
		KeysScan();
	}
	else if (ym == M_Config && mlevel > 0)
	{
		KeysConfig();
	}
	else if (ym == M_Display && mlevel == 1)
	{
		KeysDisplay();
	}
	else if (ym == M_Graphs && mlevel == 1)
	{
		KeysGraph();
	}


	//  main menu
	if (mlevel == 0)
	{
		if (d)
		{	ym += d;
			if (ym >= M_All)  ym = 0;
			if (ym < 0)  ym = M_All-1;
		}
		if (kEnt > 0)
			mlevel = 1;  // enter>
		return;
	}


	//  <back  global
	if (kBack && mlevel > 0)
		--mlevel;


	//  Help  ---
	if (ym == M_Help && mlevel == 1)
	{
		if (d)
			hpage = RangeAdd(hpage, d, 0,HAll-1, 1);
		return;
	}


	//  sub menu
	if (mlevel == 1)
	{
		auto No2nd = [&](){  return  // no 2nd level
			ym == M_Display ||
			(ym == M_Config && yy < 2);  };

		if (kEnt > 0 && !No2nd())  // enter>
			mlevel = 2;

		if (d)
		{	ym1[ym] += d;  Chk_y1();  }
		return;
	}


	//  Demos  ----
	#ifdef DEMOS
	if (ym == M_Demos && mlevel == 2)
	{
		demos.KeyPress((EDemo)ym1[ym], this);
		return;
	}
	#endif
}
