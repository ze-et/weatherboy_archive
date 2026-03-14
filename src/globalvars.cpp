#include "globalvars.h"
#include "Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// Variablen, die deklariert sind in globalvars.h
int temperature = 0, humidity = 0, gas = 0;

unsigned short temperature_list[MEASURE_LIST_LENGTH];
unsigned short humidity_list[MEASURE_LIST_LENGTH];
unsigned short gas_list[MEASURE_LIST_LENGTH];
uchar measure_list_i_offset = 0;
uchar measure_list_measured_count = 0;

uchar mode_i = 0;
uchar selection_i = 0;
bool is_mode_change_set = true; // True:  Wechselt den Modus.
                                // False: Wechselt die Einheit
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Einstellung, die deklariert sind in globalvars.h
bool is_display_inverted = false;
bool is_graph_not_relative_to_zero = true;
enum graph_interval_enum graph_interval = GRAPH_SECOND;

// Sonstige
#ifdef SCREENTOSERIAL
bool enable_screen_to_serial = false;
#endif
#ifdef MODULESINTEXT
bool enable_modules_in_text = false;
#endif