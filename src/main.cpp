#include "main.h"

//----------- VARIABLES --------------------------------------------------------------------
AppData app;
ESP8266WebServer webServer( 80 );
DNS_Server dnsServer;
Timer timer0( 0, 1000, timer0Interrupt );
Timer timer1( 0, 25, timer1Interrupt );
// CRGB leds1[ LENTA1_COUNT ];
// CRGB leds2[ LENTA2_COUNT ];
Adafruit_NeoPixel lenta1( LENTA1_COUNT, LENTA1_PIN, NEO_GRB + NEO_KHZ800 );
Adafruit_NeoPixel lenta2( LENTA2_COUNT, LENTA2_PIN, NEO_GRB + NEO_KHZ800 );
Adafruit_NeoPixel lenta3( LENTA3_COUNT, LENTA3_PIN, NEO_GRB + NEO_KHZ800 );
Adafruit_NeoPixel lenta4( LENTA4_COUNT, LENTA4_PIN, NEO_GRB + NEO_KHZ800 );
Adafruit_NeoPixel lenta5( LENTA5_COUNT, LENTA5_PIN, NEO_GRB + NEO_KHZ800 );
char pageBuff[ WEB_PAGE_BUFF_SIZE ];
Pixel rainbow[ 7 ];

//----------- FUNCTIONS--------------------------------------------------------------------
void setup()
{
	app.flags.timer0						= 0;
	app.flags.timer1						= 0;
	app.flags.use_port1						= 0;
	app.flags.use_port2						= 0;
	app.flags.use_port3						= 0;
	app.flags.use_port4						= 0;
	app.flags.use_port5						= 0;
	app.mode								= 0;
	app.lenta1_leds							= LENTA1_COUNT;
	app.lenta2_leds							= LENTA2_COUNT;
	app.lenta3_leds							= LENTA3_COUNT;
	app.lenta4_leds							= LENTA4_COUNT;
	app.lenta5_leds							= LENTA5_COUNT;

	for( uint8_t i = 0; i < ZONES_AT_PORT; i++ ){
		app.port1zones[ i ].color			= 0;
		app.port1zones[ i ].count			= 0;
		app.port2zones[ i ].color			= 0;
		app.port2zones[ i ].count			= 0;
		app.port3zones[ i ].color			= 0;
		app.port3zones[ i ].count			= 0;
		app.port4zones[ i ].color			= 0;
		app.port4zones[ i ].count			= 0;
		app.port5zones[ i ].color			= 0;
		app.port5zones[ i ].count			= 0;
	}


	ESP.wdtEnable( 10000 );

	esp::init();
	esp::setVersion( 0, 1, FIRMWARE_REVISION );
	
	pinMode( LED_BUILTIN, OUTPUT );
	
#ifdef __DEV
	while( !Serial );
	Serial.begin( 115200 );
#endif
	ESP_DEBUG( "INIT...\n" );

	loadSettings();

	// FastLED.addLeds<NEOPIXEL, LENTA1_PIN>( leds1, LENTA1_COUNT );

	if( app.flags.use_port1 ){
		lenta1.updateLength( app.lenta1_leds );
		lenta1.begin();
		lenta_clear( lenta1 );
	}
	if( app.flags.use_port2 ){
		lenta2.updateLength( app.lenta2_leds );
		lenta2.begin();
		lenta_clear( lenta2 );
	}
	if( app.flags.use_port3 ){
		lenta3.updateLength( app.lenta3_leds );
		lenta3.begin();
		lenta_clear( lenta3 );
	}
	if( app.flags.use_port4 ){
		lenta4.updateLength( app.lenta4_leds );
		lenta4.begin();
		lenta_clear( lenta4 );
	}
	if( app.flags.use_port5 ){
		lenta5.updateLength( app.lenta5_leds );
		lenta5.begin();
		lenta_clear( lenta5 );
	}


	//Инициализация Wi-Fi
	IPAddress apIP( 10, 10, 10, 10 );
	esp::wifi_init( DEVICE_NAME, apIP, apIP, IPAddress( 255, 255, 255, 0 ) );
	if( esp::flags.ap_mode ){
		dnsServer.setErrorReplyCode( DNSReplyCode::NoError );
		dnsServer.addRecord( "*", WiFi.softAPIP() );
		dnsServer.start( DNS_PORT );
	}

	//Инициализация Web сервера
	esp::pageBuff = pageBuff;
	esp::addWebServerPages( &webServer, true, true, true );
	esp::addWebUpdate( &webServer, "" );
	// webServer.on( "/storageReset", [ webServer ](void){
	// 	if( esp::checkWebAuth( &webServer, SYSTEM_LOGIN, SYSTEM_PASSWORD, ESP_AUTH_REALM, "access denied" ) ){
	// 		webServer.send ( 200, "text/html", "OK" );
	// 		homekit_storage_reset();
	// 		ESP.reset();
	// 	}
	// } );
	webServer.on( "/", indexPageHeadler );
	if( !esp::flags.ap_mode ) webServer.onNotFound( indexPageHeadler );
	webServer.on( "/get", getPageHeadler );
	webServer.on( "/set", setPageHeadler );
	webServer.begin();




	animationStart();
	

	ESP_DEBUG( "INIT OK\n" );
}

