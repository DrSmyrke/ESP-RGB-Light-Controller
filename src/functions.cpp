#include "functions.h"


//----------- VARIABLES --------------------------------------------------------------------

//----------- FUNCTIONS--------------------------------------------------------------------
void startMode0(void)
{
	app.effects.pulse_current_color += COLOR_STEP;

	for( uint8_t i = 0; i < LENTS_COUNT_MAX; i++ ){
		Order order = app.param.effects.pulse_orders[ i ];
		CRGB* port = getPortPtr( order.port );
		uint16_t size = getPortSize( order.port );
		uint8_t indx = ( order.dir ) ? 1 : size - 2;

		if(  port != nullptr ){
			port[ indx ] = CRGB( app.effects.pulse_current_color, 255, random( 10, 255 ) );
			break;
		}
	}

	m_animationCounterMax = app.param.effects.pulse_anim_delay;
	m_animationCounter = 0;
}

//-----------------------------------------------------------------------------------------
void startMode1(void)
{
	for( uint8_t i = 0; i < LENTS_COUNT_MAX; i++ ){
		Order order = app.param.effects.rainbow_orders[ i ];
		CRGB* port = getPortPtr( order.port );
		uint16_t size = getPortSize( order.port );
		uint8_t indx = ( order.dir ) ? 1 : size - 2;

		if(  port != nullptr ){
			port[ indx ] = CRGB( 255, 0, 0 );
			break;
		}
	}

	m_animationCounterMax = app.param.effects.rainbow_anim_delay;
	m_animationCounter = 0;
}

//-----------------------------------------------------------------------------------------
void startMode2(void)
{
	m_animationCounterMax = app.param.effects.rainbow_anim_delay;
	m_animationCounter = 0;
}

//-----------------------------------------------------------------------------------------
void animationStart(void)
{
	// Clear all
	oneColor( 0, 0, 0 );
	app.orderNum = 0;

	/*
		'Радуга',
		'Огонь',
		'Один цвет',
		'Зонирование',

		'Мигалка',
		'Гонки',
		'',
		'',
		'',
		'',
	*/
	switch( app.param.mode ){
		case 1:		startMode1();										break;	//Rianbow
		case 2:		startMode2();										break;	//Fire
		case 3:		oneColor( app.param.effects.masterColor );			break;	//One color
		case 4:		zonesColor();										break;	//Zones color
		case 0:
		default:	startMode0();										break;
	}

	FastLED.show();
}

//-----------------------------------------------------------------------------------------
void animationProcess(void)
{
	if( m_animationCounter ){
		m_animationCounter--;
		return;
	}

	if( app.orderNum >= LENTS_COUNT_MAX ) app.orderNum = 0;
	m_animationCounterMax = 80;

	switch( app.param.mode ){
		case 0:
			for( uint8_t i = 0; i < app.param.effects.pulse_speed; i++) pulse();
		break;
		case 1:
			for( uint8_t i = 0; i < app.param.effects.rainbow_speed; i++) rainbow();
		break;
		case 2:
			for( uint8_t i = 0; i < app.param.effects.fire_speed; i++) fire();
		break;
	}
	
	m_animationCounter = m_animationCounterMax;
	FastLED.show();
}

//-----------------------------------------------------------------------------------------
CRGB* getPortPtr(const uint8_t portNum)
{
	CRGB* port = nullptr;

	switch( portNum ){
		case 1:	if( app.param.use.port1 ) port = app.port1;	break;
		case 2:	if( app.param.use.port2 ) port = app.port2;	break;
		case 3:	if( app.param.use.port3 ) port = app.port3;	break;
		case 4:	if( app.param.use.port4 ) port = app.port4;	break;
		case 5:	if( app.param.use.port5 ) port = app.port5;	break;
		// case 6:	if( app.param.use.port6 ) port = app.port6;	break;
		// case 7:	if( app.param.use.port7 ) port = app.port7;	break;
		// case 8:	if( app.param.use.port8 ) port = app.port8;	break;
	}

	return port;
}

