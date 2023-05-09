#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

//-----------------------------------------------------------------------------------------
#include "main.h"

//-----------------------------------------------------------------------------------------


//----------- DEFINES ---------------------------------------------------------------------


//----------- VARIABLES -------------------------------------------------------------------


//----------- FUNCTIONS--------------------------------------------------------------------
void lenta_clear(Adafruit_NeoPixel &lenta);
void setLed(Adafruit_NeoPixel &lenta, const uint16_t ledNum, uint8_t r, uint8_t g, uint8_t b, const bool show = false);
void setLed(Adafruit_NeoPixel &lenta, const uint16_t ledNum, uint32_t pixel, const bool show = false);
void setBrightnessPrz(Adafruit_NeoPixel &lenta, const uint8_t percent);
void setBrightness(Adafruit_NeoPixel &lenta, const uint8_t value);
void startMode0(void);
void startMode1(void);
void startMode2(void);
void animationStart(void);
void animationProcess(void);
void clearAll(void);
void getPageHeadler(void);
void setPageHeadler(void);
void indexPageHeadler(void);
String getValue(String data, char separator, int index);

//-----------------------------------------------------------------------------------------




#endif //__FUNCTIONS_H__