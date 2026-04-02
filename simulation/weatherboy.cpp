#include "Adafruit_SSD1306.h"
#include "DHT.h"
#include "ezButton.h"

#include "customfuncs.h"
#include "customtypes.h"
#include "config.h"
#include "globalvars.h"

#define DHT11 0
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
ezButton button(JOYPINSW);

// Zeit, bis die Gaswerte angezeigt werden
uint gas_preparation_delay = 0;
// Letzter Zeitpunkt, wo automatisch die Messwerte aktualisiert wurden
unsigned long int last_auto_measure_time = 0;
// Letzter Zeitpunkt, wo Graphwerte aktualisiert wurden
unsigned long int last_graph_measure_time = 0;
// Speichert temporär X/Y Auslenkung des Joysticks
unsigned short joy_temp_value = 0;
// Momentane Joystickwerte
struct joyStruct joyState;
// Vorherige Joystickwerte
struct joyStruct previousJoyState;
// Momentane Joystickwerte so gefiltert, dass nur neue Eingaben teil davon sind.
// D.h. wenn der Knopf gedrückt gehalten wird, wird der Druck nur registiert im ersten Moment.
struct joyStruct filteredJoyState;
// Serial für Ein- und Ausgabe über Processing
Serial serial;

void display_update();
// Am Anfang waren diese Funktionen mit "inline" attributiert
// Nach Tests ist aber klar geworden, dass dem Kompiler es nicht interessiert
void read_joy_values();
void respond_to_inputs();
void measure_values();
void measure_graph_values();

#include <chrono>
unsigned long millis()
{
  namespace sc = std::chrono;
  auto time = sc::system_clock::now();

  auto since_epoch = time.time_since_epoch();

  auto millis = sc::duration_cast<sc::milliseconds>(since_epoch);
  static auto millis_start = millis.count();
  return millis.count() - millis_start;
}

void setup()
{
  serial.begin(19200);
  dht.begin();
  button.setDebounceTime(25);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { 
    for(;;); // Programmablauf beendet
  }

  gas = GAS_NOT_INITIALISED;

  reset_graph_value();
}

void loop()
{
  if(millis() - last_auto_measure_time >= MEASURING_RATE_MS)
  {
    measure_values();
    last_auto_measure_time += MEASURING_RATE_MS;
    display_update();
  }
  // Graphwerte im Intervall von graph_interval aktualisieren
  if(millis() - last_graph_measure_time >= graph_interval)
  {
    measure_graph_values();
    last_graph_measure_time += graph_interval;
  }
  read_joy_values();
  respond_to_inputs();
  

}

void respond_to_inputs()
{
  if(mode_i < mode_count) // Modi
  {
    if (filteredJoyState.pressed)
    {
      is_mode_change_set = !is_mode_change_set;
    }
    if(is_mode_change_set) // Moduswechsel
    {
      mode_change(filteredJoyState.right - filteredJoyState.left);
    }
    else                   // Einheitswechsel eines der Infomodule
    {
      unit_change(filteredJoyState.right - filteredJoyState.left,
                  filteredJoyState.down  - filteredJoyState.up   );
    }
  }
  else // Einstellungen
  {
    settings_change(filteredJoyState.right - filteredJoyState.left,
                    filteredJoyState.down  - filteredJoyState.up  ,
                    filteredJoyState.pressed);
  }
  if(filteredJoyState.right
    || filteredJoyState.left
    || filteredJoyState.up
    || filteredJoyState.down
    || filteredJoyState.pressed) // Falls irgendetwas gemacht wurde am Joystick
  {
    display_update();
  }
}

int analogRead(int)
{
  return 512;
}
void read_joy_values()
{
  joyState = serial.get_input();

  filteredJoyState = joyState && (!previousJoyState);

  previousJoyState = joyState;
}

void measure_values()
{
  temperature = dht.readTemperature();
  humidity    = dht.readHumidity();
  if(gas_preparation_delay >= 3 * 60 * 4) // 3 Minuten
  {
    gas = analogRead(GASPIN);
  }
  else
  {
    gas_preparation_delay += 1;
  }
}
void measure_graph_values()
{
  temperature_list[measure_list_i_offset] = dht.readTemperature();
  humidity_list[measure_list_i_offset]    = dht.readHumidity();
  gas_list[measure_list_i_offset] = analogRead(GASPIN);
  measure_list_i_offset = (measure_list_i_offset + 1) % MEASURE_LIST_LENGTH;
  if(measure_list_measured_count < MEASURE_LIST_LENGTH)
  {
    measure_list_measured_count += 1;
  }
}

void display_update()
{
  // Setze Display zurück
  display.clearDisplay();
  // Gebe aus in den Buffer
  if(mode_i < mode_count)
  {
    // Modiausgabe
    final_output();
  }
  else
  {
    // Einstellungsausgabe
    settings_mode_output();
  }
  // Mach den Buffer sichtbar
  display.display();
  #ifdef SCREENTOSERIAL
    print_screen_to_serial();
  #endif
}


    // Auskommentiert: Einfache Lösung, welche keine Todeszone für die Joysticks hat
    // Führt zu wiederholte Bewegungen am Rande der Grenze wegen Messungenauigkeiten
    //joyState.right = (joy_temp_value > 828);
    //joyState.left = (joy_temp_value < 196);
    //joy_temp_value = analogRead(JOYPINY);
    //joyState.down = (joy_temp_value > 828);
    //joyState.up = (joy_temp_value < 196);

/*
  // TEMP: Wird nur verwendet für Debugging -------------------------------------------------------
  Serial.print("TEMPERATURE:");
  Serial.println(temperature);
  Serial.print("HUMIDITY:");
  Serial.println(humidity);
  Serial.print("GAS:");
  Serial.println(gas);
  Serial.print("JOYSTICKUP:");
  Serial.println(joyState.up);
  Serial.print("JOYSTICKDOWN:");
  Serial.println(joyState.down);
  Serial.print("JOYSTICKLEFT:");
  Serial.println(joyState.left);
  Serial.print("JOYSTICKRIGHT:");
  Serial.println(joyState.right);
  Serial.print("JOYSTICKPRESSED:");
  Serial.println(joyState.pressed);
  // ----------------------------------------------------------------------------------------------
  */
int main(void)
{
  setup();
  while(true){
    loop();
  }
}
