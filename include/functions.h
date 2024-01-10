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
// void getPageHeadler(void);
// void setPageHeadler(void);
void indexPageHeadler(void);
String getValue(String data, char separator, int index);
void resetOuts(void);
void applySettings(void);
void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length);

//-----------------------------------------------------------------------------------------




#endif //__FUNCTIONS_H__