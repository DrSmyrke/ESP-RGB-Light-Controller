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
#define DEFAULT_BRIGHTNESS										37

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
		unsigned char use_out1								: 1;
		unsigned char use_out2								: 1;
		unsigned char use_out3								: 1;
		unsigned char use_out4								: 1;
		unsigned char out1_state							: 1;
		unsigned char out2_state							: 1;
		unsigned char out3_state							: 1;
		unsigned char out4_state							: 1;
	} flags;
	uint8_t mode;
	uint8_t port1_leds;
	uint8_t port2_leds;
	uint8_t port3_leds;
	uint8_t port4_leds;
	uint8_t port5_leds;
	uint8_t port1_bright;
	uint8_t port2_bright;
	uint8_t port3_bright;
	uint8_t port4_bright;
	uint8_t port5_bright;
	Zone port1zones[ ZONES_AT_PORT ];
	Zone port2zones[ ZONES_AT_PORT ];
	Zone port3zones[ ZONES_AT_PORT ];
	Zone port4zones[ ZONES_AT_PORT ];
	Zone port5zones[ ZONES_AT_PORT ];
};

//----------- VARIABLES --------------------------------------------------------------------

extern AppData app;
extern ESP8266WebServer webServer;
extern Adafruit_NeoPixel port1;
extern Adafruit_NeoPixel port2;
extern Adafruit_NeoPixel port3;
extern Adafruit_NeoPixel port4;
extern Adafruit_NeoPixel port5;
extern char pageBuff[ WEB_PAGE_BUFF_SIZE ];
extern Pixel rainbow[ 7 ];
extern uint16_t m_animationCounter;
extern uint16_t m_animationCounterMax;

//----------- FUNCTIONS--------------------------------------------------------------------
void timer0Interrupt(void*);
void timer1Interrupt(void*);
void saveSettings(void);
void loadSettings(void);

//-----------------------------------------------------------------------------------------







#endif //_MAIN_H_