//-----------------------------------------------------------------------------------------
uint16_t getPortSize(const uint8_t portNum)
{
	uint16_t size = 0;

	switch( portNum ){
		case 1:	if( app.param.use.port1 ) size = app.param.port1_size;	break;
		case 2:	if( app.param.use.port2 ) size = app.param.port2_size;	break;
		case 3:	if( app.param.use.port3 ) size = app.param.port3_size;	break;
		case 4:	if( app.param.use.port4 ) size = app.param.port4_size;	break;
		case 5:	if( app.param.use.port5 ) size = app.param.port5_size;	break;
		// case 6:	if( app.param.use.port6 ) size = app.param.port6_size;	break;
		// case 7:	if( app.param.use.port7 ) size = app.param.port7_size;	break;
		// case 8:	if( app.param.use.port8 ) size = app.param.port8_size;	break;
	}

	return size;
}

//-----------------------------------------------------------------------------------------
// void getPageHeadler(void)
// {
	// strcpy( pageBuff, "{\"mode\": " );
	// strcat( pageBuff, utoa( app.param.mode, esp::tmpVal, 10 ) );

	// uint32_t color = ( ( app.param.masterColor.r << 24 ) | ( app.param.masterColor.g << 16 ) | ( app.param.masterColor.b << 8 ) | 0 );
	// strcat( pageBuff, ",\"masterColor\":" ); strcat( pageBuff, utoa( color, esp::tmpVal, 10 ) );
	// // strcat( pageBuff, ",\"status3\":" ); strcat( pageBuff, utoa( app.status_3, esp::tmpVal, 10 ) );

	// strcat( pageBuff, ",\"outs\": {" );
	// 	strcat( pageBuff, "\"1\": {\"use\":" ); strcat( pageBuff, utoa( app.param.flags.use_out1, esp::tmpVal, 10 ) );
	// 	strcat( pageBuff, ",\"state\":" ); strcat( pageBuff, utoa( app.param.flags.out1_state, esp::tmpVal, 10 ) );
	// 	strcat( pageBuff, "}" );
	// 	strcat( pageBuff, ",\"2\": {\"use\":" ); strcat( pageBuff, utoa( app.param.flags.use_out2, esp::tmpVal, 10 ) );
	// 	strcat( pageBuff, ",\"state\":" ); strcat( pageBuff, utoa( app.param.flags.out2_state, esp::tmpVal, 10 ) );
	// 	strcat( pageBuff, "}" );
	// 	strcat( pageBuff, ",\"3\": {\"use\":" ); strcat( pageBuff, utoa( app.param.flags.use_out3, esp::tmpVal, 10 ) );
	// 	strcat( pageBuff, ",\"state\":" ); strcat( pageBuff, utoa( app.param.flags.out3_state, esp::tmpVal, 10 ) );
	// 	strcat( pageBuff, "}" );
	// 	strcat( pageBuff, ",\"4\": {\"use\":" ); strcat( pageBuff, utoa( app.param.flags.use_out4, esp::tmpVal, 10 ) );
	// 	strcat( pageBuff, ",\"state\":" ); strcat( pageBuff, utoa( app.param.flags.out4_state, esp::tmpVal, 10 ) );
	// 	strcat( pageBuff, "}" );
	// strcat( pageBuff, "},\"ports\": {" );
	// 	strcat( pageBuff, "\"1\": {\"use\":" ); strcat( pageBuff, utoa( app.param.flags.use_port1, esp::tmpVal, 10 ) );
	// 	strcat( pageBuff, ",\"leds\":" ); strcat( pageBuff, utoa( app.param.port1_size, esp::tmpVal, 10 ) );
	// 	strcat( pageBuff, ",\"zones\":[" );
	// 	for( uint8_t i = 0; i < ZONES_AT_PORT; i++ ){
	// 		if( i > 0 ) strcat( pageBuff, "," );
	// 		strcat( pageBuff, "{\"count\":" ); strcat( pageBuff, utoa( app.port1zones[ i ].count, esp::tmpVal, 10 ) );
	// 		strcat( pageBuff, ",\"color\":" ); strcat( pageBuff, utoa( app.port1zones[ i ].color, esp::tmpVal, 10 ) );
	// 		strcat( pageBuff, "}" );
	// 	}
	// 	strcat( pageBuff, "]}" );
	// 	strcat( pageBuff, ",\"2\": {\"use\":" ); strcat( pageBuff, utoa( app.param.flags.use_port2, esp::tmpVal, 10 ) );
	// 	strcat( pageBuff, ",\"leds\":" ); strcat( pageBuff, utoa( app.param.port2_size, esp::tmpVal, 10 ) );
	// 	strcat( pageBuff, ",\"zones\":[" );
	// 	for( uint8_t i = 0; i < ZONES_AT_PORT; i++ ){
	// 		if( i > 0 ) strcat( pageBuff, "," );
	// 		strcat( pageBuff, "{\"count\":" ); strcat( pageBuff, utoa( app.port2zones[ i ].count, esp::tmpVal, 10 ) );
	// 		strcat( pageBuff, ",\"color\":" ); strcat( pageBuff, utoa( app.port2zones[ i ].color, esp::tmpVal, 10 ) );
	// 		strcat( pageBuff, "}" );
	// 	}
	// 	strcat( pageBuff, "]}" );
	// 	strcat( pageBuff, ",\"3\": {\"use\":" ); strcat( pageBuff, utoa( app.param.flags.use_port3, esp::tmpVal, 10 ) );
	// 	strcat( pageBuff, ",\"leds\":" ); strcat( pageBuff, utoa( app.param.port3_size, esp::tmpVal, 10 ) );
	// 	strcat( pageBuff, ",\"zones\":[" );
	// 	for( uint8_t i = 0; i < ZONES_AT_PORT; i++ ){
	// 		if( i > 0 ) strcat( pageBuff, "," );
	// 		strcat( pageBuff, "{\"count\":" ); strcat( pageBuff, utoa( app.port3zones[ i ].count, esp::tmpVal, 10 ) );
	// 		strcat( pageBuff, ",\"color\":" ); strcat( pageBuff, utoa( app.port3zones[ i ].color, esp::tmpVal, 10 ) );
	// 		strcat( pageBuff, "}" );
	// 	}
	// 	strcat( pageBuff, "]}" );
	// 	strcat( pageBuff, ",\"4\": {\"use\":" ); strcat( pageBuff, utoa( app.param.flags.use_port4, esp::tmpVal, 10 ) );
	// 	strcat( pageBuff, ",\"leds\":" ); strcat( pageBuff, utoa( app.param.port4_size, esp::tmpVal, 10 ) );
	// 	strcat( pageBuff, ",\"zones\":[" );
	// 	for( uint8_t i = 0; i < ZONES_AT_PORT; i++ ){
	// 		if( i > 0 ) strcat( pageBuff, "," );
	// 		strcat( pageBuff, "{\"count\":" ); strcat( pageBuff, utoa( app.port4zones[ i ].count, esp::tmpVal, 10 ) );
	// 		strcat( pageBuff, ",\"color\":" ); strcat( pageBuff, utoa( app.port4zones[ i ].color, esp::tmpVal, 10 ) );
	// 		strcat( pageBuff, "}" );
	// 	}
	// 	strcat( pageBuff, "]}" );
	// 	strcat( pageBuff, ",\"5\": {\"use\":" ); strcat( pageBuff, utoa( app.param.flags.use_port5, esp::tmpVal, 10 ) );
	// 	strcat( pageBuff, ",\"leds\":" ); strcat( pageBuff, utoa( app.param.port5_size, esp::tmpVal, 10 ) );
	// 	strcat( pageBuff, ",\"zones\":[" );
	// 	for( uint8_t i = 0; i < ZONES_AT_PORT; i++ ){
	// 		if( i > 0 ) strcat( pageBuff, "," );
	// 		strcat( pageBuff, "{\"count\":" ); strcat( pageBuff, utoa( app.port5zones[ i ].count, esp::tmpVal, 10 ) );
	// 		strcat( pageBuff, ",\"color\":" ); strcat( pageBuff, utoa( app.port5zones[ i ].color, esp::tmpVal, 10 ) );
	// 		strcat( pageBuff, "}" );
	// 	}
	// 	strcat( pageBuff, "]}" );
	// strcat( pageBuff, "}}" );
