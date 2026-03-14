#include <string>
#include "Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include "customtypes.h"
#include "globalvars.h"
#include "customfuncs.h"


// Funktionen für Modiänderung, Displayänderung und Einstellungsänderung
void mode_change(char direction)
{
  if(!direction) {return;}

  mode_i = posmod(mode_i + direction, mode_count + 1);
  selection_i = 0;
}

void unit_change(char direction_x, char direction_y)
{
  if(!(direction_x || direction_y)) {return;}

  selection_i = posmod(selection_i + direction_x, mode_list[mode_i].size);
  if(direction_y)
  {
    mode_list[mode_i].module[selection_i].edit_unit(direction_y);
  }
}

void settings_change(char direction_x, char direction_y, unsigned char pressed)
{
  if(direction_x)
  {
    mode_change(direction_x);
    return;
  }
  if(!(direction_y || pressed)) {return;}

  selection_i = posmod(selection_i + direction_y, setting_count);
  if(pressed)
  {
    settings_list[selection_i].value = settings_list[selection_i].toggleSetting();
  }
}

// Einstellungsausgabe Helferfunktion
void checkmark_line(uchar x, uchar y, char value, bool is_selected, String text)
{
  if(!(value & 0xFE)) // Falls value ein Wahrheitswert ist
  {
    display.fillRoundRect(x, y, 6, 6, 2, INVERSE);
    if(value)
    {
      display.fillRoundRect(x+1, y+1, 4, 4, 2, INVERSE);
    }
  }
  else
  {
    display.setCursor(x, y);
    display.print(value);
  }
  if(is_selected)
  {
    display.fillRect(x + 8, y, text.length() * 6, 8, INVERSE);
  }
  display.setCursor(x + 8, y);
  display.print(text);
}

// Einstellungsausgabe
void settings_mode_output()
{
  display.fillRoundRect(1, 1, 126, 62, 8, INVERSE);
  display.setTextColor(INVERSE);
  display.setTextSize(1);
  for(uchar i = 0; i < setting_count; i += 1)
  {
    checkmark_line(4, 4 + i * 8, settings_list[i].value, i == selection_i, settings_list[i].name);
  }
}

void final_output()
{
  for(uchar i = 0; i < mode_list[mode_i].size; i += 1)
  {
    mode_list[mode_i].module[i].render(i == selection_i && (!is_mode_change_set));
                                       // Die Bedingung für umgekehrte Farben:
                                       // Ist das momentane Modul das ausgewählte Modul im Einheitswechselmodus?
  }
}

#ifdef SCREENTOSERIAL
// Gibt den Inhalt des Bildschirms aus in Bytes; mithilfe eines Processing-Programs umzuwandeln zu erkennbaren Bildschirminhalt
  void print_screen_to_serial()
  {
    if(enable_screen_to_serial)
      {
      unsigned char invert_values = is_display_inverted * 0xFF;
      for(unsigned char i = 0; i < 64; i += 1)
      {
        for(unsigned char j = 0; j < 128; j += 8)
        {
          Serial.write(invert_values ^
                      (display.getPixel(j+7, i)
                      |(display.getPixel(j+6, i) << 1)
                      |(display.getPixel(j+5, i) << 2)
                      |(display.getPixel(j+4, i) << 3)
                      |(display.getPixel(j+3, i) << 4)
                      |(display.getPixel(j+2, i) << 5)
                      |(display.getPixel(j+1, i) << 6)
                      |(display.getPixel(j,   i) << 7)));
          //Serial.print(" ");
        }
      }
      //Serial.println();
      Serial.write(0b10101010);
    }
  }
#endif

// Einstellungen ----------------------------------------------------------------------------------
settings settings_list[setting_count] = {
  settings("Dunkler Modus",invert_display),
  settings("Graph Intervall",toggle_graph_interval,'s'),
  settings("Graph Nullpunkt",toggle_graph_zero_relative),
  settings("Graph leeren",reset_graph_value,' '),
  #ifdef MODULESINTEXT
  settings("Modul->Text",toggle_module_in_text),
  #endif
  #ifdef SCREENTOSERIAL
  settings("Uebertragen",toggle_screen_to_serial),
  #endif
};

char invert_display()
{
  is_display_inverted = !is_display_inverted;
  display.invertDisplay(is_display_inverted);
  return is_display_inverted;
}

char toggle_graph_interval()
{
  reset_graph_value();
  switch(graph_interval)
  {
    case GRAPH_SECOND:
      graph_interval = GRAPH_MINUTE;
      return 'm';
    case GRAPH_MINUTE:
      graph_interval = GRAPH_HOUR;
      return 'h';
    default: // case GRAPH_HOUR:
      graph_interval = GRAPH_SECOND;
      return 's';
  }
}

char toggle_graph_zero_relative()
{
  is_graph_not_relative_to_zero = !is_graph_not_relative_to_zero;
  return !is_graph_not_relative_to_zero;
}

char reset_graph_value()
{
  for(int i = 0; i < MEASURE_LIST_LENGTH; i += 1)
  {
    temperature_list[i] = temperature;
    humidity_list[i]    = humidity;
    gas_list[i]         = gas;
  }
  measure_list_i_offset = 0;
  measure_list_measured_count = 0;
  return ' ';
}

#ifdef SCREENTOSERIAL
char toggle_screen_to_serial()
{
  enable_screen_to_serial = !enable_screen_to_serial;
  return enable_screen_to_serial;
}
#endif
#ifdef MODULESINTEXT
char toggle_module_in_text()
{
  enable_modules_in_text = !enable_modules_in_text;
  return enable_modules_in_text;
}
#endif