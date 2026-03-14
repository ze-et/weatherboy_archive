#include "customtypes.h"
#include "mode_list.h"

module_set mode_list[mode_count] =
{
  module_set(3,
    info_module(HUMIDITY_PERCENT, UNIT_HUMIDITY, 0, 0, 0, 0),
    info_module(GAS_SENSORVALUE, UNIT_GAS, 0, 0, 1, 0),
    info_module(TEMPERATURE_CELSIUS, UNIT_TEMPERATURE, 1, 0, 0, 1)
  ),

  module_set(2,
    info_module(TEMPERATURE_CELSIUS, UNIT_TEMPERATURE, 1, 0, 0, 1),
    info_module(HUMIDITY_PERCENT, UNIT_HUMIDITY, 1, 0, 0, 0)
  ),
  module_set(1,
    info_module(TEMPERATURE_CELSIUS, UNIT_TEMPERATURE, 1, 1, 0, 0)
  ),
  module_set(1,
    info_module(TEMPERATURE_CELSIUS, UNIT_TEMPERATURE_GRAPH, 1, 1, 0, 0)
  ),
  module_set(1,
    info_module(HUMIDITY_PERCENT, UNIT_HUMIDITY_GRAPH, 1, 1, 0, 0)
  ),
  module_set(1,
    info_module(GAS_SENSORVALUE, UNIT_GAS_GRAPH, 1, 1, 0, 0)
  ),
};