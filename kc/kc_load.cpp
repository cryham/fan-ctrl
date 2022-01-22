#include "WProgram.h"
#include "kc_data.h"
#include "fans.h"


//  load, save in eeprom
//.............................................
#define EOfs 0      //  offset and
#define ESize 2048  //  max size to use

#define Erd(a)    eeprom_read_byte((uint8_t*)a);      ++a;  memSize = a;  if (a >= ESize) {  err=E_size;  return;  }
#define Ewr(a,b)  eeprom_write_byte((uint8_t*)a, b);  ++a;  memSize = a;  if (a >= ESize) {  err=E_size;  return;  }


//  default params  ----
void ParInit()
{
	par.debounce = 4;  // ms?
	par.strobe_delay = 4;
	par.scanFreq = 50;  // 1 kHz

	par.brightness = 80;
	par.startScreen = 0;

	par.verCounter = 0;
	par.krDelay = 250/5;  par.krRepeat = 80/5;  // ms

	par.rtcCompensate = 0;
	par.tempOfs = int8_t(-0.06/*'C*/ / 0.03);

	par.timeRpm = 10;  // 8s
	par.timeTemp = 10;   // 8s
	par.timeTgraph = 15; // 1m  gIntervals
	
	par.minTemp = 15;  par.maxTemp = 90;
	//par.minTemp = 20;  par.maxTemp = 50;
	par.xCur = W-1;
	
	par.ym2Fan = 0;
	par.iFanAdd = 5;

	par.brightOff = 0;
	par.timeOff = 3;  // off
	
	par.pgGraph = 0;
	par.minRpm = 0;  par.maxRpm = 1200;
}

//  errors
const char* KCerrStr[E_max] = {
	"ok", "> Max size",  "Hdr1 !k", "Hdr2 !c", "Hdr3 ver <",
};

//  get ram allocated
uint16_t KC_Main::GetSize()
{
	uint16_t s=0;
	s += sizeof(KC_Main);
	return s;
}


//  Load
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
void KC_Main::Load()
{
	ParInit();  // defaults

	err = E_ok;

	int a = EOfs, n;  uint8_t s;
	//  header
	char h1 = 'f', h2 = 'c', ver = 4;  // cur
	h1 = Erd(a);  if (h1 != 'f') {  err=E_H1;  return;  }
	h2 = Erd(a);  if (h2 != 'c') {  err=E_H2;  return;  }
	ver = Erd(a);  if (ver > 6) {  err=E_ver;  return;  }


	//  params  ----
	s = Erd(a);  // size
	eeprom_read_block((void*)&par, (void*)a, s);  a+=s;
	if (a >= ESize) {  err=E_size;  return;  }

	if (par.startScreen > ST_ALL)
		par.startScreen = ST_ALL;


	//  fans  ----
	n = Erd(a);  // count
	if (n > NumFans)
		n = NumFans;

	s = Erd(a);  // fan data size
	if (s > sizeof(FanData))
		s = sizeof(FanData);

	for (int i=0; i < n; ++i)
	{
		FanData fd;  // ctor defaults, for pars over size
		kc.fans.fan[i].fd = fd;

		eeprom_read_block((void*)&kc.fans.fan[i].fd, (void*)a, s);  a += s;
		if (a >= ESize) {  err=E_size;  return;  }
	}
	memSize = a;
}


//  Save
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
void KC_Main::Save()
{
	err = E_ok;

	int a = EOfs;  uint8_t s;

	//  header
	char h1 = 'f', h2 = 'c', ver = 1;  // cur
	Ewr(a, h1);  Ewr(a, h2);  Ewr(a, ver);


	//  params  ----
	++par.verCounter;  // inc ver

	s = sizeof(par);
	Ewr(a, s);  // size
	eeprom_write_block((void*)&par, (void*)a, s);  a += s;
	if (a >= ESize) {  err=E_size;  return;  }


	//  fans  ----
	Ewr(a, NumFans);  // count

	s = sizeof(FanData);  // fan data size
	Ewr(a, s);

	for (int i=0; i < NumFans; ++i)
	{
		eeprom_write_block((void*)&kc.fans.fan[i].fd, (void*)a, s);  a += s;
		if (a >= ESize) {  err=E_size;  return;  }
	}
	memSize = a;
}
