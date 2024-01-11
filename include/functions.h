#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

//-----------------------------------------------------------------------------------------
#include "main.h"


//----------- DEFINES ---------------------------------------------------------------------


//----------- VARIABLES -------------------------------------------------------------------


//----------- FUNCTIONS--------------------------------------------------------------------
void startMode0(void);
void startMode1(void);
void startMode2(void);
void animationStart(void);
void animationProcess(void);
CRGB* getPortPtr(const uint8_t portNum);
uint16_t getPortSize(const uint8_t portNum);
// void getPageHeadler(void);
// void setPageHeadler(void);
void indexPageHeadler(void);
String getValue(String data, char separator, int index);
void resetOuts(void);
void applySettings(void);
void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length);
/**
 * Move current led at next port at order list
 * @param uint8_t order index
 * @param Order* order list ptr
 * @param CRGB* color ptr
 * @param uint16_t offset at new port
 * @param uint8_t repeat all ports
 * @return none
*/
void moveColorAtNextPort(uint8_t oIndx, const struct Order* ordersList, const CRGB* color, const uint16_t offset = 0, const uint8_t repeat = 0);

//-----------------------------------------------------------------------------------------




#endif //__FUNCTIONS_H__