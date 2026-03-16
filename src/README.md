Some notes on the workings of the source code.

## Arduino pin configuration (UNO)
| Arduino Pin | Peripheral Pin | Configurable in `config.h`? |
|------------:|----------------|:--------------:|
|         `5V`|SSD1306 `5V`; Joystick `5V`; MQ135 `VCC`|     N/A[^1]       |
|  `3,3V`/`5V`|DHT11 `VCC`   |      N/A[^1]       |
|        `GND`|SSD1306/Joystick/DHT11/MQ135 `GND`|      N/A[^1]      |
|         `A5`|SSD1306 `SCL`     |       N/A[^2]   |
|         `A4`|SSD1306 `SDA`     |       N/A[^2]   |
|         `A3`|Joystick `VRX`    |        ✔️       |
|         `A2`|Joystick `VRY`    |        ✔️       |
|         `A1`|MQ135 `AO`        |        ✔️       |
|          `3`|Joystick `SW`     |        ✔️       |
|          `2`|DHT11 `DATA`      |        ✔️       |

[^1]: Voltage and Ground pins are independent of code since they purely supply the voltage needed to operate the peripheral unit.
[^2]: These are the pins needed for I²C communication.

## Functionality
A basic overview of how the program works is as follows:
- **Screen updates**: The screen updates every time a value is measured or the instant an input has been made.
- **Value measurements**:
  - Values are measured every 250ms by default. Gas values only get measured after 3 minutes[^3] due to how the sensor has to heat up.
  - Graph values are measured every second, minute, or hour, depending on the current setting. That includes the gas value this time.\
    There's up to 101 graph values. They are stored in an array where values are never moved, only overwritten. Where the most recent value is is stored in `measure_list_i_offset`.
- **Rendering**:
  - Plain info modules get rendered as rounded squares with the name of what they display and the value along with its unit in doubled font size.
    Info modules which are long can use a longer name, e.g. "Temperatur:" instead of "Temp.:".
  - Graph info modules, which are only programmed to work for full size, display the last 101 measured values as a line graph. They also display the unit and highest/lowest value.
  - Settings are listed with their name and either a button (toggle), whitespace (trigger), or a letter (multiple possible values) to represent their current state.
    There's no scrolling implemented, so 7 settings is the maximum before rendering breaks.
- **Control**:
  - While not selecting any module, modes are cycled through with left and right. The last mode is always the settings menu.
  - Clicking outside of the settings menu selects the first module. Modules are cycled through with left and right, while up and down cycle the unit the module uses.
    The unit switch lasts until the program shuts down.
  - Clicking in the settings triggers a function. The function then returns what the new visual state of the setting is.

[^3]: Technically after 720 measurements because I didn't make the condition dependent on the measuring rate.\
      Line 156 of `weatherboy.ino` is the culprit; replace `3 * 60 * 4` with `(3 * 60 * 1000) / MEASURING_RATE_MS` for behavior that lines up more closely to what I intended.
## Modularity
There's a few key parts which can be extended, some with more or less difficulty.
- **Units**: Requires adding an enum value to the unit enum as well as extending the two render functions of `info_module`.
- **Modes**: Requires adding a mode to the mode list global variable as well as changing the `mode_count` definition.
- **Settings**: Requires adding a setting to the settings list global variable as well as changing the `settings_count` definition.
Units, I probably could've done more effectively. Modes and settings were about as well as I could implement them while having them be constants.

## The somewhat odd file structure
The main thing I would change if I remade this program is grouping the components of the program up into different files. `customfuncs`, `customtypes`, and `globalvars` are all unclear on what they contain.
These file names came about since I made them at first purely to avoid a huge one file program. Here's a rough explanation of what they contain:
- `customtypes`: All enums and the classes `joyStruct`, `info_module`, `module_set`, and `settings`. Also `posmod()`.
- `customfuncs`: All other non-member functions. Also contains the list of settings.
- `mode_list`: The global variable `mode_list` which contains all modes for the program to use.
- `config`: Definitions to change the measuring rate, length of the graph arrays (probably not recommended), and pins.
- `globalvars`: Every global variable. `globalvars.cpp` doesn't define every single one as some, e.g. the mode list, are defined in other files.
