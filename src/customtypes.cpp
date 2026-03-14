#include "config.h"
#include <algorithm>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include "customtypes.h"
#include "globalvars.h"
#include "miniNum4pt7b.h"
#include "graph_bitmap.h"

int posmod(int value, unsigned int divisor)
{
  if(value < 0)
  {
    return divisor - 1;
  }
  else
  {
    return value % divisor;
  }
}

// Konstruktoren definiert
joyStruct::joyStruct()
{
  pressed = 0;
  up = 0;
  down = 0;
  left = 0;
  right = 0;
}
joyStruct joyStruct::operator!() const
{
  joyStruct r;
  r.pressed = !pressed;
  r.up = !up;
  r.down = !down;
  r.left = !left;
  r.right = !right;
  return r;
}
joyStruct joyStruct::operator&&(joyStruct const& a) const
{
  joyStruct r;
  r.pressed = pressed && a.pressed;
  r.up = up && a.up;
  r.down = down && a.down;
  r.left = left && a.left;
  r.right = right && a.right;
  return r;
}

info_module::info_module()
{
  unit = 0xFF;
  unit_type = UNIT_INVALID;
  displayed.size_x = 3;
  displayed.size_y = 3;
  displayed.pos_x = 3;
  displayed.size_y = 3;
}
info_module::info_module(uchar initial_unit_type,
                         enum unit_enum unit_measured,
                         /*enum symbol_enum symbol_used,*/
                         uchar size_x,
                         uchar size_y,
                         uchar pos_x,
                         uchar pos_y)
{
  unit = initial_unit_type;
  unit_type = unit_measured;
  //symbol_value = symbol_used;
  displayed.size_x = size_x;
  displayed.size_y = size_y;
  displayed.pos_x = pos_x;
  displayed.pos_y = pos_y;
}


// Funktionen definiert
void info_module::edit_unit(const uint direction)
{
  switch(unit_type & 0b1111)
  {
    case UNIT_TEMPERATURE:
    {
      unit = posmod(unit + direction, TEMPERATURE_END);
      break;
    }
    case UNIT_HUMIDITY:
    {
      unit = posmod(unit + direction, HUMIDITY_END);
      break;
    }
    case UNIT_GAS:
    {
      unit = posmod(unit + direction, GAS_END);
      break;
    }
    default:
    {
      unit = 0xFF;
    }
  }
}

void info_module::print_small(uchar size, String text) const
{
  display.setTextSize(1<<size);
  display.print(text);
  display.setTextSize(2<<size);
}
void info_module::print_small(uchar size, char symbol) const
{
  display.setTextSize(1<<size);
  display.print(symbol);
  display.setTextSize(2<<size);
}
void info_module::print_number_tiny(uint number) const
{
  display.setFont(&miniNum4pt7b);
  display.print(number);
  display.setFont();
}

