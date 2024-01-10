#include "main.h"

//----------- VARIABLES --------------------------------------------------------------------
AppData app;
ESP8266WebServer webServer( 80 );
WebSocketsServer webSocket( 81 );
Timer timer0( 0, 1000, timer0Interrupt );
Timer timer1( 0, 1, timer1Interrupt );

// CRGB leds2[ LENTA2_COUNT ];
// Adafruit_NeoPixel port1( LENTA1_COUNT, LENTA2_PIN, NEO_GRB + NEO_KHZ800 );
// Adafruit_NeoPixel port2( LENTA2_COUNT, LENTA2_PIN, NEO_GRB + NEO_KHZ800 );
// Adafruit_NeoPixel port3( LENTA3_COUNT, LENTA3_PIN, NEO_GRB + NEO_KHZ800 );
// Adafruit_NeoPixel port4( LENTA4_COUNT, LENTA4_PIN, NEO_GRB + NEO_KHZ800 );
// Adafruit_NeoPixel port5( LENTA5_COUNT, LENTA5_PIN, NEO_GRB + NEO_KHZ800 );

char pageBuff[ WEB_PAGE_BUFF_SIZE ];

uint16_t m_animationCounter;
uint16_t m_animationCounterMax;

//----------- FUNCTIONS--------------------------------------------------------------------
void setup()
{
#ifdef __DEV
	while( !Serial );
	Serial.begin( 115200 );
#endif

	ESP_DEBUG( "---------------------------------------\n" );

	esp::init();
	esp::printAllFiles( Serial );
	esp::setVersion( 0, 1, FIRMWARE_REVISION );
	
	ESP_DEBUG( "---------------------------------------\n" );
	ESP_DEBUG( "BOOTING...\n" );

	app.flags.timer0											= 0;
	app.flags.timer1											= 0;

	app.port1													= nullptr;
	app.port2													= nullptr;
	app.port3													= nullptr;
	app.port4													= nullptr;
	app.port5													= nullptr;
	app.orderNum												= 0;

	app.param.startByte											= 0xA1;
	app.param.mode												= 0;
	app.param.portsMax											= LENTS_COUNT;
	app.param.outsMax											= OUTS_COUNT;
	app.param.use.port1											= 0;
	app.param.use.port2											= 0;
	app.param.use.port3											= 0;
	app.param.use.port4											= 0;
	app.param.use.port5											= 0;
	app.param.use.port6											= 0;
	app.param.use.port7											= 0;
	app.param.use.port8											= 0;
	app.param.use.out1											= 0;
	app.param.use.out2											= 0;
	app.param.use.out3											= 0;
	app.param.use.out4											= 0;
	app.param.use.out5											= 0;
	app.param.use.out6											= 0;
	app.param.use.out7											= 0;
	app.param.use.out8											= 0;
	
#ifdef LENTA1_SIZE
	app.param.port1_size										= LENTA1_SIZE;
#else
	app.param.port1_size										= 0;
#endif
#ifdef LENTA2_SIZE
	app.param.port2_size										= LENTA2_SIZE;
#else
	app.param.port2_size										= 0;
#endif
#ifdef LENTA3_SIZE
	app.param.port3_size										= LENTA3_SIZE;
#else
	app.param.port3_size										= 0;
#endif
#ifdef LENTA4_SIZE
	app.param.port4_size										= LENTA4_SIZE;
#else
	app.param.port4_size										= 0;
#endif
#ifdef LENTA5_SIZE
	app.param.port5_size										= LENTA5_SIZE;
#else
	app.param.port5_size										= 0;
#endif
#ifdef LENTA6_SIZE
	app.param.port6_size										= LENTA6_SIZE;
#else
	app.param.port6_size										= 0;
#endif
#ifdef LENTA7_SIZE
	app.param.port7_size										= LENTA7_SIZE;
#else
	app.param.port7_size										= 0;
#endif
#ifdef LENTA8_SIZE
	app.param.port8_size										= LENTA8_SIZE;
#else
	app.param.port8_size										= 0;
#endif

	app.param.effects.rainbow_speed								= EFFECT_RAINBOW_SPEED;
	app.param.effects.rainbow_step								= EFFECT_RAINBOW_STEP;
	app.param.effects.fire_speed								= EFFECT_FIRE_SPEED;
	app.param.effects.fire_step									= EFFECT_FIRE_STEP;
	app.param.effects.fire_hue_gap								= EFFECT_FIRE_HUE_GAP;
	app.param.effects.fire_hue_start							= EFFECT_FIRE_HUE_START;
	app.param.effects.fire_min_bright							= EFFECT_FIRE_MIN_BRIGHT;
	app.param.effects.fire_max_bright							= EFFECT_FIRE_MAX_BRIGHT;
	app.param.effects.fire_min_sat								= EFFECT_FIRE_MIN_SAT;
	app.param.effects.fire_max_sat								= EFFECT_FIRE_MAX_SAT;
	app.param.effects.pulse_speed								= EFFECT_PULSE_SPEED;
	app.param.effects.pulse_step								= EFFECT_PULSE_STEP;

	app.param.effects.masterColor								= { 0, 255, 0 };

	for( uint8_t i = 0; i < LENTS_COUNT_MAX; i++ ){
		app.param.effects.rainbow_orders[ i ].port				= i + 1;
		app.param.effects.rainbow_orders[ i ].dir				= 1;
		app.param.effects.pulse_orders[ i ].port				= i + 1;
		app.param.effects.pulse_orders[ i ].dir					= 1;
	}
	
	app.param.zonesCount										= ZONES_MAX;
	for( uint8_t i = 0; i < ZONES_MAX; i++ ){
		app.param.zones[ i ].port								= 0;
		app.param.zones[ i ].count								= 0;
		app.param.zones[ i ].start								= 0;
	}
	
	ESP_DEBUG( "INIT...\n" );

	ESP.wdtEnable( 10000 );
	
	pinMode( LED_BUILTIN, OUTPUT );
	pinMode( OUT1_PIN, OUTPUT );
	pinMode( OUT2_PIN, OUTPUT );
	pinMode( OUT3_PIN, OUTPUT );
	pinMode( OUT4_PIN, OUTPUT );

	loadSettings();
	applySettings();

	//Инициализация Wi-Fi
	IPAddress apIP( 10, 10, 10, 10 );
	esp::wifi_init( apIP, IPAddress( 0, 0, 0, 0 ), IPAddress( 255, 255, 255, 0 ) );
	 WiFi.setSleepMode( WIFI_NONE_SLEEP );
	// WiFi.setOutputPower( 17.5 );

	//Инициализация Web сервера
	esp::pageBuff = pageBuff;
	esp::addWebServerPages( &webServer, false, true );
	esp::addWebUpdate( &webServer, DEVICE_NAME );

	webServer.on( "/", indexPageHeadler );
	webServer.on( USER_SETTINGS_FILE, [](){
		esp::webSendFile( &webServer, USER_SETTINGS_FILE, "application/octet-stream" );
	} );
	if( esp::getMode() != esp::Mode::AP ) webServer.onNotFound( indexPageHeadler );
	// webServer.on( "/get", getPageHeadler );
	// webServer.on( "/set", setPageHeadler );
	webServer.begin();

	webSocket.begin();
	webSocket.onEvent( webSocketEvent );

	animationStart();
	ESP_DEBUG( "INIT OK\n" );
}

