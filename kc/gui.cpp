#include "gui.h"
#include "def.h"
#include "kc_data.h"


//  Main  ----
Gui::Gui() : d(0)
{
	Init(0);

	memset(grTemp, 0, sizeof(grTemp));
}

void Gui::Init(Ada4_ST7735* tft)
{
	d = tft;

	mlevel = 0;
	ym = 0;  yy = 0;
	for (int i=0; i < M_All; ++i)
		ym1[i]=0;


	oldti=0;  oldti_kr=0;
	hpage = 0;

#ifdef DEMOS
	demos.Init(d);
#endif

	ym2Fan=0; ym2Scan=0;
	ym2Disp=0; pgDisp=0;

	tInfo=0;  infType=0;
}


//  start screen  ---
void Gui::SetScreen(int8_t s)
{
	if (s == ST_Main0)
	{	mlevel = 0;  ym = 0;  return;  }
	else  mlevel = 1;

	switch (s)
	{
	case ST_Fans:   ym = M_Fans;  break;
	case ST_Keys:   ym = M_Keys;  break;
	case ST_Config: ym = M_Config;  break;

	case ST_Displ:  ym = M_Display;  break;
#ifdef DEMOS
	case ST_Demos:  ym = M_Demos;  break;
	}
	if (s >= ST_Demos2){  mlevel = 2;  ym = M_Demos;  ym1[ym] = s - ST_Demos2;  }  //else
#else
	}
#endif  // level2
	if (s >= ST_Keys2 && s < ST_Keys2Max){  mlevel = 2;  ym = M_Keys;    ym1[ym] = s - ST_Keys2;  }  else
	if (s >= ST_Graph && s < ST_GraphMax){  mlevel = 2;  ym = M_Graphs;  ym1[ym] = s - ST_Graph;  }  else
	if (s >= ST_Config2 && s < ST_Config2Max){  mlevel = 2;  ym = M_Config;  ym1[ym] = s - ST_Config2;  }
}

const char* Gui::StrScreen(int8_t s)
{
	switch (s)
	{
	case ST_Main0:  return "Main ""\x13";

	case ST_Fans:   return strMain[M_Fans];
	case ST_Keys:   return strMain[M_Keys];
	case ST_Config: return strMain[M_Config];

	case ST_Displ:  return strMain[M_Display];
#ifdef DEMOS
	case ST_Demos:  return strMain[M_Demos];
	}
	if (s >= ST_Demos2)  return strDemo[s - ST_Demos2];
#else
	}
#endif  // level2
	if (s >= ST_Keys2 && s < ST_Keys2Max)	return strKeys[s - ST_Keys2];  else
	if (s >= ST_Graph && s < ST_GraphMax)	return strMain[M_Graphs]/*strGraph[s - ST_Graph]*/;  else
	if (s >= ST_Config2 && s < ST_Config2Max)	return strConfig[s - ST_Config2];
	return "";
}
