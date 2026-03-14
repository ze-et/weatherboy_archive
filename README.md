# weatherboy_archive
This repo contains the code related to the project "Weatherboy", partly a passion project and partly done for an university group project.

## What's "Weatherboy" even?
The project was about creating a climate measuring device that is customisable and visually appealing. I ended up being the person responsible for the code and for UI design, although for the latter I made sure to seek second opinions from my group members.
Out of the sensors provided to us, we ended up using the DHT11 for temperature and humidity and the MQ-135 for a very rough "air quality" measure. For interactivity, a single regular joystick was used. And finally, the output was done via an Adafruit SSD1306 128x64 OLED screen. All of these parts were connected to an Arduino Uno R4, somewhat overkill but the only other Arduino that was available wouldn't have been able to handle the screen.
Using the Weatherboy works like this:
- The screen shows a full "mode"; a mode contains multiple modules, each module displaying something measureable in a specific unit.
- The mode can be changed via left and right. The default application has 7 modes, 3 of which purely display values, 3 of which are graphs showing the last 100 values measured, and the unique settings mode.
- For the normal modes, clicking highlights one module. Left and right now switches which module is highlighted, while up and down changes the displayed unit (e.g. Fahrenheit versus Celsius). This mode can be exited by clicking again.
- For the settings, clicking toggles settings on or off (those with a button to the left), cycles through values (those with a character to the left), or triggers something (those with nothing to the left). Up and down moves through each setting.

## What is part of this repo?
- `src`: The plain source code.
- `bitmap_resources`: The original files with which bitmaps were created for use in the project.
- `processing`: A Processing program that recreates the contents of the screen inside a window. This was used for presentation purposes and for taking screenshots. Later on, screenshot functionality was implemented inside the program itself.
- `simulation`: An attempt of recreating the workings of the libraries used in the code just well enough for the project to run with good old `gcc`. Reason for doing this is to get more screenshots for this repo as I no longer have access to the Arduino kit. More info is provided in the Readme for that folder.
- `screenshots`: A collection of screenshots. They are separated into those gotten from the actual hardware (just two, unfortunately) and those gotten from the aforementioned simulation (which includes a picture of every single mode). You can see the slight inaccuracies between the two in `actual_hardware/dark_mode_off.png` and `simulation/7-settings.png`.
- `uml`: A very simplified UML of how the program is meant to work. In German.

## External code used in this repo
The main source code does not to my knowledge contain code I have not written myself.
As the simulation project couldn't work without knowing how the libraries themselves worked, it needed some of the source code used for them to function. Namely, some functions headers in the [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library) and [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306) libraries were copied, and the full Adafruit GFX [`gfxfont.h`](https://github.com/adafruit/Adafruit-GFX-Library/blob/master/gfxfont.h) file. The font included in [`glcdfont.c`](https://github.com/adafruit/Adafruit-GFX-Library/blob/master/glcdfont.c) was also used, albeit converted into the new Adafruit font type.
