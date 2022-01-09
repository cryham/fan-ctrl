#include "gui.h"
#include "Ada4_ST7735.h"
#include "kc_params.h"

#ifdef TEMP_PIN

#include <OneWire.h>
#include <DS18B20.h>

OneWire onewire(TEMP_PIN);  // pin
DS18B20 sensors(&onewire);


// search sensors again
void Gui::ResetTemp()
{
	onewire.reset_search();
	tempInit = TI_SEARCH;
	tempCount = 0;
}

//  Get Temp'C
//....................................................................................
void Gui::GetTemp()
{
	uint32_t ms = millis();

	if (tempInit == TI_SEARCH)
	{	tempInit = TI_FAIL;  //  first

		//  Look for 1-Wire devices
		while (tempCount < MaxTemp && onewire.search(addr[tempCount]))
		if (OneWire::crc8(addr[tempCount], 7) == addr[tempCount][7])
		{
			tempInit = TI_DONE;
			++tempCount;
		}
		//  setup
		if (tempInit == TI_DONE)
		{
			sensors.begin(12);  // quality bits
			for (int i=0; i < tempCount; ++i)
				sensors.request(addr[i]);
		}
	}
	if (tempInit >= TI_DONE)
	{
		if (ms - msTemp > tTemp(par) || ms < msTemp || tempInit == TI_DONE)
		{
			//  if measurement ready
			if (sensors.available())
			{
				msTemp = ms;
				tempInit = TI_READ;
				//?  slower if not in gui, every 20, 10 sec

				for (int i=0; i < tempCount; ++i)
				{
					fTemp[i] = sensors.readTemperature(addr[i]);
					fTemp[i] += 0.03f * par.tempOfs;  // fix sensor error
					sensors.request(addr[i]);  // next
				}
			}
		}

		if (ms - msTempGr > tTgraph(par) || ms < msTempGr)
		{
			msTempGr = ms;
			//  graph inc pos
			++grTpos;
			if (grTpos >= W)  grTpos = 0;

			//  add to graph  all temp sensors
			for (int i=0; i < tempCount; ++i)
			{
				int t = TempFtoB(fTemp[i]);  //par
			#if 0  // test
				static uint8_t ii = 0;  ++ii;
				static uint8_t ti = 128;  if (ii%3==0)  ++ti;  t = ti;
			#endif
				t = t > 255 ? 255 : (t < 0 ? 0 : t);  // 0 not measured
				grTemp[i][grTpos] = t;
				grTempUpd[i] = 1;
			}
		}
	}
}

#endif
