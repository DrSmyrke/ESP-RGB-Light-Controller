#ifndef _MAIN_H_
#define _MAIN_H_

//-----------------------------------------------------------------------------------------
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <esp_functions.h>
#include <DNS_Server.h>
#include <timer.h>
#include <Adafruit_NeoPixel.h>
// #include <FastLED.h>
#include "functions.h"
#include "effects.h"

//----------- DEFINES ----------------------------------------------------------------------
#define DEVICE_NAME												"RGB_WS2812_Controller"
#define AP_ADMIN_USERNAME										"admin"
#define LENTA1_COUNT											8
#define LENTA2_COUNT											8
#define LENTA3_COUNT											8
#define LENTA4_COUNT											8
#define LENTA5_COUNT											8
#define LENTA1_PIN												D8
#define LENTA2_PIN												D7
#define LENTA3_PIN												D6
#define LENTA4_PIN												D7
#define LENTA5_PIN												D3
#define OUT1_PIN												D2
#define OUT2_PIN												D1
#define OUT3_PIN												D0
#define OUT4_PIN												10
#define WEB_PAGE_BUFF_SIZE										2048
#define ZONES_AT_PORT											5

#define setPlus(reg,bit) reg |= (1<<bit)
#define setZero(reg,bit) reg &= ~(1<<bit)
#define ibi(reg,bit) reg ^= (1<<bit)
#define CheckBit(reg,bit) (reg&(1<<bit))

//-----------------------------------------------------------------------------------------
struct Pixel{
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

struct Zone{
	uint32_t color;
	uint16_t count;
};

struct AppData{
	struct{
		unsigned char timer0								: 1;
		unsigned char timer1								: 1;
		unsigned char use_port1								: 1;
		unsigned char use_port2								: 1;
		unsigned char use_port3								: 1;
		unsigned char use_port4								: 1;
		unsigned char use_port5								: 1;
	} flags;
	uint8_t mode;
	uint8_t lenta1_leds;
	uint8_t lenta2_leds;
	uint8_t lenta3_leds;
	uint8_t lenta4_leds;
	uint8_t lenta5_leds;
	Zone port1zones[ ZONES_AT_PORT ];
	Zone port2zones[ ZONES_AT_PORT ];
	Zone port3zones[ ZONES_AT_PORT ];
	Zone port4zones[ ZONES_AT_PORT ];
	Zone port5zones[ ZONES_AT_PORT ];
};

//----------- VARIABLES --------------------------------------------------------------------

extern AppData app;
extern ESP8266WebServer webServer;
extern Adafruit_NeoPixel lenta1;
extern Adafruit_NeoPixel lenta2;
extern Adafruit_NeoPixel lenta3;
extern Adafruit_NeoPixel lenta4;
extern Adafruit_NeoPixel lenta5;
extern char pageBuff[ WEB_PAGE_BUFF_SIZE ];
extern Pixel rainbow[ 7 ];

//----------- FUNCTIONS--------------------------------------------------------------------
void timer0Interrupt(void*);
void timer1Interrupt(void*);
void saveSettings(void);
void loadSettings(void);

//-----------------------------------------------------------------------------------------







#endif //_MAIN_H_