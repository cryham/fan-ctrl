#include "gui.h"
#include "Ada4_ST7735.h"
#include "FreeSans9pt7b.h"


//  Draw  main
//....................................................................................
void Gui::Draw()
{
	yy = ym1[ym];

	//  Clear
	#ifdef DEMOS
	bool demo = ym == M_Demos && mlevel == 2;
	bool no = demo && (yy == D_Rain || yy == D_Plasma || yy == D_Wave || yy == D_Fire);
	if (!no)  //  if not full screen demo
	#endif
		d->clear();

	if (!demo)
	{
		d->setFont(&FreeSans9pt7b);
		d->setCursor(0,0);
	}


	//  Main menu
	//------------------------------------------------------
	if (mlevel==0)
	{
		d->setClr(6,19,31);
		d->print("Main Menu");  d->setFont(0);

		DrawMenu(M_All,strMain, C_Main,RGB(20,25,29),RGB(5,7,9), 10, M_Next);
		return;
	}
	d->setClr(12,22,31);


	switch (ym)
	{
	//  Demos
	//------------------------------------------------------
	#ifdef DEMOS
	case M_Demos:
		if (mlevel == 2)
		{
			d->setFont(0);
			switch (yy)
			{
			#ifdef DEMOS_PLASMA
				case D_Plasma:   demos.Plasma();  break;  // clr 55, 40 ms
			#endif
				case D_Wave:     demos.Wave();  break;
				case D_Fire:     demos.Fire();  break;
				case D_CK_Logo:  demos.CK_logo();  break;  // 7
			#ifdef DEMOS_3D
				case D_Hedrons:  demos.Hedrons();  break;  // 5-9ms
			#endif
				case D_Ngons:    demos.Ngons();  break;  // 12 8ms 14 10ms
				case D_Fonts:	 demos.Fonts();  break;

				case D_Rain:     demos.Rain();  break;
			}
		}else
		{	//  menu
			d->setClr(25,16,28);
			d->print(strMain[ym]);  d->setFont(0);

			DrawMenu(D_All,strDemo, C_Demos,RGB(27,27,30),RGB(6,6,9), 10, D_Next);
		}
		return;
	#endif

	//  Fans
	case M_Fans:    
		if (mlevel == 2)
					DrawFanDetails();
		else		DrawFans();  return;
	case M_Keys:    DrawKeys();  return;
	case M_Config:  DrawConfig();  return;

	//  Display, Help
	case M_Display: DrawDisplay();  return;
	case M_Graphs:  DrawGraphs();  return;
	case M_Help:    DrawHelp();  return;
	}
}
