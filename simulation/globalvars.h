#pragma once

#include "Adafruit_SSD1306.h"
#include "customtypes.h"
#include "config.h"

// Definitionen
#define settings_count 4
#define mode_count 6

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

// Einfache Variablen -----------------------------------------------------------------------------
extern int temperature,humidity,gas;
extern unsigned short temperature_list[MEASURE_LIST_LENGTH]; // ~
extern unsigned short humidity_list[MEASURE_LIST_LENGTH];    // Passt zwar in ein uchar, aber wird zu short für Clean Code
extern unsigned short gas_list[MEASURE_LIST_LENGTH];
extern uchar measure_list_i_offset;
extern uchar measure_list_measured_count;

extern uchar mode_i;
extern uchar selection_i;
extern bool is_mode_change_set;
// ------------------------------------------------------------------------------------------------
// Sonstige
#ifdef SCREENTOSERIAL
  extern bool enable_screen_to_serial;
  #ifdef MODULESINTEXT
    extern bool enable_modules_in_text;
    #define setting_count settings_count + 2
  #else
    #define setting_count settings_count + 1
  #endif

#elif defined MODULESINTEXT
  extern bool enable_modules_in_text;
  #define setting_count settings_count + 1

#else
  #define setting_count settings_count
#endif

// Einstellungen ----------------------------------------------------------------------------------
extern settings settings_list[setting_count];
extern bool is_display_inverted;
extern bool is_graph_not_relative_to_zero;
extern enum graph_interval_enum graph_interval;
// ------------------------------------------------------------------------------------------------

// Objekte ----------------------------------------------------------------------------------------
extern Adafruit_SSD1306 display;
extern module_set mode_list[mode_count];
// ------------------------------------------------------------------------------------------------
