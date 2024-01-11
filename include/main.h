#ifndef _MAIN_H_
#define _MAIN_H_

//-----------------------------------------------------------------------------------------
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <esp_functions.h>
#include <timer.h>
#include <WebSocketsServer.h>
#include <FastLED.h>
#include "functions.h"
#include "effects.h"

//----------- DEFINES ----------------------------------------------------------------------
#define DEVICE_NAME												"RGB_Light_Controller"
#define AP_ADMIN_USERNAME										"admin"
#define LENTA_COUNT_MAX											1024
#define LENTS_COUNT_MAX											8
#define LENTS_COUNT												5
#define OUTS_COUNT												4
#define LENTA1_SIZE												8
#define LENTA2_SIZE												8
#define LENTA3_SIZE												8
#define LENTA4_SIZE												8
#define LENTA5_SIZE												8
#define LENTA1_PIN												D8
#define LENTA2_PIN												D7
#define LENTA3_PIN												D6
#define LENTA4_PIN												D5
#define LENTA5_PIN												D3
#define OUT1_PIN												D2
#define OUT2_PIN												D1
#define OUT3_PIN												D0
#define OUT4_PIN												10
#define WEB_PAGE_BUFF_SIZE										1024
#define ZONES_MAX												25
#define DEFAULT_BRIGHTNESS										37
#define MAX_BRIGHTNESS											255

#define setPlus(reg,bit) reg |= (1<<bit)
#define setZero(reg,bit) reg &= ~(1<<bit)
#define ibi(reg,bit) reg ^= (1<<bit)
#define CheckBit(reg,bit) (reg&(1<<bit))

#ifndef FIRMWARE_REVISION
	#define FIRMWARE_REVISION									0
#endif

//-----------------------------------------------------------------------------------------
// #pragma pack(push,1)

namespace WS{
	struct Header{
		uint8_t cmd;
		uint8_t paramID;
	};

	struct Packet{
		Header header;
		uint8_t* data;
	};
	struct Packet_zone{
		Header header;
		uint8_t zoneNum;
		uint8_t port;
		uint8_t start[ 2 ];
		uint8_t count[ 2 ];
	};
	struct Packet_effect{
		Header header;
		uint8_t effectID;
		uint8_t dataLen;
		uint8_t data[ 12 ];
	};
	struct Packet_portSize{
		Header header;
		uint8_t num;
		uint8_t data[ 2 ];
	};
};

struct Zone{
	uint8_t port;
	uint16_t start;
	uint16_t count;
};

struct Color
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

struct Order
{
	uint8_t port;
	uint8_t dir;
};


struct AppData{
	struct{
		unsigned char timer0								: 1;
		unsigned char timer1								: 1;
	} flags;
	
	CRGB *port1;
	CRGB *port2;
	CRGB *port3;
	CRGB *port4;
	CRGB *port5;

	uint8_t orderNum;
	struct{
		uint8_t pulse_current_color;
	} effects;

	struct{
		uint8_t startByte;
		uint8_t mode;
		uint8_t portsMax;
		uint8_t outsMax;
		struct{
			unsigned char port1									: 1;
			unsigned char port2									: 1;
			unsigned char port3									: 1;
			unsigned char port4									: 1;
			unsigned char port5									: 1;
			unsigned char port6									: 1;
			unsigned char port7									: 1;
			unsigned char port8									: 1;
			unsigned char out1									: 1;
			unsigned char out2									: 1;
			unsigned char out3									: 1;
			unsigned char out4									: 1;
			unsigned char out5									: 1;
			unsigned char out6									: 1;
			unsigned char out7									: 1;
			unsigned char out8									: 1;
		} use;
		uint16_t port1_size;
		uint16_t port2_size;
		uint16_t port3_size;
		uint16_t port4_size;
		uint16_t port5_size;
		uint16_t port6_size;
		uint16_t port7_size;
		uint16_t port8_size;
		struct{
			unsigned char out1									: 1;
			unsigned char out2									: 1;
			unsigned char out3									: 1;
			unsigned char out4									: 1;
			unsigned char out5									: 1;
			unsigned char out6									: 1;
			unsigned char out7									: 1;
			unsigned char out8									: 1;
		} outsState;
		uint8_t zonesCount;
		Zone zones[ ZONES_MAX ];
		struct{
			uint8_t rainbow_anim_delay;
			uint8_t rainbow_speed;
			uint8_t rainbow_step;
			Order rainbow_orders[ LENTS_COUNT_MAX ];
			uint8_t fire_anim_delay;
			uint8_t fire_speed;
			uint8_t fire_step;
			uint8_t fire_hue_gap;
			uint8_t fire_hue_start;
			uint8_t fire_min_bright;
			uint8_t fire_max_bright;
			uint8_t fire_min_sat;
			uint8_t fire_max_sat;
			uint8_t pulse_anim_delay;
			uint8_t pulse_speed;
			uint8_t pulse_step;
			Order pulse_orders[ LENTS_COUNT_MAX ];
			Color masterColor;
			Color zonesColor[ ZONES_MAX ];
		} effects;
	} param;
};

// #pragma pack(pop)

//----------- VARIABLES --------------------------------------------------------------------

extern AppData app;
extern ESP8266WebServer webServer;
extern WebSocketsServer webSocket;
extern char pageBuff[ WEB_PAGE_BUFF_SIZE ];
extern uint16_t m_animationCounter;
extern uint16_t m_animationCounterMax;

//----------- FUNCTIONS--------------------------------------------------------------------
void timer0Interrupt(void*);
void timer1Interrupt(void*);
void saveSettings(void);
void loadSettings(void);

//-----------------------------------------------------------------------------------------







#endif //_MAIN_H_