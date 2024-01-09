#include "functions.h"


//----------- VARIABLES --------------------------------------------------------------------

//----------- FUNCTIONS--------------------------------------------------------------------
void startMode0(void)
{
	if( app.param.use.port1 ){
		app.port1[ 0 ] = CRGB( random( 0, 255 ), random( 0, 255 ), random( 0, 255 ) );
	}
	if( app.param.use.port2 ){
		app.port2[ 0 ] = CRGB( random( 0, 255 ), random( 0, 255 ), random( 0, 255 ) );
	}
	if( app.param.use.port3 ){
		app.port3[ 0 ] = CRGB( random( 0, 255 ), random( 0, 255 ), random( 0, 255 ) );
	}
	if( app.param.use.port4 ){
		app.port4[ 0 ] = CRGB( random( 0, 255 ), random( 0, 255 ), random( 0, 255 ) );
	}
	if( app.param.use.port5 ){
		app.port5[ 0 ] = CRGB( random( 0, 255 ), random( 0, 255 ), random( 0, 255 ) );
	}

	m_animationCounterMax = app.param.effects.pulse_speed;
	m_animationCounter = m_animationCounterMax;
}

//-----------------------------------------------------------------------------------------
void startMode1(void)
{
	if( app.param.use.port1 ) app.port1[ 0 ] = CRGB( 255, 0, 0 );
	if( app.param.use.port2 ) app.port2[ 0 ] = CRGB( 255, 0, 0 );
	if( app.param.use.port3 ) app.port3[ 0 ] = CRGB( 255, 0, 0 );
	if( app.param.use.port4 ) app.port4[ 0 ] = CRGB( 255, 0, 0 );
	if( app.param.use.port5 ) app.port5[ 0 ] = CRGB( 255, 0, 0 );

	m_animationCounterMax = app.param.effects.rainbow_speed;
	m_animationCounter = m_animationCounterMax;
}

//-----------------------------------------------------------------------------------------
void startMode2(void)
{
	m_animationCounterMax = app.param.effects.fire_speed;
	m_animationCounter = m_animationCounterMax;
}

//-----------------------------------------------------------------------------------------
void startMode3(void)
{
	FastLED.showColor( CRGB( app.param.effects.masterColor.r, app.param.effects.masterColor.g, app.param.effects.masterColor.b ) );
}

//-----------------------------------------------------------------------------------------
void animationStart(void)
{
	FastLED.clear();
	FastLED.showColor( CRGB( 0, 0, 0 ) );

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
		case 1:		startMode1();			break;	//Rianbow
		case 2:		startMode2();			break;	//Fire
		case 3:		startMode3();			break;	//One color
		case 0:
		default:	startMode0();			break;
	}

	FastLED.show();
}