#define pX displayed.pos_x
#define pY displayed.pos_y
#define sX displayed.size_x
#define sY displayed.size_y
void info_module::render_graph() const
{
  unsigned short highest_measured_value;
  unsigned short lowest_measured_value;
  unsigned short delta;
  // Dieser dreckige One-Liner ist nötig, da die Referenz bei der Deklaration bekannt sein muss
  unsigned short (&list_reference)[MEASURE_LIST_LENGTH] = ((unit_type & UNIT_WITHOUT_GRAPHNESS) == UNIT_TEMPERATURE) ?\
                                                              temperature_list\
                                                          : (((unit_type & UNIT_WITHOUT_GRAPHNESS) == UNIT_HUMIDITY) ?\
                                                              humidity_list\
                                                          :\
                                                              gas_list);
  highest_measured_value = *std::max_element(std::begin(list_reference),\
                                              std::end(list_reference));
  lowest_measured_value  = *std::min_element(std::begin(list_reference),\
                                              std::end(list_reference)) * is_graph_not_relative_to_zero;

  if(unit_type == UNIT_TEMPERATURE_GRAPH)
  {
    switch(unit)
    {
      case TEMPERATURE_FAHRENHEIT:
        highest_measured_value = highest_measured_value * 1.8 + 32;
        lowest_measured_value = lowest_measured_value * 1.8 + 32 * is_graph_not_relative_to_zero;;
        break;
      case TEMPERATURE_KELVIN:
        highest_measured_value = highest_measured_value + 273;
        lowest_measured_value = lowest_measured_value + 273;
        break;
    }
  }
  display.setTextSize(1);
  display.drawBitmap(22,5,graph_bitmap,102,58,INVERSE);
  display.setCursor(4,4);
  print_number_tiny(highest_measured_value);
  display.setCursor(4,52);
  print_number_tiny(lowest_measured_value);
  display.setCursor(4,28);
  switch(unit_type)
  {
    case UNIT_TEMPERATURE_GRAPH:
      switch(unit)
      {
        case TEMPERATURE_CELSIUS:
          display.print('C'); break;
        case TEMPERATURE_FAHRENHEIT:
          display.print('F'); break;
        case TEMPERATURE_KELVIN:
          display.print('K'); break;
      }
      break;
    case UNIT_HUMIDITY_GRAPH:
      display.print('%'); break;
    case UNIT_GAS_GRAPH:
      display.print('g'); break;
    default:
      display.print('?');
  }
  display.print('/');
  switch(graph_interval)
  {
    case GRAPH_SECOND:
      display.print('s'); break;
    case GRAPH_MINUTE:
      display.print('m'); break;
    case GRAPH_HOUR:
      display.print('h'); break;
    default:
      display.print('?');
  }
  delta = highest_measured_value - lowest_measured_value;
  for(int i = MEASURE_LIST_LENGTH - 2; i > MEASURE_LIST_LENGTH - measure_list_measured_count; i -= 1)
  {
    display.drawLine(i+25,(-(list_reference[(i+1 + measure_list_i_offset) % MEASURE_LIST_LENGTH] - lowest_measured_value) * 52)/ delta + 57,\
                     i+24,(-(list_reference[( i  + measure_list_i_offset) % MEASURE_LIST_LENGTH] - lowest_measured_value) * 52)/ delta + 57, BLACK);
  }
}