//-----------------------------------------------------------------------------------------
void loop()
{
	ESP.wdtFeed();

	
	//Условие срабатывания по таймеру0 (1сек.)
	if( app.flags.timer0 ){
		app.flags.timer0 = 0;

		//Меняем состояние светодиода для индикации корректности работы
		digitalWrite( LED_BUILTIN, !digitalRead( LED_BUILTIN ) );
	}

	//Условие срабатывания по таймеру1 (25мсек.)
	if( app.flags.timer1 ){
		app.flags.timer1 = 0;

		animationProcess();
	}




	webServer.handleClient();

	if( esp::flags.ap_mode ){
		dnsServer.processNextRequest();
	}
}

//-----------------------------------------------------------------------------------------
void timer0Interrupt(void*)
{
	app.flags.timer0 = 1;
}

//-----------------------------------------------------------------------------------------
void timer1Interrupt(void*)
{
	app.flags.timer1 = 1;
}

//-----------------------------------------------------------------------------------------
void saveSettings(void)
{
	ESP_DEBUG( "Save settings\n" );

	uint8_t data[ sizeof( AppData ) + 1 ];
	data[ 0 ]								= 0;
	data[ 1 ]								= app.lenta1_leds;
	data[ 2 ]								= app.lenta2_leds;
	data[ 3 ]								= app.lenta3_leds;
	data[ 4 ]								= app.lenta4_leds;
	data[ 5 ]								= app.lenta5_leds;
	data[ 6 ]								= app.mode;
	//zones
	uint8_t offset							= 7;
	uint8_t i;
	for( i = 0; i < ZONES_AT_PORT; i++ ){
		data[ offset++ ]					= app.port1zones[ i ].color >> 24;
		data[ offset++ ]					= app.port1zones[ i ].color >> 16;
		data[ offset++ ]					= app.port1zones[ i ].color >> 8;
		data[ offset++ ]					= app.port1zones[ i ].color;
		data[ offset++ ]					= app.port1zones[ i ].count >> 8;
		data[ offset++ ]					= app.port1zones[ i ].count;
	}
	for( i = 0; i < ZONES_AT_PORT; i++ ){
		data[ offset++ ]					= app.port2zones[ i ].color >> 24;
		data[ offset++ ]					= app.port2zones[ i ].color >> 16;
		data[ offset++ ]					= app.port2zones[ i ].color >> 8;
		data[ offset++ ]					= app.port2zones[ i ].color;
		data[ offset++ ]					= app.port2zones[ i ].count >> 8;
		data[ offset++ ]					= app.port2zones[ i ].count;
	}
	for( i = 0; i < ZONES_AT_PORT; i++ ){
		data[ offset++ ]					= app.port3zones[ i ].color >> 24;
		data[ offset++ ]					= app.port3zones[ i ].color >> 16;
		data[ offset++ ]					= app.port3zones[ i ].color >> 8;
		data[ offset++ ]					= app.port3zones[ i ].color;
		data[ offset++ ]					= app.port3zones[ i ].count >> 8;
		data[ offset++ ]					= app.port3zones[ i ].count;
	}
	for( i = 0; i < ZONES_AT_PORT; i++ ){
		data[ offset++ ]					= app.port4zones[ i ].color >> 16;
		data[ offset++ ]					= app.port4zones[ i ].color >> 24;
		data[ offset++ ]					= app.port4zones[ i ].color >> 8;
		data[ offset++ ]					= app.port4zones[ i ].color;
		data[ offset++ ]					= app.port4zones[ i ].count >> 8;
		data[ offset++ ]					= app.port4zones[ i ].count;
	}
	for( i = 0; i < ZONES_AT_PORT; i++ ){
		data[ offset++ ]					= app.port5zones[ i ].color >> 24;
		data[ offset++ ]					= app.port5zones[ i ].color >> 16;
		data[ offset++ ]					= app.port5zones[ i ].color >> 8;
		data[ offset++ ]					= app.port5zones[ i ].color;
		data[ offset++ ]					= app.port5zones[ i ].count >> 8;
		data[ offset++ ]					= app.port5zones[ i ].count;
	}

	if( app.flags.use_port1 ) setPlus( data[ 0 ], 0 );
	if( app.flags.use_port2 ) setPlus( data[ 0 ], 1 );
	if( app.flags.use_port3 ) setPlus( data[ 0 ], 2 );
	if( app.flags.use_port4 ) setPlus( data[ 0 ], 3 );
	if( app.flags.use_port5 ) setPlus( data[ 0 ], 4 );

	esp::saveSettings( data, sizeof( data ) );
}

