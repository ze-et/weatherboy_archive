#pragma once
#include "Arduino.h"

typedef unsigned char uchar;
typedef unsigned int uint;
// Enums ------------------------------------------------------------------------------------------
enum unit_enum
{
  UNIT_TEMPERATURE = 0b0001,
  UNIT_HUMIDITY    = 0b0010,
  UNIT_GAS         = 0b0100,

  UNIT_WITHOUT_GRAPHNESS = 0b1111,  // Binäres UND mit diesen Wert, um die Graphkomponente zu entfernen
  UNIT_IS_GRAPH = 0b10000,  // Binäres UND mit diesen Wert, um herauszufinden, ob es ein Graphwert ist
  UNIT_TEMPERATURE_GRAPH = 0b10001,
  UNIT_HUMIDITY_GRAPH = 0b10010,
  UNIT_GAS_GRAPH = 0b10100,
  UNIT_INVALID
};

// TODO Den Standardwert implementieren
// Man kann möglicherweise ein XYZ_STANDARD Wert implementieren vor XYZ_END;
// Dies würde dann automatisch auf einen Standardwert initialisiert werden, den man in Mode 0 setzt
enum temperature_unit
{
  TEMPERATURE_CELSIUS,
  TEMPERATURE_FAHRENHEIT,
  TEMPERATURE_KELVIN,
  TEMPERATURE_END
};
enum humidity_unit
{
  HUMIDITY_PERCENT,
  HUMIDITY_END
};
enum gas_unit
{
  GAS_DANGER,
  GAS_QUALITYEQUIVALENT,
  GAS_SENSORVALUE,
  GAS_END
};
/*enum symbol_enum // Veraltet
{
  SYMBOL_SUN,
  SYMBOL_THERMOMETER,
  SYMBOL_DROPLET,

  SYMBOL_INVALID
};*/
enum graph_interval_enum
{
  GRAPH_SECOND =   1000,
  GRAPH_MINUTE =   60000,
  GRAPH_HOUR   = 3600000,
  GRAPH_END
};

#define GAS_NOT_INITIALISED -1
// ------------------------------------------------------------------------------------------------

// Objekte ----------------------------------------------------------------------------------------
struct joyStruct
{
  uint left : 1;
  uint right : 1;
  uint up : 1;
  uint down : 1;
  uint pressed : 4;

  joyStruct();
  joyStruct operator!() const;
  joyStruct operator&&(joyStruct const& a) const;
};

class info_module
{
  private:
    uchar unit;
    enum unit_enum unit_type;
    //enum symbol_enum symbol_value;
    struct
    {
      uint size_x : 2;
      uint size_y : 2;
      uint pos_x : 2;
      uint pos_y : 2;
    } displayed;
    void print_small(uchar size, String text) const;
    void print_small(uchar size, char symbol) const;
    void print_number_tiny(uint number) const;
    void render_graph() const;
  public:
    info_module();  // Standardkonstruktor; füllt Array temporär mit ungültigen Werten
    info_module(uchar initial_unit_type,
                enum unit_enum unit_measured,
                // enum symbol_enum symbol_used,
                uchar size_x,
                uchar size_y,
                uchar pos_x,
                uchar pos_y);
    void render(bool invert_colors) const;
    void edit_unit(const uint direction);
};

class module_set
{
  public:
    uchar size;
    info_module module[4];
    module_set(uchar set_size = 0, info_module a = info_module(), info_module b = info_module(), info_module c = info_module(), info_module d = info_module());
};

class settings
{
  public:
    char value;

    typedef char (*settingFunc)();
    settingFunc currentSettingFunc;

    String name;

    settings();
    settings(String setting_name, settingFunc function, char set_value = 0);
    char toggleSetting();
};
// ------------------------------------------------------------------------------------------------

// Funktionen -------------------------------------------------------------------------------------
int posmod(int value, unsigned int divisor);
// ------------------------------------------------------------------------------------------------