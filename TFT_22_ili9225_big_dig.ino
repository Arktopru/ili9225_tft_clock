#include "SPI.h"
#include "TFT_22_ILI9225.h"
#include <MD_DS3231.h>
//Connect DS3231
//SCL - A5
//SDA - A4
#include <Wire.h>
//Connect ILI9225 2.2 inch SPI LCD
#define TFT_RST 8
#define TFT_RS  9
#define TFT_CS  10
#define TFT_SDI 11
#define TFT_CLK 13
#define TFT_LED 3
#define TFT_BRIGHTNESS 100

TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED, TFT_BRIGHTNESS);

uint16_t x0 = 31, y0 = 45, x1 = 15, y1 = 15, x2 = 100, y2 = 120;
uint8_t hh = 60, mm = 60, ss = 60, m = 0, d = 0, dow = 0;
uint16_t yy = 0;
uint16_t textColor = COLOR_YELLOW;
uint16_t backgroundColor = COLOR_INDIGO;

void setup() {
  Serial.begin(9600);
  Serial.print("t yyyymmdd hhmmss dw\twrite the current date, time and day of week (1-7)");
  tft.setBacklightBrightness(100);
  tft.begin();
  tft.clear();
  tft.setOrientation(1);
  tft.setBackgroundColor(backgroundColor);
  RTC.readTime();
  printLayout();
  printDate();
  printTime();
}
void loop() {
  char  c = "";
  if (Serial.available() > 0) {
    c = Serial.read();
  }
  if (c == 't') {
    inputTime();
    RTC.writeTime();
    RTC.readTime();
    printDate();
    printTime();
  } else {
    RTC.readTime();
    printDate();
    printTime();
  }
}
void printLayout(){
  tft.fillRectangle(1,1, 219, 175, backgroundColor);
  drawHorizontal(x1 + 16 * 3 + 11, y1 + 11, textColor, 3);
  drawHorizontal(x1 + 16 * 5 + 22, y1 + 11, textColor, 3);
}
void printDate() {
  if (RTC.yyyy != yy) {
    drawYYYY(backgroundColor);
    yy = RTC.yyyy;
    drawYYYY(textColor);
  }
  tft.setFont(Terminal11x16);
  if (RTC.mm != m) {
    drawMonth(backgroundColor);
    m = RTC.mm;
    drawMonth(textColor);
  }
  if (RTC.dd != d) {
    drawDay(backgroundColor);
    d = RTC.dd;
    drawDay(textColor);
  }
  if (RTC.dow != dow) {
    tft.drawText(x1 + 150, y1, dow2String(dow), backgroundColor);
    dow = RTC.dow;
    tft.drawText(x1 + 150, y1, dow2String(dow), textColor);
  }
}
void printTime() {
  if (RTC.h != hh) {
    drawHH(backgroundColor);
    hh = RTC.h;
    drawHH(textColor);
  }
  if (RTC.m != mm) {
    drawMM(backgroundColor);
    mm = RTC.m;
    drawMM(textColor);
  }
  if (RTC.s != ss) {
    drawSS(backgroundColor);
    ss = RTC.s;
    drawSS(textColor);
  }
}
void drawYYYY(uint16_t color) {
  String f = String::String(yy);
  Serial.print(f);
  printDigit(f.substring(0, 1).toInt(), x1, y1, color, 3);
  printDigit(f.substring(1, 2).toInt(), x1 + 16, y1, color, 3);
  printDigit(f.substring(2, 3).toInt(), x1 + 16 * 2, y1, color, 3);
  printDigit(f.substring(3).toInt(), x1 + 16 * 3, y1, color, 3);
}
void drawMonth(uint16_t color) {
  String f = String::String(p2dig(m));
  printDigit(f.substring(0, 1).toInt(), x1 + 16 * 3 + 28, y1, color, 3);
  printDigit(f.substring(1).toInt(), x1 + 16 * 4 + 28, y1, color, 3);
}
void drawDay(uint16_t color) {
  String f = String::String(p2dig(d));
  printDigit(f.substring(0, 1).toInt(), x1 + 16 * 5 + 38, y1, color, 3);
  printDigit(f.substring(1).toInt(), x1 + 16 * 6 + 38, y1, color, 3);
}
void drawHH(uint16_t color) {
  String f = String::String(p2dig(hh));
  printDigit(f.substring(0, 1).toInt(), x0, y0, color, 1);
  printDigit(f.substring(1).toInt(), x0 + 47, y0, color, 1);
}
void drawMM(uint16_t color) {
  String f = String::String(p2dig(mm));
  printDigit(f.substring(0, 1).toInt(), x0 + 47 * 2 + 11, y0, color, 1);
  printDigit(f.substring(1).toInt(), x0 + 47 * 2 + 11 + 47, y0, color, 1);
}
void drawSS(uint16_t color) {
  String f = String::String(p2dig(ss));
  printDigit(f.substring(0, 1).toInt(), x2, y2, color, 2);
  printDigit(f.substring(1).toInt(), x2 + 24, y2, color, 2);
}
void printDigit(int digit, uint16_t x, uint16_t y, uint16_t color, uint16_t mult) {
  switch (digit) {
    case 0:
      drawO(x, y, color, mult);
      break;
    case 1:
      drawOne(x, y, color, mult);
      break;
    case 2:
      drawTwo(x, y, color, mult);
      break;
    case 3:
      drawThree(x, y, color, mult);
      break;
    case 4:
      drawFour(x, y, color, mult);
      break;
    case 5:
      drawFive(x, y, color, mult);
      break;
    case 6:
      drawSix(x, y, color, mult);
      break;
    case 7:
      drawSeven(x, y, color, mult);
      break;
    case 8:
      drawEight(x, y, color, mult);
      break;
    case 9:
      drawNine(x, y, color, mult);
      break;
  }
}
void drawNine(uint16_t x, uint16_t y, uint16_t color, uint16_t mult) {
  drawHorizontal(x, y, color, mult);      //      -
  drawVertical(x - 4 / mult, y + 8 / mult, color, mult);//     |
  drawHorizontal(x - 18 / mult, y + 62 / mult, color, mult);//  -
  drawVertical(x + 29 / mult, y + 10 / mult, color, mult);// .    |
  drawVertical(x + 21 / mult, y + 42 / mult, color, mult);// .    |
  drawHorizontal(x - 9 / mult, y + 33 / mult, color, mult);// .  _
}
void drawEight(uint16_t x, uint16_t y, uint16_t color, uint16_t mult) {
  drawHorizontal(x, y, color, mult);      //      -
  drawVertical(x - 4 / mult, y + 8 / mult, color, mult);//     |
  drawVertical(x - 13 / mult, y + 39 / mult, color, mult);//   |
  drawHorizontal(x - 18 / mult, y + 62 / mult, color, mult);//  -
  drawVertical(x + 29 / mult, y + 10 / mult, color, mult);// .    |
  drawVertical(x + 21 / mult, y + 42 / mult, color, mult);// .    |
  drawHorizontal(x - 9 / mult, y + 33 / mult, color, mult);// .  _
}
void drawSeven(uint16_t x, uint16_t y, uint16_t color, uint16_t mult) {
  drawHorizontal(x, y, color, mult);      //      -
  drawVertical(x + 29 / mult, y + 10 / mult, color, mult);// .    |
  drawVertical(x + 21 / mult, y + 42 / mult, color, mult);// .    |
}
void drawSix(uint16_t x, uint16_t y, uint16_t color, uint16_t mult) {
  drawHorizontal(x, y, color, mult);      //      -
  drawVertical(x - 4 / mult, y + 8 / mult, color, mult);//     |
  drawVertical(x - 13 / mult, y + 39 / mult, color, mult);//   |
  drawHorizontal(x - 18 / mult, y + 62 / mult, color, mult);//  -
  drawVertical(x + 21 / mult, y + 42 / mult, color, mult);// .    |
  drawHorizontal(x - 9 / mult, y + 33 / mult, color, mult);// .  _
}
void drawFive(uint16_t x, uint16_t y, uint16_t color, uint16_t mult) {
  drawHorizontal(x, y, color, mult);      //      -
  drawVertical(x - 4 / mult, y + 8 / mult, color, mult);//     |
  drawHorizontal(x - 18 / mult, y + 62 / mult, color, mult);//  -
  drawVertical(x + 21 / mult, y + 42 / mult, color, mult);// .    |
  drawHorizontal(x - 9 / mult, y + 33 / mult, color, mult);// .  _
}
void drawFour(uint16_t x, uint16_t y, uint16_t color, uint16_t mult) {
  drawVertical(x - 4 / mult, y + 8 / mult, color, mult);//     |
  drawVertical(x + 29 / mult, y + 10 / mult, color, mult);// .    |
  drawVertical(x + 21 / mult, y + 42 / mult, color, mult);// .    |
  drawHorizontal(x - 9 / mult, y + 33 / mult, color, mult);// .  _
}
void drawThree(uint16_t x, uint16_t y, uint16_t color, uint16_t mult) {
  drawHorizontal(x, y, color, mult);      //      -
  drawHorizontal(x - 18 / mult, y + 62 / mult, color, mult);//  -
  drawVertical(x + 29 / mult, y + 10 / mult, color, mult);// .    |
  drawVertical(x + 21 / mult, y + 42 / mult, color, mult);// .    |
  drawHorizontal(x - 9 / mult, y + 33 / mult, color, mult);// .  _
}
void drawTwo(uint16_t x, uint16_t y, uint16_t color, uint16_t mult) {
  drawHorizontal(x, y, color, mult);      //      -
  drawVertical(x - 13 / mult, y + 39 / mult, color, mult);//   |
  drawHorizontal(x - 18 / mult, y + 62 / mult, color, mult);//  -
  drawVertical(x + 29 / mult, y + 10 / mult, color, mult);// .    |
  drawHorizontal(x - 9 / mult, y + 33 / mult, color, mult);// .  _
}
void drawOne(uint16_t x, uint16_t y, uint16_t color, uint16_t mult) {
  drawVertical(x + 29 / mult, y + 10 / mult, color, mult);// .    |
  drawVertical(x + 21 / mult, y + 42 / mult, color, mult);// .    |
}
void drawO(uint16_t x, uint16_t y, uint16_t color, uint16_t mult) {
  drawHorizontal(x, y, color, mult);      //      -
  drawVertical(x - 4 / mult, y + 8 / mult, color, mult);//     |
  drawVertical(x - 13 / mult, y + 39 / mult, color, mult);//   |
  drawHorizontal(x - 18 / mult, y + 62 / mult, color, mult);//  -
  drawVertical(x + 29 / mult, y + 10 / mult, color, mult);// .    |
  drawVertical(x + 21 / mult, y + 42 / mult, color, mult);// .    |
}
void drawVertical(uint16_t x, uint16_t y, uint16_t color, uint16_t mult) {
  tft.drawLine(x, y, x - 6 / mult, y + 22 / mult, color);
  tft.drawLine(x - 6 / mult, y + 22 / mult, x - 10 / mult, y + 24 / mult, color);
  tft.drawLine(x - 10 / mult, y + 24 / mult, x - 13 / mult, y + 20 / mult, color);
  tft.drawLine(x - 13 / mult, y + 20 / mult, x - 7 / mult, y, color);
  tft.drawLine(x - 7 / mult, y, x - 3 / mult, y - 4 / mult, color);
  tft.drawLine(x - 3 / mult, y - 4 / mult, x, y, color);
}
void drawHorizontal(uint16_t x, uint16_t y, uint16_t color, uint16_t mult) {
  tft.drawLine(x, y, x + 22 / mult, y, color);
  tft.drawLine(x + 22 / mult, y, x + 25 / mult, y + 3 / mult, color);
  tft.drawLine(x + 25 / mult, y + 3 / mult, x + 22 / mult, y + 7 / mult, color);
  tft.drawLine(x + 22 / mult, y + 7 / mult, x, y + 7 / mult, color);
  tft.drawLine(x, y + 7 / mult, x - 3 / mult, y + 3 / mult, color);
  tft.drawLine(x - 3 / mult, y + 3 / mult, x, y, color);
}
String p2dig(uint8_t v) {
  String d = String::String(v);
  if (v < 10) {
    d = "0" + d;
  }
  return d;
}
void inputTime(void) {
  RTC.yyyy = i2dig(DEC) * 100 + i2dig(DEC);
  RTC.mm = i2dig(DEC);
  RTC.dd = i2dig(DEC);

  RTC.h = i2dig(DEC);
  RTC.m = i2dig(DEC);
  RTC.s = i2dig(DEC);

  RTC.dow = i2dig(DEC);
}
uint8_t i2dig(uint8_t mode) {
  uint8_t  v = 0;
  char    c[3] = "00";

  c[0] = readNext();
  c[1] = readNext();

  switch (mode) {
    case DEC: v = atoi(c);  break;
    case HEX: v = (htoi(c[0]) << 4) + htoi(c[1]); ;  break;
  }
  return (v);
}

char readNext() {
  char  c;
  do {
    c = Serial.read();
  } while (isspace(c));
  return (c);
}

uint8_t htoi(char c) {
  c = toupper(c);
  if (c >= '0' && c <= '9')
    return (c - '0');
  else if (c >= 'A' && c <= 'F')
    return (c - 'A' + 10);
  else
    return (0);
}

const char *dow2String(uint8_t code) {
  static const char *str[] = {" ---", " Mon", " Tue", " Wed", " Thu", " Fri", " Sat", " Sun"};
  return (str[code]);
}

