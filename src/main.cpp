#include "main.h"

//----------- VARIABLES --------------------------------------------------------------------
AppData app;
ESP8266WebServer webServer( 80 );
DNS_Server dnsServer;
Timer timer0( 0, 1000, timer0Interrupt );
Timer timer1( 0, 25, timer1Interrupt );
// CRGB leds1[ LENTA1_COUNT ];
// CRGB leds2[ LENTA2_COUNT ];
Adafruit_NeoPixel port1( LENTA1_COUNT, LENTA1_PIN, NEO_GRB + NEO_KHZ800 );
Adafruit_NeoPixel port2( LENTA2_COUNT, LENTA2_PIN, NEO_GRB + NEO_KHZ800 );
Adafruit_NeoPixel port3( LENTA3_COUNT, LENTA3_PIN, NEO_GRB + NEO_KHZ800 );
Adafruit_NeoPixel port4( LENTA4_COUNT, LENTA4_PIN, NEO_GRB + NEO_KHZ800 );
Adafruit_NeoPixel port5( LENTA5_COUNT, LENTA5_PIN, NEO_GRB + NEO_KHZ800 );
char pageBuff[ WEB_PAGE_BUFF_SIZE ];
Pixel rainbow[ 7 ];
uint16_t m_animationCounter;
uint16_t m_animationCounterMax;

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
	app.port1_leds							= LENTA1_COUNT;
	app.port2_leds							= LENTA2_COUNT;
	app.port3_leds							= LENTA3_COUNT;
	app.port4_leds							= LENTA4_COUNT;
	app.port5_leds							= LENTA5_COUNT;
	app.port1_bright						= DEFAULT_BRIGHTNESS;
	app.port2_bright						= DEFAULT_BRIGHTNESS;
	app.port3_bright						= DEFAULT_BRIGHTNESS;
	app.port4_bright						= DEFAULT_BRIGHTNESS;
	app.port5_bright						= DEFAULT_BRIGHTNESS;
	app.masterColor							= (Color){ 0, 0, 0 };

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
	pinMode( OUT1_PIN, OUTPUT );
	pinMode( OUT2_PIN, OUTPUT );
	pinMode( OUT3_PIN, OUTPUT );
	pinMode( OUT4_PIN, OUTPUT );
	
#ifdef __DEV
	while( !Serial );
	Serial.begin( 115200 );
#endif
	ESP_DEBUG( "INIT...\n" );

	loadSettings();

	// FastLED.addLeds<NEOPIXEL, LENTA1_PIN>( leds1, LENTA1_COUNT );

	if( app.flags.use_port1 ){
		port1.updateLength( app.port1_leds );
		port1.begin();
		lenta_clear( port1 );
		setBrightnessPrz( port1, app.port1_bright );
	}
	if( app.flags.use_port2 ){
		port2.updateLength( app.port2_leds );
		port2.begin();
		lenta_clear( port2 );
		setBrightnessPrz( port2, app.port2_bright );
	}
	if( app.flags.use_port3 ){
		port3.updateLength( app.port3_leds );
		port3.begin();
		lenta_clear( port3 );
		setBrightnessPrz( port3, app.port3_bright );
	}
	if( app.flags.use_port4 ){
		port4.updateLength( app.port4_leds );
		port4.begin();
		lenta_clear( port4 );
		setBrightnessPrz( port4, app.port4_bright );
	}
	if( app.flags.use_port5 ){
		port5.updateLength( app.port5_leds );
		port5.begin();
		lenta_clear( port5 );
		setBrightnessPrz( port5, app.port5_bright );
	}
	
	resetOuts();


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
	uint8_t offset							= 0;
	data[ offset++ ]						= 0;
	data[ offset++ ]						= 0;
	data[ offset++ ]						= app.port1_leds;
	data[ offset++ ]						= app.port2_leds;
	data[ offset++ ]						= app.port3_leds;
	data[ offset++ ]						= app.port4_leds;
	data[ offset++ ]						= app.port5_leds;
	data[ offset++ ]						= app.port1_bright;
	data[ offset++ ]						= app.port2_bright;
	data[ offset++ ]						= app.port3_bright;
	data[ offset++ ]						= app.port4_bright;
	data[ offset++ ]						= app.port5_bright;
	data[ offset++ ]						= app.mode;
	//zones
	
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

	data[ offset++ ]						= app.masterColor.r;
	data[ offset++ ]						= app.masterColor.g;
	data[ offset++ ]						= app.masterColor.b;

	if( app.flags.use_port1 ) setPlus( data[ 0 ], 0 );
	if( app.flags.use_port2 ) setPlus( data[ 0 ], 1 );
	if( app.flags.use_port3 ) setPlus( data[ 0 ], 2 );
	if( app.flags.use_port4 ) setPlus( data[ 0 ], 3 );
	if( app.flags.use_port5 ) setPlus( data[ 0 ], 4 );

	if( app.flags.use_out1 ) setPlus( data[ 1 ], 0 );
	if( app.flags.use_out2 ) setPlus( data[ 1 ], 1 );
	if( app.flags.use_out3 ) setPlus( data[ 1 ], 2 );
	if( app.flags.use_out4 ) setPlus( data[ 1 ], 3 );
	if( app.flags.out1_state ) setPlus( data[ 1 ], 4 );
	if( app.flags.out2_state ) setPlus( data[ 1 ], 5 );
	if( app.flags.out3_state ) setPlus( data[ 1 ], 6 );
	if( app.flags.out4_state ) setPlus( data[ 1 ], 7 );

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

		app.flags.use_out1					= ( CheckBit( data[ 1 ], 0 ) ) ? 1 : 0;
		app.flags.use_out2					= ( CheckBit( data[ 1 ], 1 ) ) ? 1 : 0;
		app.flags.use_out3					= ( CheckBit( data[ 1 ], 2 ) ) ? 1 : 0;
		app.flags.use_out4					= ( CheckBit( data[ 1 ], 3 ) ) ? 1 : 0;
		app.flags.out1_state				= ( CheckBit( data[ 1 ], 4 ) ) ? 1 : 0;
		app.flags.out2_state				= ( CheckBit( data[ 1 ], 5 ) ) ? 1 : 0;
		app.flags.out3_state				= ( CheckBit( data[ 1 ], 6 ) ) ? 1 : 0;
		app.flags.out4_state				= ( CheckBit( data[ 1 ], 7 ) ) ? 1 : 0;
		
		uint8_t offset						= 2;
		app.port1_leds						= data[ offset++ ];
		app.port2_leds						= data[ offset++ ];
		app.port3_leds						= data[ offset++ ];
		app.port4_leds						= data[ offset++ ];
		app.port5_leds						= data[ offset++ ];
		app.port1_bright					= data[ offset++ ];
		app.port2_bright					= data[ offset++ ];
		app.port3_bright					= data[ offset++ ];
		app.port4_bright					= data[ offset++ ];
		app.port5_bright					= data[ offset++ ];
		app.mode							= data[ offset++ ];
		//zones
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

		app.masterColor.r					= data[ offset++ ];
		app.masterColor.g					= data[ offset++ ];
		app.masterColor.b					= data[ offset++ ];
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
