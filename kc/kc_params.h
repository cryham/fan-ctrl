#pragma once
#include <stdint.h>
#include "def.h"


//  params, saved
//--------------------------------------
struct KC_Params
{
	//  scan setup
	uint8_t debounce;  // ms
	//  delay before and after each strobe
	uint8_t strobe_delay;  // us
	//  scanFreq * 1000 = timer period, 50 is 960 Hz
	uint8_t scanFreq;

	//  brightness led, 0..100%
	uint8_t brightness;
	int8_t startScreen;  // screen after start

	uint8_t verCounter;  // inc on each save

	//  gui key auto repeat, ms
	uint8_t krDelay, krRepeat;

	//  offsets
	int8_t rtcCompensate; // xtal capacitance, adjust ppm
	int8_t tempOfs;       // adj Temp val  *0.03'C  +-3.8'C

	//  time intervals
	uint8_t timeRpm;      // Rpm add to graph, avg val
	uint8_t timeTemp;     // Temp'C read
	uint8_t timeTgraph;   // Temp'C add to graph, no average
	#define tRpm(par)     (100 * gIntervals[par.timeRpm    & gIntvMask])  // ms
	#define tTemp(par)    (100 * gIntervals[par.timeTemp   & gIntvMask])  // ms
	#define tTgraph(par)  (100 * gIntervals[par.timeTgraph & gIntvMask])
	#define tTmOff(par)   (100 * gIntervals[par.timeOff    & gIntvMask])

	//  Temp graph scale
	uint8_t minTemp, maxTemp;
	uint8_t xCur;         // cursor, not used

	uint8_t ym2Fan;       // last fan detail
	int8_t iFanAdd;       // add %

	uint8_t brightOff;
	uint8_t timeOff;
	
	int8_t pgGraph;  // EGraphs
};
//  --- ADD new to END ----
//  set defaults in ParInit()


enum EStartScr  //  start screen
{	ST_Main0 = 0,
	ST_Fans, ST_Fans2, ST_Fans2Max = ST_Fans2 + NumFans-1,
	ST_Graph, ST_GraphMax = ST_Graph + G_All-1,

	ST_Displ,
	ST_Help,
	ST_Config, ST_Config2Ver,
	ST_Keys, ST_Keys2, ST_Keys2Max = ST_Keys2 + K_All-1,

	ST_Demos,
	ST_Demos2,  ST_ALL = ST_Demos2
	#ifdef DEMOS
		+D_All-1,
	#endif
};

#ifdef __cplusplus
extern "C" {
#endif

extern struct KC_Params par;

#ifdef __cplusplus
}
#endif
