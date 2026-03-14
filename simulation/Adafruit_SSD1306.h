#ifndef _Adafruit_SSD1306_H_
#define _Adafruit_SSD1306_H_

#include <string>
/// gfxfont is taken as is from Adafruit as it is independent from the Arduino frontend
#include "gfxfont.h"

#define SSD1306_SWITCHCAPVCC 0x02 ///< Gen. display voltage from 3.3V

/// values directly
#ifndef NO_ADAFRUIT_SSD1306_COLOR_COMPATIBILITY
#define BLACK SSD1306_BLACK     ///< Draw 'off' pixels
#define WHITE SSD1306_WHITE     ///< Draw 'on' pixels
#define INVERSE SSD1306_INVERSE ///< Invert pixels
#endif
/// fit into the SSD1306_ naming scheme
#define SSD1306_BLACK 0   ///< Draw 'off' pixels
#define SSD1306_WHITE 1   ///< Draw 'on' pixels
#define SSD1306_INVERSE 2 ///< Invert pixels

class Adafruit_SSD1306 {
public:
  Adafruit_SSD1306(uint8_t w, uint8_t h) {};
  ~Adafruit_SSD1306(void) {};

  bool begin(uint8_t switchvcc = SSD1306_SWITCHCAPVCC, uint8_t i2caddr = 0,
             bool reset = true, bool periphBegin = true);
  void display(void);
  void clearDisplay(void);
  void invertDisplay(bool i);
  void drawPixel(int16_t x, int16_t y, uint16_t color);
  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  bool getPixel(int16_t x, int16_t y);

  void setCursor(uint8_t x, uint8_t y);
  void setTextColor(uint8_t color);
  void setTextSize(uint8_t size);

  void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h,
                     int16_t radius, uint16_t color);
  void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                uint16_t color);
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                uint16_t color);
  void setFont(const GFXfont *f = nullptr);

  void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h,
                  uint16_t color, int strife = 1, bool do_not_round = false);
  void print(std::string);
  void println(std::string);
  void print(char);
  void print(int);
  void print(unsigned int);
  void println(char);

private:
  bool is_inverted;
  uint16_t font_color;
  uint8_t font_size;
  const GFXfont *font;

  uint8_t cursor_x;
  uint8_t cursor_y;

  uint8_t pixel[16][64];
};



#endif // _Adafruit_SSD1306_H_