//-----------------------------------------------------------------------------------------
void animationProcess(void)
{
	switch( app.param.mode ){
		case 1:		rainbow();				break;
		case 2:		fire();					break;
		case 0:
		default:	pulse();				break;
	}
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
		case WStype_DISCONNECTED: // enum that read status this is used for debugging.
			ESP_DEBUG( "DISCONNECTED\n" );
			animationStart();
		break;
		case WStype_CONNECTED:  // Check if a WebSocket client is connected or not
			FastLED.showColor( CRGB( 37, 173, 0 ) );
			ESP_DEBUG( "CONNECTED\n", type );
		break;
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
								break;
								case 0x01:		// use ports	(1 byte)
									if( length >= 3 ){
										uint8_t* pointer = (uint8_t*)&app.param.use;
										pointer[ 0 ] = payload[ 2 ];
										// app.param.use.port1 = CheckBit( payload[ 2 ], 0 );
										// app.param.use.port2 = CheckBit( payload[ 2 ], 1 );
										// app.param.use.port3 = CheckBit( payload[ 2 ], 2 );
										// app.param.use.port4 = CheckBit( payload[ 2 ], 3 );
										// app.param.use.port5 = CheckBit( payload[ 2 ], 4 );
										// app.param.use.port6 = CheckBit( payload[ 2 ], 5 );
										// app.param.use.port7 = CheckBit( payload[ 2 ], 6 );
										// app.param.use.port8 = CheckBit( payload[ 2 ], 7 );
									}
								break;
								case 0x02:		// use outs		(1 byte)
									if( length >= 3 ){
										uint8_t* pointer = (uint8_t*)&app.param.use;
										pointer[ 1 ] = payload[ 2 ];
										// app.param.use.out1 = CheckBit( payload[ 2 ], 0 );
										// app.param.use.out2 = CheckBit( payload[ 2 ], 1 );
										// app.param.use.out3 = CheckBit( payload[ 2 ], 2 );
										// app.param.use.out4 = CheckBit( payload[ 2 ], 3 );
										// app.param.use.out5 = CheckBit( payload[ 2 ], 4 );
										// app.param.use.out6 = CheckBit( payload[ 2 ], 5 );
										// app.param.use.out7 = CheckBit( payload[ 2 ], 6 );
										// app.param.use.out8 = CheckBit( payload[ 2 ], 7 );
									}
								break;
								case 0x03:		// outs state	(1 byte)
								break;
								case 0x04:		// action		(1 byte)
								if( length >= 3 ){
									// Action ID
									switch( payload[ 2 ] ){
										case 0x00:		// reboot
											ESP.reset();
										break;
										case 0x01:		// saveReboot
											saveSettings();
											delay( 730 );
											ESP.reset();
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
									}
								}
								break;
								case 0x06:		// effect param	(1-2 byte)
								if( length >= 5 ){
									WS::Packet_effect* effect = ( WS::Packet_effect* ) payload;
									uint16_t value = ( effect->data[ 0 ] << 8 ) | ( effect->data[ 1 ] );
									if( effect->effectID == 0x00 ){
										app.param.effects.rainbow_speed = value;
									}else if( effect->effectID == 0x01 ){
										app.param.effects.rainbow_step = value;
									}else if( effect->effectID == 0x02 ){
										app.param.effects.fire_speed = value;
									}else if( effect->effectID == 0x03 ){
										app.param.effects.fire_step = value;
									}else if( effect->effectID == 0x04 ){
										app.param.effects.pulse_speed = value;
									}else if( effect->effectID == 0x05 ){
										app.param.effects.pulse_step = value;
									}
								}
								break;
								case 0x07:		// port count	(3 byte)
								if( length >= 5 ){
									WS::Packet_portSize* port = ( WS::Packet_portSize* ) payload;
									uint16_t size = ( payload[ 3 ] << 8 ) | ( payload[ 4 ] );
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
								}
								break;
							}
						}
					break;
					default: break;
				}
			}
			
			// 	// ESP_DEBUG( "[%04X][%04X][%04X]\n", payload[ 0 ], payload[ 1 ], payload[ 2 ] );
			// 	if( payload[ 0 ] == 0xF1 ){
			// 		app.param.mode = ( payload[ 1 ] == 1 ) ? 1 : 0;
			// 		saveSettings();
			// 	}
			// }else{
			// 	// Check data at file
			// 	ESP_DEBUG( "DATA LEN [%u]\n", length );
			// 	printRawData( payload, length );

			// 	if( payload[ 0 ] == 0xA1 && payload[ 1 ] == 0x1A ){
			// 		// Обнуляем индекс сырых данных
			// 		app.uploadDataIndx = 0;

			// 		// Копируем данные
			// 		if( ( app.uploadDataIndx + length ) < RAW_DATA_BUFF_SIZE ){
			// 			uint8_t* ptr = app.uploadData + app.uploadDataIndx;
			// 			memcpy( ptr, payload, length );
			// 			app.uploadDataIndx += length;
			// 		}

			// 		// Checking new file...
			// 		LOGO_Config* config = (LOGO_Config*)app.uploadData;

			// 		if( config->key[ 0 ] == 0xA1 && config->key[ 1 ] == 0x1A ){
			// 			ESP_DEBUG( "Key successed!\n" );

			// 			uint16_t length = ( config->length[ 0 ] << 8 ) | ( config->length[ 1 ] );
			// 			uint16_t crc = config->inputsNum + config->outputsNum + config->functionsNum + length;

			// 			if( app.uploadDataIndx == ( length + sizeof( LOGO_Config ) + sizeof( crc ) ) ){
			// 				ESP_DEBUG( "Checking data length successed!\n" );

			// 				//Computing CRC
			// 				uint16_t crcOffset = app.uploadDataIndx - sizeof( crc );
			// 				for( uint16_t i = sizeof( LOGO_Config ); i < crcOffset; i++ ){
			// 					crc += app.uploadData[ i ];
			// 				}
				
			// 				uint16_t readCRC = ( app.uploadData[ crcOffset ] << 8 ) | ( app.uploadData[ crcOffset + 1 ] );

			// 				if( crc == readCRC ){
			// 					ESP_DEBUG( "Checking CRC successed!\n" );
			// 					// Заменяем файл проекта в памяти
			// 					saveProjectFile();

			// 					// Заменяем сырые данные загруженными
			// 					clearRawData();
			// 					memcpy( app.rawData, app.uploadData, app.uploadDataIndx );

			// 					// Применяем конфиг проекта
			// 					parsingConfig();

			// 					// Отвечаем клиенту
			// 					app.uploadData[ 0 ] = WebSocket_Commands::UPLOAD_RESPONSE;
			// 					app.uploadData[ 1 ] = Upload_Result::OK;
			// 					webSocket.broadcastBIN( app.uploadData, 2 );
			// 				}else{
			// 					ESP_DEBUG( "Checking CRC failed! [%02X][%02X]\n", readCRC, crc );
			// 					app.uploadData[ 0 ] = WebSocket_Commands::UPLOAD_RESPONSE;
			// 					app.uploadData[ 1 ] = Upload_Result::CRC_ERROR;
			// 					webSocket.broadcastBIN( app.uploadData, 2 );
			// 				}
			// 			}else{
			// 				ESP_DEBUG( "Checking data length failed!\n" );
			// 				app.uploadData[ 0 ] = WebSocket_Commands::UPLOAD_RESPONSE;
			// 				app.uploadData[ 1 ] = Upload_Result::LENGTH_ERROR;
			// 				webSocket.broadcastBIN( app.uploadData, 2 );
			// 			}
			// 		}else{
			// 			ESP_DEBUG( "Checking header file failed!\n" );
			// 			app.uploadData[ 0 ] = WebSocket_Commands::UPLOAD_RESPONSE;
			// 			app.uploadData[ 1 ] = Upload_Result::FILE_ERROR;
			// 			webSocket.broadcastBIN( app.uploadData, 2 );
			// 		}
			// 	}
			// }
		break;
	}
}

//-----------------------------------------------------------------------------------------