//-----------------------------------------------------------------------------------------
void loadSettings(void)
{
	ESP_DEBUG( "Load settings\n" );

	uint8_t data[ sizeof( AppData ) + 1 ];
	if( esp::loadSettings( data, sizeof( data ) ) == sizeof( data ) ){
		app.flags.use_port1					= ( CheckBit( data[ 0 ], 0 ) ) ? 1 : 0;
		app.flags.use_port2					= ( CheckBit( data[ 0 ], 1 ) ) ? 1 : 0;
		app.flags.use_port3					= ( CheckBit( data[ 0 ], 2 ) ) ? 1 : 0;
		app.flags.use_port4					= ( CheckBit( data[ 0 ], 3 ) ) ? 1 : 0;
		app.flags.use_port5					= ( CheckBit( data[ 0 ], 4 ) ) ? 1 : 0;
		
		app.lenta1_leds						= data[ 1 ];
		app.lenta2_leds						= data[ 2 ];
		app.lenta3_leds						= data[ 3 ];
		app.lenta4_leds						= data[ 4 ];
		app.lenta5_leds						= data[ 5 ];
		app.mode							= data[ 6 ];
		//zones
		uint8_t offset						= 7;
		uint8_t i;
		for( i = 0; i < ZONES_AT_PORT; i++ ){
			app.port1zones[ i ].color		= ( ( data[ offset++ ] << 24 ) | ( data[ offset++ ] << 16 ) | ( data[ offset++ ] << 8 ) | ( data[ offset++ ] ) );
			app.port1zones[ i ].count		= ( ( data[ offset++ ] << 8 ) | ( data[ offset++ ] ) );
		}
		for( i = 0; i < ZONES_AT_PORT; i++ ){
			app.port2zones[ i ].color		= ( ( data[ offset++ ] << 24 ) | ( data[ offset++ ] << 16 ) | ( data[ offset++ ] << 8 ) | ( data[ offset++ ] ) );
			app.port2zones[ i ].count		= ( ( data[ offset++ ] << 8 ) | ( data[ offset++ ] ) );
		}
		for( i = 0; i < ZONES_AT_PORT; i++ ){
			app.port3zones[ i ].color		= ( ( data[ offset++ ] << 24 ) | ( data[ offset++ ] << 16 ) | ( data[ offset++ ] << 8 ) | ( data[ offset++ ] ) );
			app.port3zones[ i ].count		= ( ( data[ offset++ ] << 8 ) | ( data[ offset++ ] ) );
		}
		for( i = 0; i < ZONES_AT_PORT; i++ ){
			app.port4zones[ i ].color		= ( ( data[ offset++ ] << 24 ) | ( data[ offset++ ] << 16 ) | ( data[ offset++ ] << 8 ) | ( data[ offset++ ] ) );
			app.port4zones[ i ].count		= ( ( data[ offset++ ] << 8 ) | ( data[ offset++ ] ) );
		}
		for( i = 0; i < ZONES_AT_PORT; i++ ){
			app.port5zones[ i ].color		= ( ( data[ offset++ ] << 24 ) | ( data[ offset++ ] << 16 ) | ( data[ offset++ ] << 8 ) | ( data[ offset++ ] ) );
			app.port5zones[ i ].count		= ( ( data[ offset++ ] << 8 ) | ( data[ offset++ ] ) );
		}
	}

#ifdef __DEV
	ESP_DEBUG( "MODE: %d\n", app.mode );
	if( app.flags.use_port1 ) ESP_DEBUG( "Port 1 [ACTIVATED]\n" );
	if( app.flags.use_port2 ) ESP_DEBUG( "Port 2 [ACTIVATED]\n" );
	if( app.flags.use_port3 ) ESP_DEBUG( "Port 3 [ACTIVATED]\n" );
	if( app.flags.use_port4 ) ESP_DEBUG( "Port 4 [ACTIVATED]\n" );
	if( app.flags.use_port5 ) ESP_DEBUG( "Port 5 [ACTIVATED]\n" );
#endif
}

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