// 	strcat( pageBuff, "{}" );
// 	webServer.send ( 200, "application/json", pageBuff );
// }

//-----------------------------------------------------------------------------------------
// void setPageHeadler(void)
// {
	/*
	strcpy( pageBuff, "{\"success\": " );

	bool success = false;

#if defined( CONTROL_LOGIN ) && defined( CONTROL_PASSWORD )
	if( esp::checkWebAuth( &webServer, "login", "password", ESP_AUTH_REALM, "access denied" ) ){
#endif
		if( webServer.hasArg( "param" ) && webServer.hasArg( "value" ) ){
			const String &param = webServer.arg( "param" );
			const String &value = webServer.arg( "value" );
		
			if( param == "port1use" ){
				app.param.flags.use_port1 = ( value.toInt() == 1 ) ? 1 : 0; success = true;
			}else if( param == "port2use" ){
				app.param.flags.use_port2 = ( value.toInt() == 1 ) ? 1 : 0; success = true;
			}else if( param == "port3use" ){
				app.param.flags.use_port3 = ( value.toInt() == 1 ) ? 1 : 0; success = true;
			}else if( param == "port4use" ){
				app.param.flags.use_port4 = ( value.toInt() == 1 ) ? 1 : 0; success = true;
			}else if( param == "port5use" ){
				app.param.flags.use_port5 = ( value.toInt() == 1 ) ? 1 : 0; success = true;
			}else if( param == "port1leds" ){
				app.param.port1_size = (uint16_t)value.toInt(); success = true;
			}else if( param == "port2leds" ){
				app.param.port2_size = (uint16_t)value.toInt(); success = true;
			}else if( param == "port3leds" ){
				app.param.port3_size = (uint16_t)value.toInt(); success = true;
			}else if( param == "port4leds" ){
				app.param.port4_size = (uint16_t)value.toInt(); success = true;
			}else if( param == "port5leds" ){
				app.param.port5_size = (uint16_t)value.toInt(); success = true;
			}else if( param == "out1use" ){
				app.param.flags.use_out1 = ( value.toInt() == 1 ) ? 1 : 0; success = true; resetOuts();
			}else if( param == "out2use" ){
				app.param.flags.use_out2 = ( value.toInt() == 1 ) ? 1 : 0; success = true; resetOuts();
			}else if( param == "out3use" ){
				app.param.flags.use_out3 = ( value.toInt() == 1 ) ? 1 : 0; success = true; resetOuts();
			}else if( param == "out4use" ){
				app.param.flags.use_out4 = ( value.toInt() == 1 ) ? 1 : 0; success = true; resetOuts();
			}else if( param == "out1state" ){
				app.param.flags.out1_state = ( value.toInt() == 1 ) ? 1 : 0; success = true; resetOuts();
			}else if( param == "out2state" ){
				app.param.flags.out2_state = ( value.toInt() == 1 ) ? 1 : 0; success = true; resetOuts();
			}else if( param == "out3state" ){
				app.param.flags.out3_state = ( value.toInt() == 1 ) ? 1 : 0; success = true; resetOuts();
			}else if( param == "out4state" ){
				app.param.flags.out4_state = ( value.toInt() == 1 ) ? 1 : 0; success = true; resetOuts();
			}else if( param == "zoneCount" ){
				uint8_t port = (uint8_t)getValue( value, ':', 0 ).toInt();
				uint8_t zone = (uint8_t)getValue( value, ':', 1 ).toInt();
				uint16_t count = (uint16_t)getValue( value, ':', 2 ).toInt();

				if( zone < ZONES_AT_PORT ){
					if( port == 1 ){
						app.port1zones[ zone ].count = count;
					}else if( port == 2 ){
						app.port2zones[ zone ].count = count;
					}else if( port == 3 ){
						app.port3zones[ zone ].count = count;
					}else if( port == 4 ){
						app.port4zones[ zone ].count = count;
					}else if( port == 5 ){
						app.port5zones[ zone ].count = count;
					}
					success = true;
					animationStart();
				}
			}else if( param == "zoneColor" ){
				uint8_t port = (uint8_t)getValue( value, ':', 0 ).toInt();
				uint8_t zone = (uint8_t)getValue( value, ':', 1 ).toInt();
				String val = getValue( value, ':', 2 );
				uint32_t color = (uint32_t)getValue( value, ':', 2 ).toInt();

				if( zone < ZONES_AT_PORT ){
					if( port == 1 ){
						app.port1zones[ zone ].color = color;
					}else if( port == 2 ){
						app.port2zones[ zone ].color = color;
					}else if( port == 3 ){
						app.port3zones[ zone ].color = color;
					}else if( port == 4 ){
						app.port4zones[ zone ].color = color;
					}else if( port == 5 ){
						app.port5zones[ zone ].color = color;
					}
					success = true;
					animationStart();
				}
			}else if( param == "masterColor" ){
				app.param.masterColor = (uint32_t)value.toInt();
				success = true;
				animationStart();
			}else if( param == "mode" ){
				app.param.mode = (uint8_t)value.toInt();
				success = true;
				animationStart();
			}else if( param == "action" ){
				if( value == "saveReboot" ){
					webServer.send ( 200, "application/json", "{ \"success\": \"Rebooting...\" }" );
					saveSettings();
					delay( 100 );
					ESP.reset();
					return;
				}else if( value == "save" ){
					saveSettings();
					delay( 100 );
					success = true;
				}
			}
		}
#if defined( CONTROL_LOGIN ) && defined( CONTROL_PASSWORD )
	}
#endif



	if( success ){
		strcat( pageBuff, "\"OK\"}" );
	}else{
		strcat( pageBuff, "\"ERROR\"}" );
	}
	*/
