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

	//  update keys press  _k_
	kUp = kr(3,dt) - kr(2,dt);
	kBack = kr(1,dt);
	
	//  rot enc  (*)  scroll
	static int8_t old = KeyH(5);
	int8_t scr = KeyH(5);
	if (scr && !old)
		kRight = KeyH(4) > 0 ? -1 : 1;
	else
		kRight = Key(0);
	old = scr;

	//kEnt = Key(gEnt);  kSave = Key(gSave);


	if (ym == M_Fans && mlevel == 1)
	{
		KeysFans();
	}
	else if (ym == M_Keys && mlevel == 2)
	{
		KeysScan();
	}
	else if (ym == M_Config && mlevel == 2)
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


	if (mlevel == 0)  //  main menu
	{
		if (kUp){  ym += kUp;  if (ym >= M_All)  ym = 0;  if (ym < 0)  ym = M_All-1;  }
		if (kRight > 0)  mlevel = 1;  // enter>
		return;
	}


	//  <back global
	if (kBack && mlevel > 0)
		--mlevel;


	//  Help
	if (ym == M_Help && mlevel == 1)
	{
		if (kUp || kPgUp)
			hpage = RangeAdd(hpage, kUp+kPgUp, 0,HAll-1, 1);
		return;
	}


	if (mlevel == 1)  //  sub menu
	{
		//  enter>
		if (kRight > 0)
			if (ym != M_Display && ym != M_Fans)  // no 2nd level
				mlevel = 2;

		if (kUp){  ym1[ym] += kUp;  Chk_y1();  }
		return;
	}


	//  Demos
	#ifdef DEMOS
	if (ym == M_Demos && mlevel == 2)
	{
		demos.KeyPress((EDemo)ym1[ym], this);
		return;
	}
	#endif
}
