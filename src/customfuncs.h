#pragma once

#include "config.h"

// Ändert den Modi in die angegebene Richtung
void mode_change(char direction);

// Ändert den Modi in die angegebene Richtung Y
// Wechselt das ausgewählte Infomodul mit Richtung X
void unit_change(char direction_x, char direction_y);

// Ändert die Einstellungen
void settings_change(char direction_x, char direction_y, unsigned char pressed);

// Helferfunktion, die benutzt wird in settings_mode_output
void checkmark_line(unsigned char x, unsigned char y, bool is_checked, bool is_selected, String text);

// Stellt den Einstellungsmodus dar
void settings_mode_output();

// Stellt alle momentan gegebenen Infomodule dar auf den Bildschirm
void final_output();

// Gibt Bildschirminhalt in Konsole aus
#ifdef SCREENTOSERIAL
  void print_screen_to_serial();
#endif

// ------------------------- Einstellungen --------------------------------------------------------
// Kehre die Farben des Displays um ("Dunkler Modus")
char invert_display();

// Wechsel Graphintervall auf Sekunde, Minute oder Stunde
char toggle_graph_interval();

// Bring Graphen dazu, immer den Nullpunkt zu beinhalten
char toggle_graph_zero_relative();

// Setze Graphwerte zurück
char reset_graph_value();

// Aktiviert/Deaktiviert die Bildschirminhaltausgabe
#ifdef SCREENTOSERIAL
  char toggle_screen_to_serial();
#endif
// AKtiviert/Deaktiviert Moduldarstellung als Text
#ifdef MODULESINTEXT
char toggle_module_in_text();
#endif