// 	strcat( pageBuff, "{}" );
	
// 	webServer.send ( 200, "application/json", pageBuff );
// }

//-----------------------------------------------------------------------------------------
void indexPageHeadler(void)
{
	esp::webSendFile( &webServer, "/index.html", "text/html" );
}

//-----------------------------------------------------------------------------------------
String getValue(String data, char separator, int index)
{
	int found = 0;
	int strIndex[] = {0, -1};
	int maxIndex = data.length()-1;

	for(int i=0; i<=maxIndex && found<=index; i++){
		if(data.charAt(i)==separator || i==maxIndex){
			found++;
			strIndex[0] = strIndex[1]+1;
			strIndex[1] = (i == maxIndex) ? i+1 : i;
		}
	}

	return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

//-----------------------------------------------------------------------------------------
void resetOuts(void)
{
	if( app.param.use.out1 ) digitalWrite( OUT1_PIN, app.param.outsState.out1 );
	if( app.param.use.out2 ) digitalWrite( OUT2_PIN, app.param.outsState.out2 );
	if( app.param.use.out3 ) digitalWrite( OUT3_PIN, app.param.outsState.out3 );
	if( app.param.use.out4 ) digitalWrite( OUT4_PIN, app.param.outsState.out4 );
#ifdef OUT5_PIN
	if( app.param.use.out5 ) digitalWrite( OUT5_PIN, app.param.outsState.out5 );
#endif
#ifdef OUT6_PIN
	if( app.param.use.out6 ) digitalWrite( OUT6_PIN, app.param.outsState.out6 );
#endif
#ifdef OUT7_PIN
	if( app.param.use.out7 ) digitalWrite( OUT7_PIN, app.param.outsState.out7 );
#endif
#ifdef OUT8_PIN
	if( app.param.use.out8 ) digitalWrite( OUT8_PIN, app.param.outsState.out8 );
#endif
}

//-----------------------------------------------------------------------------------------
void applySettings(void)
{
	if( app.param.effects.rainbow_anim_delay == 0 ) app.param.effects.rainbow_anim_delay = EFFECT_RAINBOW_ANIM_DELAY;
	if( app.param.effects.rainbow_speed == 0 ) app.param.effects.rainbow_speed = EFFECT_RAINBOW_SPEED;
	if( app.param.effects.rainbow_step == 0 ) app.param.effects.rainbow_step = EFFECT_RAINBOW_STEP;
	if( app.param.effects.fire_anim_delay == 0 ) app.param.effects.fire_anim_delay = EFFECT_FIRE_ANIM_DELAY;
	if( app.param.effects.fire_speed == 0 ) app.param.effects.fire_speed = EFFECT_FIRE_SPEED;
	if( app.param.effects.fire_step == 0 ) app.param.effects.fire_step = EFFECT_FIRE_STEP;
	if( app.param.effects.fire_hue_gap == 0 ) app.param.effects.fire_hue_gap = EFFECT_FIRE_HUE_GAP;
	// if( app.param.effects.fire_hue_start == 0 ) app.param.effects.fire_hue_start = EFFECT_FIRE_HUE_START;
	if( app.param.effects.fire_min_bright == 0 ) app.param.effects.fire_min_bright = EFFECT_FIRE_MIN_BRIGHT;
	if( app.param.effects.fire_max_bright == 0 ) app.param.effects.fire_max_bright = EFFECT_FIRE_MAX_BRIGHT;
	if( app.param.effects.fire_min_sat == 0 ) app.param.effects.fire_min_sat = EFFECT_FIRE_MIN_SAT;
	if( app.param.effects.fire_max_sat == 0 ) app.param.effects.fire_max_sat = EFFECT_FIRE_MAX_SAT;
	if( app.param.effects.pulse_anim_delay == 0 ) app.param.effects.pulse_anim_delay = EFFECT_PULSE_ANIM_DELAY;
	if( app.param.effects.pulse_speed == 0 ) app.param.effects.pulse_speed = EFFECT_PULSE_SPEED;
	if( app.param.effects.pulse_step == 0 ) app.param.effects.pulse_step = EFFECT_PULSE_STEP;

	if( app.param.port1_size > LENTA_COUNT_MAX ) app.param.port1_size = LENTA_COUNT_MAX;
	if( app.param.port2_size > LENTA_COUNT_MAX ) app.param.port2_size = LENTA_COUNT_MAX;
	if( app.param.port3_size > LENTA_COUNT_MAX ) app.param.port3_size = LENTA_COUNT_MAX;
	if( app.param.port4_size > LENTA_COUNT_MAX ) app.param.port4_size = LENTA_COUNT_MAX;
	if( app.param.port5_size > LENTA_COUNT_MAX ) app.param.port5_size = LENTA_COUNT_MAX;
	if( app.param.port6_size > LENTA_COUNT_MAX ) app.param.port6_size = LENTA_COUNT_MAX;
	if( app.param.port7_size > LENTA_COUNT_MAX ) app.param.port7_size = LENTA_COUNT_MAX;
	if( app.param.port8_size > LENTA_COUNT_MAX ) app.param.port8_size = LENTA_COUNT_MAX;

#ifdef LENTA1_PIN
	if( app.port1 != nullptr ) free( app.port1 );
	app.port1 = (CRGB*)malloc( app.param.port1_size * sizeof( CRGB ) );
	FastLED.addLeds<NEOPIXEL, LENTA1_PIN>( app.port1, app.param.port1_size );
#endif
#ifdef LENTA2_PIN
	if( app.port2 != nullptr ) free( app.port2 );
	app.port2 = (CRGB*)malloc( app.param.port2_size * sizeof( CRGB ) );
	FastLED.addLeds<NEOPIXEL, LENTA2_PIN>( app.port2, app.param.port2_size );
#endif
#ifdef LENTA3_PIN
	if( app.port3 != nullptr ) free( app.port3 );
	app.port3 = (CRGB*)malloc( app.param.port3_size * sizeof( CRGB ) );
	FastLED.addLeds<NEOPIXEL, LENTA3_PIN>( app.port3, app.param.port3_size );
#endif
#ifdef LENTA4_PIN
	if( app.port4 != nullptr ) free( app.port4 );
	app.port4 = (CRGB*)malloc( app.param.port4_size * sizeof( CRGB ) );
	FastLED.addLeds<NEOPIXEL, LENTA4_PIN>( app.port4, app.param.port4_size );
#endif
#ifdef LENTA5_PIN
	if( app.port5 != nullptr ) free( app.port5 );
	app.port5 = (CRGB*)malloc( app.param.port5_size * sizeof( CRGB ) );
	FastLED.addLeds<NEOPIXEL, LENTA5_PIN>( app.port5, app.param.port5_size );
#endif
	
	FastLED.clear();
	resetOuts();
}

//-----------------------------------------------------------------------------------------
void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length)
{
	ESP_DEBUG( "WS Type %u: \n", type );

	switch( type ){
		// case WStype_DISCONNECTED: // enum that read status this is used for debugging.
			// ESP_DEBUG( "DISCONNECTED\n" );
			// animationStart();
		// break;
		// case WStype_CONNECTED:  // Check if a WebSocket client is connected or not
			// FastLED.showColor( CRGB( 37, 173, 0 ) );
			// ESP_DEBUG( "CONNECTED\n" );
		// break;
		case WStype_BIN:
			ESP_DEBUG( "WS RCV: [%u bytes] [", length );
			for( uint16_t i = 0; i < length; i++ ) ESP_DEBUG( "%02X ", payload[ i ] );
			ESP_DEBUG( "]\n" );

			if( length >= 1 ){
				WS::Packet* packet = ( WS::Packet* ) payload;
				switch( packet->header.cmd ){
					case 0xF1:		// get configuration
						{
						uint8_t* pointer = (uint8_t*)&app.param;
						webSocket.broadcastBIN( pointer, sizeof( app.param ) );
						}
					break;
					case 0xF2:		// set parametr
						if( length >= 2 ){
							// param ID
							switch( payload[ 1 ] ){
								case 0x00:		// device mode	(1 byte)
									app.param.mode = payload[ 2 ];
									animationStart();
									saveSettings();
								break;
								case 0x01:		// use ports	(1 byte)
									if( length >= 3 ){
										uint8_t* pointer = (uint8_t*)&app.param.use;
										pointer[ 0 ] = payload[ 2 ];
										applySettings();
										animationStart();
									}
								break;
								case 0x02:		// use outs		(1 byte)
									if( length >= 3 ){
										uint8_t* pointer = (uint8_t*)&app.param.use;
										pointer[ 1 ] = payload[ 2 ];
									}
								break;
								case 0x03:		// outs state	(1 byte)
								break;
								case 0x04:		// action		(1 byte)
								if( length >= 3 ){
									// Action ID
									switch( payload[ 2 ] ){
										case 0x00:		// reboot
											// ESP.reset();
											ESP.restart();
										break;
										case 0x01:		// saveReboot
											saveSettings();
											delay( 730 );
											ESP.restart();
										break;
										case 0x02:		// save
											saveSettings();
										break;
									}
								}
								break;
								case 0x05:		// zone params	(6 byte)
								if( length == 8 ){
									WS::Packet_zone* zone = ( WS::Packet_zone* ) payload;
									if( zone->zoneNum < ZONES_MAX ){
										app.param.zones[ zone->zoneNum ].port = zone->port;
										app.param.zones[ zone->zoneNum ].start = ( zone->start[ 0 ] << 8 ) | zone->start[ 1 ];
										app.param.zones[ zone->zoneNum ].count = ( zone->count[ 0 ] << 8 ) | zone->count[ 1 ];
										if( app.param.mode == 4 ) animationStart();
									}
								}
								break;
								case 0x06:		// effect param
								if( length >= 5 ){
									WS::Packet_effect* effect = ( WS::Packet_effect* ) payload;
									if( length == 5 ){
										uint16_t value = effect->data[ 0 ];
										switch( effect->effectID ){
											case 0x00:	app.param.effects.rainbow_anim_delay = value;		break;
											case 0x01:	app.param.effects.rainbow_speed = value;			break;
											case 0x02:	app.param.effects.rainbow_step = value;				break;

											case 0x04:	app.param.effects.fire_anim_delay = value;			break;
											case 0x05:	app.param.effects.fire_speed = value;				break;
											case 0x06:	app.param.effects.fire_step = value;				break;
											case 0x07:	app.param.effects.fire_hue_gap = value;				break;
											case 0x08:	app.param.effects.fire_hue_start = value;			break;
											case 0x09:	app.param.effects.fire_min_bright = value;			break;
											case 0x0A:	app.param.effects.fire_max_bright = value;			break;
											case 0x0B:	app.param.effects.fire_min_sat = value;				break;
											case 0x0C:	app.param.effects.fire_max_sat = value;				break;
											case 0x0D:	app.param.effects.pulse_anim_delay = value;			break;
											case 0x0E:	app.param.effects.pulse_speed = value;				break;
											case 0x0F:	app.param.effects.pulse_step = value;				break;
										}
									}else if( length == 6 ){
										// uint16_t value = ( effect->data[ 0 ] << 8 ) | ( effect->data[ 1 ] );
									}else if( length == 7 ){
										if( effect->effectID == 0x11 ){
											app.param.effects.masterColor.r = effect->data[ 0 ];
											app.param.effects.masterColor.g = effect->data[ 1 ];
											app.param.effects.masterColor.b = effect->data[ 2 ];
										}
									}else if( length == 8 ){
										if( effect->effectID == 0x12 ){
											uint8_t zoneID = effect->data[ 0 ];
											if( zoneID < ZONES_MAX ){
												app.param.effects.zonesColor[ zoneID ].r = effect->data[ 1 ];
												app.param.effects.zonesColor[ zoneID ].g = effect->data[ 2 ];
												app.param.effects.zonesColor[ zoneID ].b = effect->data[ 3 ];
											}
										}
									}else if( length == 20 ){
										if( effect->effectID == 0x03 ){
											uint8_t* ptr = (uint8_t*)app.param.effects.rainbow_orders;
											memcpy( ptr, effect->data, sizeof( app.param.effects.rainbow_orders ) );
										}else if( effect->effectID == 0x10 ){
											uint8_t* ptr = (uint8_t*)app.param.effects.pulse_orders;
											memcpy( ptr, effect->data, sizeof( app.param.effects.pulse_orders ) );
										}
									}
									animationStart();
								}
								break;
								case 0x07:		// port count	(3 byte)
								if( length >= 5 ){
									WS::Packet_portSize* port = ( WS::Packet_portSize* ) payload;
									uint16_t size = ( payload[ 3 ] << 8 ) | ( payload[ 4 ] );
									if( size > LENTA_COUNT_MAX ) size = LENTA_COUNT_MAX;
									switch( port->num ){
										case 1:		app.param.port1_size = size;	break;
										case 2:		app.param.port2_size = size;	break;
										case 3:		app.param.port3_size = size;	break;
										case 4:		app.param.port4_size = size;	break;
										case 5:		app.param.port5_size = size;	break;
										case 6:		app.param.port6_size = size;	break;
										case 7:		app.param.port7_size = size;	break;
										case 8:		app.param.port8_size = size;	break;
									}
									if( port->num >= 1 && port->num <= 8 ){
										applySettings();
										animationStart();
									}
								}
								break;
							}
						}
					break;
					case 0xF3:		// get memory status
						{
							uint32_t free = ESP.getFreeHeap();
							uint32_t total = 81920;
							uint8_t data[ 9 ];
							data[ 0 ] = 0xA2;
							data[ 1 ] = free >> 24;
							data[ 2 ] = free >> 16;
							data[ 3 ] = free >> 8;
							data[ 4 ] = free;
							data[ 5 ] = total >> 24;
							data[ 6 ] = total >> 16;
							data[ 7 ] = total >> 8;
							data[ 8 ] = total;
							webSocket.broadcastBIN( data, sizeof( data ) );
						}
					break;
					default: break;
				}
			}
		break;
	}
}

//-----------------------------------------------------------------------------------------
void moveColorAtNextPort(uint8_t oIndx, const struct Order* ordersList, const CRGB* color, const uint16_t offset, const uint8_t repeat)
{
	oIndx++;

	if( oIndx >= LENTS_COUNT_MAX ){
		if( repeat ){
			oIndx = 0;
		}else{
			return;
		}
	}

	const Order* nextOrder = &ordersList[ oIndx ];
	if( nextOrder->port == 0 ){
		moveColorAtNextPort( oIndx, ordersList, color, offset, repeat );
		return;
	}

	CRGB* nextPort = getPortPtr( nextOrder->port );
	uint16_t nextSize = getPortSize( nextOrder->port );
	if(  nextPort != nullptr ){
		uint16_t indx = ( nextOrder->dir ) ? offset : ( nextSize - 1 - offset );
		if( indx > nextSize ) indx = nextSize - 1;
		nextPort[ indx ] = *color;
	}else{
		moveColorAtNextPort( oIndx, ordersList, color, offset, repeat );
	}
}

//-----------------------------------------------------------------------------------------