//-----------------------------------------------------------------------------------------
void loop()
{
	ESP.wdtFeed();

	static uint8_t counter = 0;
	
	//Условие срабатывания по таймеру0 (1сек.)
	if( app.flags.timer0 ){
		app.flags.timer0 = 0;

		//Меняем состояние светодиода для индикации корректности работы
		digitalWrite( LED_BUILTIN, !digitalRead( LED_BUILTIN ) );
// #ifdef __DEV
// 		ESP_DEBUG( "FREE: %u / 81920\n", ESP.getFreeHeap() );
// #endif
	}

	//Условие срабатывания по таймеру1
	if( app.flags.timer1 ){
		app.flags.timer1 = 0;

		if( !esp::isWiFiConnection() ){
			if( counter++ >= 40 ){
				digitalWrite( LED_BUILTIN, !digitalRead( LED_BUILTIN ) );
				counter = 0;
			}
		}

		animationProcess();
	}

	

	webSocket.loop();
	webServer.handleClient();
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
	uint8_t* pointer = (uint8_t*)&app.param;
	esp::saveSettings( pointer, sizeof( app.param ) );
}

//-----------------------------------------------------------------------------------------
void loadSettings(void)
{
	ESP_DEBUG( "Load settings\n" );
	uint32_t len = sizeof( app.param );
	uint8_t* pointer = (uint8_t*)&app.param;
	esp::loadSettings( pointer, len );

	ESP_DEBUG( ">: [%u bytes] ", len );
	esp::printHexData( pointer, len );
	ESP_DEBUG( "\n" );
}

//-----------------------------------------------------------------------------------------
