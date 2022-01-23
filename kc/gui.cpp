#include "gui.h"
#include "def.h"
#include "kc_data.h"


//  Main  ----
Gui::Gui() : d(0)
{
	Init(0);

	msTemp = msTempGr = msRpm = millis();  // ms

	for (int i=0; i < MaxTemp; ++i)
	{
		//addr[i] =
		fTemp[i] = 20.f;  // 'C
		
		for (int x=0; x < W; ++x)
			grTemp[i][x] = 0;
		
		grTempUpd[i] = 1;
		grFmin[i] = 17; grFmax[i] = 35;
		grBmin[i] = 0;  grBmax[i] = 255;
	}

	for (int i=0; i < NumFans; ++i)
	{
		for (int x=0; x < W; ++x)
			grRpm[i][x] = 0;
		
		grRpmUpd[i] = 1;
		grRFmin[i] = 600; grRFmax[i] = 900;
		grRBmin[i] = 0;  grRBmax[i] = 255;
	}
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

	// level2
	if (s >= ST_Fans2 && s < ST_Fans2Max){  mlevel = 2;  ym = M_Fans;    ym2Fan = s - ST_Fans2;  }  else
	if (s >= ST_Graph && s < ST_GraphMax){  mlevel = 2;  ym = M_Graphs;  ym1[ym] = s - ST_Graph;  }  else
	if (s >= ST_Keys2 && s < ST_Keys2Max){  mlevel = 2;  ym = M_Keys;    ym1[ym] = s - ST_Keys2;  }  else
	switch (s)
	{
	case ST_Fans:   ym = M_Fans;  break;

	case ST_Displ:  ym = M_Display;  break;
	case ST_Help:   ym = M_Help;  break;
	case ST_Keys:   ym = M_Keys;  break;

	case ST_Config:      ym = M_Config;  break;
	case ST_Config2Ver:  ym = M_Config;  ym1[ym] = C_Version;  break;
#ifdef DEMOS
	case ST_Demos:  ym = M_Demos;  break;
	}
	if (s >= ST_Demos2){  mlevel = 2;  ym = M_Demos;  ym1[ym] = s - ST_Demos2;  }  //else
#else
	}
#endif
}

void Gui::StrScreen(int8_t st, char* s)
{
	s[0] = 0;
	// level2
	if (st >= ST_Fans2 && st <= ST_Fans2Max)  sprintf(s,"Fan %d", st - ST_Fans2 +1);  else
	if (st >= ST_Graph && st <= ST_GraphMax)  sprintf(s,"Graph %d", st - ST_Graph +1);  else
	if (st >= ST_Keys2 && st <= ST_Keys2Max)  strcpy(s, strKeys[st - ST_Keys2]);  else
	switch (st)
	{
	case ST_Main0:  sprintf(s, "Main ""\x13");  return;

	case ST_Fans:   strcpy(s, strMain[M_Fans]);  return;

	case ST_Displ:  strcpy(s, strMain[M_Display]);  return;
	case ST_Help:   strcpy(s, strMain[M_Help]);  return;
	case ST_Keys:   strcpy(s, strMain[M_Keys]);  return;
 
	case ST_Config:     strcpy(s, strMain[M_Config]);  return;
	case ST_Config2Ver: strcpy(s, strConfig[C_Version]);  return;
#ifdef DEMOS
	case ST_Demos:  strcpy(s, strMain[M_Demos]);  return;
	}
	if (st >= ST_Demos2)  strcpy(s, strDemo[st - ST_Demos2]);  return;
#else
	}
#endif
}
