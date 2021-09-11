#include "gui.h"
#include "matrix.h"
#include "kc_params.h"
#include "kc_data.h"
#include "periodic.h"

const uint8_t Gui::DispPages[Di_All] = {2,2,1,3,2};


//  Info
//....................................................................................

void Gui::KeysConfig()
{
	switch (yy)
	{
	case C_Save:  Save();  break;
	case C_Load:  Load(0);  break;
	case C_Use:
		par.verCounter = RangeAdd(par.verCounter, kRight, 0, 255, 1);  break;
	}
}

//  Keys Scan
//....................................................................................
void Gui::KeysScan()
{
	if (yy != K_Scan)  return;
	int16_t ysp = 2;

	if (kUp)
	{	ym2Scan = RangeAdd(ym2Scan, kUp, 0, ysp, 1);  }
	else if (kRight)
	switch (ym2Scan)
	{
	case 0:
		par.scanFreq = RangeAdd(par.scanFreq, -kRight * 4, 2, 150);
		Periodic_init( par.scanFreq * 1000 );  break;  // upd
	case 1:
		par.strobe_delay = RangeAdd(par.strobe_delay, kRight, 0, 50);  break;
	case 2:
		par.debounce = RangeAdd(par.debounce, kRight, 0, 50);  break;
	}
}


//  Display
//....................................................................................
void Gui::KeysDisplay()
{
	if (kBack)  --mlevel;

	if (kUp)  // y
	{	pgDisp = RangeAdd(pgDisp, kUp < 0 ? 1 : 0, 0, Di_All-1, 1);  //
		ym2Disp = RangeAdd(ym2Disp, kUp > 0 ? 1 : 0, 0, DispPages[pgDisp], 1);
	}else
	/*if (kPgUp)  // pg
	{	pgDisp = RangeAdd(pgDisp, kPgUp, 0, Di_All-1, 1);
		ym2Disp = RangeAdd(ym2Disp, 0, 0, DispPages[pgDisp], 1);
	}else*/
	if (kRight)  // adjust values
	switch (pgDisp)
	{
	case Di_Bright:
		switch (ym2Disp)
		{
		case 0:
			par.brightness = RangeAdd(par.brightness, kRight * 2, 0, 100, 1);
			kc.setBright = 1;  break;
		case 1:
			par.startScreen = RangeAdd(par.startScreen, kRight, 0, ST_ALL-1);  break;
		}	break;

	case Di_Key:
		switch (ym2Disp)
		{
		case 0:
			par.krDelay = RangeAdd(par.krDelay, kRight, 0,255);  break;
		case 1:
			par.krRepeat = RangeAdd(par.krRepeat, kRight, 0,255);  break;
		}	break;

	case Di_Stats:
		switch (ym2Disp)
		{
		case 0:
			par.timeRpm = RangeAdd(par.timeRpm, kRight, 0,255);  break;
		}	break;

	case Di_Graph:
		switch (ym2Disp)
		{
		case 0:
			par.timeTemp = RangeAdd(par.timeTemp, kRight, 0, gIntvMask, 1);  break;
		case 1:
			par.timeTgraph = RangeAdd(par.timeTgraph, kRight, 0, gIntvMask, 1);  break;
		case 2:
			par.minTemp = RangeAdd(par.minTemp, kRight, 0, 40, 1);  break;
		case 3:
			par.maxTemp = RangeAdd(par.maxTemp, kRight, 0, 40, 1);  break;
		}	break;

	case Di_Debug:
		switch (ym2Disp)
		{
		case 0:  // ram info
			iRam = RangeAdd(iRam, kRight, 0, 2);  break;
		case 1:  // fps
			demos.iFps = RangeAdd(demos.iFps, kRight, 0, 2);  break;
		case 2:
			par.tempOfs = RangeAdd(par.tempOfs, kRight * 1, -128, 127, 1);  break;
		}	break;
	}
}
