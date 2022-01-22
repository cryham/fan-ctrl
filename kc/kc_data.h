#pragma once
#include <stdint.h>
#include <vector>
#include "kc_params.h"
#include "fans.h"
#include "def.h"


//  errors load, save
enum KC_Err {  E_ok=0, E_size,  E_H1, E_H2, E_ver,  E_max  };
extern const char* KCerrStr[E_max];


//  main, state
//--------------------------------------
struct KC_Main
{

	//  main  ----
	KC_Main();
	
	Fans fans;  // ***
	
	void Update(uint32_t ms);


	//  eeprom  ----
	void Load(), Save();
	uint16_t GetSize();  // mem

	uint16_t memSize = 0;  // result B
	KC_Err err = E_ok;


	//  inactive time(s) for stats
	uint32_t
		tm_on = 0,  // uptime   tm_= rtc, ms_= milis
		tm_key = 0, tm_keyOld = 0,  // last key press time, previous
		msRpm = 0;    // rpm dt time
	void ResetStats(bool rtc=false);


	uint8_t grRpm[NumFans][W];  // graph array  rpm/10  e.g. 1200rpm = 120px
	uint8_t grRpos = 0;  // write pos
};

extern KC_Main kc;
