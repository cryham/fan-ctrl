Fan Controler
=============

WIP: Fan Controler code for Teensy 3.2 with TFT ST7735 color display.

Based on my K.C. "Kacey" Keyboard Controller (More description of project on [My Website](http://cryham.tuxfamily.org/portfolio/2018-k-c-controller).


## Features

* PWM output to (PC) 3 pin 12V fans.
  (10 kHz, 7 Fan channels **)
* RPM (revolutions per minute or second) detection and display
* Stop prevention (useful for lowest RPM)

* GUI with menu on display, parameters
* Rotary encoder Todo: add to matrix
* Keyboard buttons (matrix 3x3 **)

* Optional DS18B20 temperature sensor
* TODO: Thermocouple op-amp, mux, reading?

** Can be changed in code.

---

### Code used

It uses code from repositories, quite reduced and/or modified:
* [PaulStoffregen cores](https://github.com/PaulStoffregen/cores/tree/master/teensy3) for Teensy 3 core.
* [sumotoy TFT_ST7735](https://github.com/sumotoy/TFT_ST7735/tree/1.0p1) for fast TFT ST7735 display.
* [Adafruit-GFX-Library](https://github.com/adafruit/Adafruit-GFX-Library) for drawing, combined with above.
* [Kiibohd](https://github.com/kiibohd/controller) only keyboard matrix scan code.

Since I am using a buffer (40kB for 160x128), all drawing code fills it. On each frame, buffer gets sent to display, thus no flickering.
All unnecessary code for drawing from libraries was removed.

Displaying GUI and scanning keyboard (at 1kHz) gives about 70 drawn frames per second. Demos are slower 10-30.

#### Licensing

My sources are licensed GPLv3. For other sources used, see their repositories.

---

### Hardware

For detailed info check subdir kicad with schematics, or included schematics.png image (todo).

The keyboard is made of:
* Teensy 3.2 (or 3.1). Can be overclocked, to 144 MHz (or 120 MHz, with longer wires).
* TFT LCD display ST7735, 160 x 128, 16bit color RGB565. Red PCB with SD card slot. Terribly low horizontal viewing angle.
* NPN transistor BC550. Connection from pin 23 output through 4.7k resistor to base, collector to 3.3V, emitter to Display LED.
* Optionally a DS18B20 1-wire temperature sensor with data pin through 4.7k resistor to 3.3V.
* Each fan uses 2 resistors 1k and 1k to 4.7k, 1 capacitor 100n (for 10kHz PWM) and 2 transistors: BC550C (or similar NPN) and BD140 (PNP).

Display uses pins: 9 DC, 10 CS, 8 RST (set in `Ada4_ST7735.cpp`) 11 DOUT, 13 CLK.
Then used pin 23 PWM to 4.7k resistor to BC550 base, collector to 3.3V and emitter to LCD backlight LED pin.

### Configuring

Information useful when building / starting to use this controller.

Pins setup is done in files:
* `def.h`
  * Define code features to use like: demos, game. Also optional pins for LED and Temp'C.
* `matrix.h` has defined pins for keyboard: Matrix_cols and Matrix_rows.
* Fan pins are defined in `fan.cpp`.
* Code related to fans is in files having `fan` in name.

When defining a new keyboard, it is useful to force starting in main.cpp on GUI Keys Matrix tab, it shows scan codes.

### Building

On **Windows** could be done using [Cygwin](https://www.cygwin.com/).  
Needs to have arm-gnu-none-eabi installed, just like for Linux.

On **Linux** required package are (at least): binutils-arm-gnu-none-eabi, gcc-arm-gnu-none-eabi, git.  
On Debian based, you can `sudo apt-get install` them or use package manager.


To get sources in console use:
```
git clone https://github.com/cryham/fan-ctrl.git
```
Then `cd kc` and `make` (or `make -j8` for 8 threads/CPUs) to start building.

On successful build the last lines are e.g.
```
  CC kc/matrix.c
 Linking 
   SRAM: 68%  44852 / 65536 B
  Flash: 51%  133888 / 262144 B
```
showing used percentages of memories.

The output file `main.hex` is created inside `bin/` folder.  
Open it in [Teensy Loader App](https://www.pjrc.com/teensy/loader.html) and press reset on board to flash (upload) and use.
