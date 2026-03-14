#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ezButton.h>

#include "customfuncs.h"
#include "customtypes.h"
#include "mode_list.h"
#include "config.h"

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

void display_update();
// Am Anfang waren diese Funktionen mit "inline" attributiert
// Nach Tests ist aber klar geworden, dass dem Kompiler es nicht interessiert
void read_joy_values();
void respond_to_inputs();
void measure_values();
void measure_graph_values();

void setup()
{
  #ifdef SCREENTOSERIAL
  Serial.begin(115200);
  #endif
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
  // IF Über read_joy_values verschoben; kann sein, dass dies ein Fehler ist
  // Momentane Werte im Intervall von MEASURING_RATE_MS aktualisieren
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

void read_joy_values()
{
  button.loop();
  joyState.pressed = !button.getState();

  joy_temp_value = analogRead(JOYPINX);
  // Todeszone zwischen 196/672 und 352/828 inklusiv
  // -> Wert ändert sich nicht in Todeszone, um
  //    wiederholte Eingaben durch Messungenauigkeiten zu verhindern
  if(joy_temp_value > 828)
    {joyState.left = 1;}
  else if (joy_temp_value < 672)
    {joyState.left = 0;}
  if(joy_temp_value < 196)
    {joyState.right = 1;}
  else if (joy_temp_value > 352)
    {joyState.right = 0;}
  
  joy_temp_value = analogRead(JOYPINY);
  if(joy_temp_value > 828)
    {joyState.up = 1;}
  else if (joy_temp_value < 672)
    {joyState.up = 0;}
      if(joy_temp_value < 196)
    {joyState.down = 1;}
  else if (joy_temp_value > 352)
    {joyState.down = 0;}

  /* Neuer Wert UND negiert (Alter Wert)
  * -> Nur, wenn der neue Wert wahr ist und der alte Wert falsch ist der Finalwert wahr
  * --> Wenn gerade erst der Knopf gedrückt wurde bzw. der Joystick ausgelenkt wurde
  */
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