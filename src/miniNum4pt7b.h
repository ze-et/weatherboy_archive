/*
const uint8_t miniNum4pt7bBitmaps[] PROGMEM = {															// Nach den Auskommentieren: Hab 0xF0 hinzugefügt, da bei der 9 der Bogen unten gefehlt hat
  0x56, 0xA0, 0xF0, 0xC5, 0x70, 0xCC, 0xE0, 0x77, 0x90, 0xE7, 0x7A, 0xA0, 0xE5, 0x20, 0xFE, 0xF0, 0xF5, 0xF0}; // Sollte stimmen, aber ist nicht kontrolliert


const GFXglyph miniNum4pt7bGlyphs[] PROGMEM = {
  {    0,   3,   4,   4,    0,   -3 },   // 0x30 '0'
  {    2,   1,   4,   3,    1,   -3 },   // 0x31 '1'
  {    3,   3,   4,   4,    0,   -3 },   // 0x32 '2'
  {    5,   3,   4,   4,    0,   -3 },   // 0x33 '3'
  {    7,   3,   4,   4,    0,   -3 },   // 0x34 '4'
  {    9,   2,   4,   4,    1,   -3 },   // 0x35 '5'
  {    10,   3,   4,   4,    0,   -3 },  // 0x36 '6'
  {    12,   3,   4,   4,    0,   -3 },  // 0x37 '7'
  {    14,   3,   4,   4,    0,   -3 },  // 0x38 '8'
  {    16,   3,   4,   4,    0,   -3 },  // 0x39 '9'
  };

// Generiert mit Adafruit fontconvert und dann manuell gekürzt auf 0 bis 9

// 18 Bytes für Bitmap, 80 Bytes für Glyphmap, 24 Bytes für Schriftartobjekt
// -> 122 Bytes

*/


// Leicht optimierte Bitmap; Symbole wurden zusammengefügt, wenn ihr Endbyte mit dem Anfangsbyte eines anderen Symbols übereinstimmte
// Fun Fact: Für alle Symbole mit ungerader Breite (alle außer die 5) kann man den letzten Halbbyte beliebig ändern, da der unbenutzt ist
const uint8_t miniNum4pt7bBitmaps[] PROGMEM = {
  0x56, 0xA0, 0xF5, 0xFE, 0xF0, 0xC5, 0x77, 0x90, 0xCC, 0xE7, 0x7A, 0xA0, 0xE5, 0x20};
//|---0---|   |---9---|   |1|   |---2---|         |---3---|   |---6---|   |---7---|
//                  |---8---|         |---4---|         |---5!---|

const GFXglyph miniNum4pt7bGlyphs[] PROGMEM = {
  {    0,   3,   4,   4,    0,   -3 },   // 0x30 '0'
  {    4,   1,   4,   3,    1,   -3 },   // 0x31 '1'
  {    5,   3,   4,   4,    0,   -3 },   // 0x32 '2'
  {    8,   3,   4,   4,    0,   -3 },   // 0x33 '3'
  {    6,   3,   4,   4,    0,   -3 },   // 0x34 '4'
  {    9,   2,   4,   4,    1,   -3 },   // 0x35 '5'
  {    10,   3,   4,   4,    0,   -3 },  // 0x36 '6'
  {    12,   3,   4,   4,    0,   -3 },  // 0x37 '7'
  {    3,   3,   4,   4,    0,   -3 },   // 0x38 '8'
  {    2,   3,   4,   4,    0,   -3 },   // 0x39 '9'
  };

const GFXfont miniNum4pt7b PROGMEM = {
  (uint8_t  *)miniNum4pt7bBitmaps,
  (GFXglyph *)miniNum4pt7bGlyphs,
  0x30, 0x39, 5 };

// Generiert mit Adafruit fontconvert und dann manuell gekürzt auf 0 bis 9

// 14 Bytes für Bitmap, 80 Bytes für Glyphmap, 24 Bytes für Schriftartobjekt
// -> 118 Bytes

// Notiz: Stellt sich heraus, das es bereits ein Programm online gibt, welches Schriftart Header Dateien akzeptiert
// und diese kürzt aufs Mindeste und dabei noch erlaubt, die Schrift zu bearbeiten und anzusehen. Wups!
// Zumindest hat mein Weg es erlaubt, etwas den Schriftarttyp zu verstehen und zu optimieren.
// https://tchapi.github.io/Adafruit-GFX-Font-Customiser/