void info_module::render(bool invert_colors) const
{
  if(invert_colors)
  {
    display.fillRect((64*pX), (32*pY), (64<<sX), (32<<sY), INVERSE);
  }
  #ifdef MODULESINTEXT
  if(enable_modules_in_text)
  {
    uchar off_x = 64*pX + 32*sX*(!sY);
    uchar off_y = 32*pY + 16*sY*(!sX);
    display.setTextSize(1<<(sX * sY));
    display.setCursor(off_x, off_y);
    if(unit_type & UNIT_IS_GRAPH)
    {
      display.print("Graph ");
    }
    switch(unit_type & UNIT_WITHOUT_GRAPHNESS)
    {
      case UNIT_TEMPERATURE:
        display.println("Temp");
        display.setCursor(off_x, off_y+8<<sY);
        switch(unit)
        {
          case TEMPERATURE_CELSIUS:
            display.println("Celsius"); break;
          case TEMPERATURE_FAHRENHEIT:
            display.println("Fahrenheit"); break;
          case TEMPERATURE_KELVIN:
            display.println("Kelvin"); break;
        }
        break;
      case UNIT_HUMIDITY:
        display.println("Fcht");
        display.setCursor(off_x, off_y+8<<sY);
        display.println("Prozent");
        break;
      case UNIT_GAS:
        display.println("Gas");
        display.setCursor(off_x, off_y+8<<sY);
        switch(unit)
        {
          case GAS_DANGER:
            display.println("Smiley"); break;
          case GAS_QUALITYEQUIVALENT:
            display.println("Vergleich"); break;
          case GAS_SENSORVALUE:
            display.println("direktWert"); break;
        }
        break;
    }
    display.setCursor(off_x, off_y+16<<sY);
    display.print("Pos: "); display.print('('); display.print(pX); display.print(','); display.print(pY); display.println(')');
    display.setCursor(off_x, off_y+24<<sY);
    display.print("Size:"); display.print('('); display.print(sX+1); display.print(','); display.print(sY+1); display.println(')');
    return;
  }
  #endif
  display.fillRoundRect((64*pX)+1, (32*pY)+1, (64<<sX)-2, (32<<sY)-2, 6, INVERSE);
  if(unit_type & UNIT_IS_GRAPH)
  {
    render_graph();
  }
  else
  {
    display.setCursor(64*pX + 4<<sX, 32*pY + 4<<sY);
    display.setTextSize(1<<(sX * sY));
    display.setTextColor(INVERSE);
    switch(unit_type)
    {
      case UNIT_TEMPERATURE:
        if(sX & (!sY))
        {
          display.print("Temperatur:");
        }
        else
        {
          display.print("Temp.:");
        }
        display.setTextSize(2<<(sX * sY));
        display.setCursor(64*pX + 4<<sX, 32*pY + 13<<sY);
        switch(unit)
        {
          case TEMPERATURE_CELSIUS:
            display.print(temperature);
            print_small(sX * sY, 'o');
            display.print('C');
            break;
          case TEMPERATURE_FAHRENHEIT:
            display.print((uint) (temperature * 1.8 + 32));
            print_small(sX * sY, 'o');
            display.print('F');
            break;
          case TEMPERATURE_KELVIN:
            display.print(temperature + 273);
            display.print('K');
            break;
          default:
            display.print("ERunit");
        }
        break;
      case UNIT_HUMIDITY:
        if(sX & (!sY))
        {
          display.print("Feuchtigkeit:");
        }
        else
        {
          display.print("Feucht.:");
        }
        display.setTextSize(2<<(sX * sY));
        display.setCursor(64*pX + 4<<sX, 32*pY + 13<<sY);
        switch(unit)
        {
          case HUMIDITY_PERCENT:
            display.print(humidity);
            display.print('%');
            break;
          default:
            display.print("ERunit");
        }
        break;
      case UNIT_GAS:
        display.println("Gaswert:");
        display.setTextSize(2<<(sX * sY));
        display.setCursor(64*pX + 4<<sX, 32*pY + 13<<sY);
        if(gas == GAS_NOT_INITIALISED)
        {
          display.print("Warte");
          return;
        }
        switch(unit)
        {
          case GAS_SENSORVALUE:
            display.print(gas);
            display.print('b');
            break;

          // Sollte anzeigen, ob die Luftqualität gesundheitlich gefährdent ist
          case GAS_DANGER:
            if(gas > 120)
              {display.print(":D"); break;}
            if(gas > 90)
              {display.print(":)"); break;}
            if(gas > 60)
              {display.print(":|"); break;}
            if(gas > 30)
              {display.print("):"); break;}
            display.print("D:"); break;
            break;
          // Vergleichswert anzeigen, z.B. "WALD' 'STADT' 'RAUM"
          case GAS_QUALITYEQUIVALENT:
            if(gas > 120)
              {display.print("Wald"); break;}
            if(gas > 90)
              {display.print("Raum"); break;}
            if(gas > 60)
              {display.setTextSize(1<<(sX * sY)); display.print("vollRaum"); break;}
            if(gas > 30)
              {display.print("Stadt"); break;}
            display.setTextSize(1<<(sX * sY));
            display.print("keineLuft"); break;
            break;
          default:
            display.print("ERunit");
        }
        break;
      default:
        display.println("unit_type Ungültig");
    }
  }
}
#undef pX
#undef pY
#undef sX
#undef sY

module_set::module_set(uchar set_size, info_module a, info_module b, info_module c, info_module d)
{
  size = set_size;
  module[0] = a;
  if(size == 1) {return;}
  module[1] = b;
  if(size == 2) {return;}
  module[2] = c;
  if(size == 3) {return;}
  module[3] = d;
}
settings::settings()
{
  name = "";
  value = 0;
}
settings::settings(String setting_name, settingFunc function, char set_value)
{
  name = setting_name;
  currentSettingFunc = function;
  value = set_value;
}
char settings::toggleSetting()
{
  if(currentSettingFunc)
  {
    return (*currentSettingFunc)();
  }
}