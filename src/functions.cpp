#include "functions.h"


//----------- VARIABLES --------------------------------------------------------------------

//----------- FUNCTIONS--------------------------------------------------------------------
void lenta_clear(Adafruit_NeoPixel &lenta)
{
	lenta.clear(); lenta.show();
}

//-----------------------------------------------------------------------------------------
void setLed(Adafruit_NeoPixel &lenta, const uint16_t ledNum, uint8_t r, uint8_t g, uint8_t b, const bool show)
{
	lenta.setPixelColor( ledNum, lenta.Color( r, g, b ) );
	if( show ) lenta.show();
}

//-----------------------------------------------------------------------------------------
void setLed(Adafruit_NeoPixel &lenta, const uint16_t ledNum, uint32_t pixel, const bool show)
{
	lenta.setPixelColor( ledNum, pixel );
	if( show ) lenta.show();
}

//-----------------------------------------------------------------------------------------
void setBrightnessPrz(Adafruit_NeoPixel &lenta, const uint8_t percent)
{
	setBrightness( lenta, map( percent, 0, 100, 0, 255 ) );
}

//-----------------------------------------------------------------------------------------
void setBrightness(Adafruit_NeoPixel &lenta, const uint8_t value)
{
	lenta.setBrightness( value );
	lenta.show();
}

//-----------------------------------------------------------------------------------------
void startMode0(void)
{
	rainbow[ 0 ]					= { 0xFF, 0, 0 };
	rainbow[ 1 ]					= { 0xFF, 0x35, 0 };
	rainbow[ 2 ]					= { 0xFF, 0xFF, 0 };
	rainbow[ 3 ]					= { 0, 0x80, 0 };
	rainbow[ 4 ]					= { 0, 0, 0xFF };
	rainbow[ 5 ]					= { 0x4B, 0, 0x82 };
	rainbow[ 6 ]					= { 0xEE, 0x82, 0xEE };

	if( app.flags.use_port1 ){
		setBrightnessPrz( port1, 45 );
		uint8_t ri = 0;
		for( uint16_t i = 0; i <= port1.numPixels(); i++ ){
			if( ri >= 6 ) ri = 0;
			setLed( port1, i, rainbow[ ri ].r, rainbow[ ri ].g, rainbow[ ri ].b, true ); delay( 100 );
			ri++;
		}
		delay( 250 );
	}
	if( app.flags.use_port2 ){
		setBrightnessPrz( port2, 45 );
		uint8_t ri = 0;
		for( uint16_t i = 0; i <= port2.numPixels(); i++ ){
			if( ri >= 6 ) ri = 0;
			setLed( port2, i, rainbow[ ri ].r, rainbow[ ri ].g, rainbow[ ri ].b, true ); delay( 100 );
			ri++;
		}
		delay( 250 );
	}
	if( app.flags.use_port3 ){
		setBrightnessPrz( port3, 45 );
		uint8_t ri = 0;
		for( uint16_t i = 0; i <= port3.numPixels(); i++ ){
			if( ri >= 6 ) ri = 0;
			setLed( port3, i, rainbow[ ri ].r, rainbow[ ri ].g, rainbow[ ri ].b, true ); delay( 100 );
			ri++;
		}
		delay( 250 );
	}
	if( app.flags.use_port4 ){
		setBrightnessPrz( port4, 45 );
		uint8_t ri = 0;
		for( uint16_t i = 0; i <= port4.numPixels(); i++ ){
			if( ri >= 6 ) ri = 0;
			setLed( port4, i, rainbow[ ri ].r, rainbow[ ri ].g, rainbow[ ri ].b, true ); delay( 100 );
			ri++;
		}
		delay( 250 );
	}
	if( app.flags.use_port5 ){
		setBrightnessPrz( port5, 45 );
		uint8_t ri = 0;
		for( uint16_t i = 0; i <= port5.numPixels(); i++ ){
			if( ri >= 6 ) ri = 0;
			setLed( port5, i, rainbow[ ri ].r, rainbow[ ri ].g, rainbow[ ri ].b, true ); delay( 100 );
			ri++;
		}
		delay( 250 );
	}
}

//-----------------------------------------------------------------------------------------
void startMode1(void)
{
	clearAll();

	if( app.flags.use_port1 ){
		uint16_t led = 0;
		for( uint8_t i = 0; i < ZONES_AT_PORT; i++ ){
			uint16_t count = app.port1zones[ i ].count;
			if( count == 0 ) continue;
			while( count-- ){
				uint8_t r = app.port1zones[ i ].color >> 24;
				uint8_t g = app.port1zones[ i ].color >> 16;
				uint8_t b = app.port1zones[ i ].color >> 8;
				setLed( port1, led++, r, g, b );
			}
		}
		port1.show();
	}
	if( app.flags.use_port2 ){
		uint16_t led = 0;
		for( uint8_t i = 0; i < ZONES_AT_PORT; i++ ){
			uint16_t count = app.port2zones[ i ].count;
			if( count == 0 ) continue;
			while( count-- ){
				uint8_t r = app.port2zones[ i ].color >> 24;
				uint8_t g = app.port2zones[ i ].color >> 16;
				uint8_t b = app.port2zones[ i ].color >> 8;
				setLed( port2, led++, r, g, b );
			}
		}
		port2.show();
	}
	if( app.flags.use_port3 ){
		uint16_t led = 0;
		for( uint8_t i = 0; i < ZONES_AT_PORT; i++ ){
			uint16_t count = app.port3zones[ i ].count;
			if( count == 0 ) continue;
			while( count-- ){
				uint8_t r = app.port3zones[ i ].color >> 24;
				uint8_t g = app.port3zones[ i ].color >> 16;
				uint8_t b = app.port3zones[ i ].color >> 8;
				setLed( port3, led++, r, g, b );
			}
		}
		port3.show();
	}
	if( app.flags.use_port4 ){
		uint16_t led = 0;
		for( uint8_t i = 0; i < ZONES_AT_PORT; i++ ){
			uint16_t count = app.port4zones[ i ].count;
			if( count == 0 ) continue;
			while( count-- ){
				uint8_t r = app.port4zones[ i ].color >> 24;
				uint8_t g = app.port4zones[ i ].color >> 16;
				uint8_t b = app.port4zones[ i ].color >> 8;
				setLed( port4, led++, r, g, b );
			}
		}
		port4.show();
	}
	if( app.flags.use_port5 ){
		uint16_t led = 0;
		for( uint8_t i = 0; i < ZONES_AT_PORT; i++ ){
			uint16_t count = app.port5zones[ i ].count;
			if( count == 0 ) continue;
			while( count-- ){
				uint8_t r = app.port5zones[ i ].color >> 24;
				uint8_t g = app.port5zones[ i ].color >> 16;
				uint8_t b = app.port5zones[ i ].color >> 8;
				setLed( port5, led++, r, g, b );
			}
		}
		port5.show();
	}
}

//-----------------------------------------------------------------------------------------
void startMode2(void)
{
	clearAll();

	if( app.flags.use_port1 ){
		for( uint16_t i = 0; i < app.port1_leds; i++ ){
			uint8_t g = random( FIRE_GREEN_MIN, 255 );
			setLed( port1, i, 255, g, 0 );
		}
		port1.show();
	}
	if( app.flags.use_port2 ){
		for( uint16_t i = 0; i < app.port2_leds; i++ ){
			uint8_t g = random( FIRE_GREEN_MIN, 255 );
			setLed( port2, i, 255, g, 0 );
		}
		port2.show();
	}
	if( app.flags.use_port3 ){
		for( uint16_t i = 0; i < app.port3_leds; i++ ){
			uint8_t g = random( FIRE_GREEN_MIN, 255 );
			setLed( port3, i, 255, g, 0 );
		}
		port3.show();
	}
	if( app.flags.use_port4 ){
		for( uint16_t i = 0; i < app.port4_leds; i++ ){
			uint8_t g = random( FIRE_GREEN_MIN, 255 );
			setLed( port4, i, 255, g, 0 );
		}
		port4.show();
	}
	if( app.flags.use_port5 ){
		for( uint16_t i = 0; i < app.port5_leds; i++ ){
			uint8_t g = random( FIRE_GREEN_MIN, 255 );
			setLed( port5, i, 255, g, 0 );
		}
		port5.show();
	}

	m_animationCounterMax = 4;
	m_animationCounter = 0;
}

//-----------------------------------------------------------------------------------------
void animationStart(void)
{
	/*
		'Радуга',
		'Разделенщина',
		'Один цвет',
		'Мигалка',
		'Огонь',
		'Гонки',
	
	*/
	switch( app.mode ){
		case 1:		startMode1();		break;
		case 2:		startMode2();		break;	//Fire
		case 0:
		default:
			startMode0();
		break;
	}
}

//-----------------------------------------------------------------------------------------
void animationProcess(void)
{
	switch( app.mode ){
		case 2:		fire();				break;
	}
}

//-----------------------------------------------------------------------------------------
void clearAll(void)
{
	if( app.flags.use_port1 ) lenta_clear( port1 );
	if( app.flags.use_port2 ) lenta_clear( port2 );
	if( app.flags.use_port3 ) lenta_clear( port3 );
	if( app.flags.use_port4 ) lenta_clear( port4 );
	if( app.flags.use_port5 ) lenta_clear( port5 );
}

//-----------------------------------------------------------------------------------------
void getPageHeadler(void)
{
	strcpy( pageBuff, "{\"mode\": " );
	strcat( pageBuff, utoa( app.mode, esp::tmpVal, 10 ) );

	// strcat( pageBuff, ",\"zones\":" ); strcat( pageBuff, utoa( ZONES_AT_PORT, esp::tmpVal, 10 ) );
	// strcat( pageBuff, ",\"zone1leds\":" ); strcat( pageBuff, utoa( app.zone1_leds, esp::tmpVal, 10 ) );
	// strcat( pageBuff, ",\"zone2leds\":" ); strcat( pageBuff, utoa( app.zone2_leds, esp::tmpVal, 10 ) );
	// strcat( pageBuff, ",\"zone3leds\":" ); strcat( pageBuff, utoa( app.zone3_leds, esp::tmpVal, 10 ) );

	// strcat( pageBuff, ",\"status2\":" ); strcat( pageBuff, utoa( app.status_2, esp::tmpVal, 10 ) );
	// strcat( pageBuff, ",\"status3\":" ); strcat( pageBuff, utoa( app.status_3, esp::tmpVal, 10 ) );

	strcat( pageBuff, ",\"ports\": {" );
		strcat( pageBuff, "\"1\": {\"use\":" ); strcat( pageBuff, utoa( app.flags.use_port1, esp::tmpVal, 10 ) );
		strcat( pageBuff, ",\"leds\":" ); strcat( pageBuff, utoa( app.port1_leds, esp::tmpVal, 10 ) );
		strcat( pageBuff, ",\"zones\":[" );
		for( uint8_t i = 0; i < ZONES_AT_PORT; i++ ){
			if( i > 0 ) strcat( pageBuff, "," );
			strcat( pageBuff, "{\"count\":" ); strcat( pageBuff, utoa( app.port1zones[ i ].count, esp::tmpVal, 10 ) );
			strcat( pageBuff, ",\"color\":" ); strcat( pageBuff, utoa( app.port1zones[ i ].color, esp::tmpVal, 10 ) );
			strcat( pageBuff, "}" );
		}
		strcat( pageBuff, "]}" );
		strcat( pageBuff, ",\"2\": {\"use\":" ); strcat( pageBuff, utoa( app.flags.use_port2, esp::tmpVal, 10 ) );
		strcat( pageBuff, ",\"leds\":" ); strcat( pageBuff, utoa( app.port2_leds, esp::tmpVal, 10 ) );
		strcat( pageBuff, ",\"zones\":[" );
		for( uint8_t i = 0; i < ZONES_AT_PORT; i++ ){
			if( i > 0 ) strcat( pageBuff, "," );
			strcat( pageBuff, "{\"count\":" ); strcat( pageBuff, utoa( app.port2zones[ i ].count, esp::tmpVal, 10 ) );
			strcat( pageBuff, ",\"color\":" ); strcat( pageBuff, utoa( app.port2zones[ i ].color, esp::tmpVal, 10 ) );
			strcat( pageBuff, "}" );
		}
		strcat( pageBuff, "]}" );
		strcat( pageBuff, ",\"3\": {\"use\":" ); strcat( pageBuff, utoa( app.flags.use_port3, esp::tmpVal, 10 ) );
		strcat( pageBuff, ",\"leds\":" ); strcat( pageBuff, utoa( app.port3_leds, esp::tmpVal, 10 ) );
		strcat( pageBuff, ",\"zones\":[" );
		for( uint8_t i = 0; i < ZONES_AT_PORT; i++ ){
			if( i > 0 ) strcat( pageBuff, "," );
			strcat( pageBuff, "{\"count\":" ); strcat( pageBuff, utoa( app.port3zones[ i ].count, esp::tmpVal, 10 ) );
			strcat( pageBuff, ",\"color\":" ); strcat( pageBuff, utoa( app.port3zones[ i ].color, esp::tmpVal, 10 ) );
			strcat( pageBuff, "}" );
		}
		strcat( pageBuff, "]}" );
		strcat( pageBuff, ",\"4\": {\"use\":" ); strcat( pageBuff, utoa( app.flags.use_port4, esp::tmpVal, 10 ) );
		strcat( pageBuff, ",\"leds\":" ); strcat( pageBuff, utoa( app.port4_leds, esp::tmpVal, 10 ) );
		strcat( pageBuff, ",\"zones\":[" );
		for( uint8_t i = 0; i < ZONES_AT_PORT; i++ ){
			if( i > 0 ) strcat( pageBuff, "," );
			strcat( pageBuff, "{\"count\":" ); strcat( pageBuff, utoa( app.port4zones[ i ].count, esp::tmpVal, 10 ) );
			strcat( pageBuff, ",\"color\":" ); strcat( pageBuff, utoa( app.port4zones[ i ].color, esp::tmpVal, 10 ) );
			strcat( pageBuff, "}" );
		}
		strcat( pageBuff, "]}" );
		strcat( pageBuff, ",\"5\": {\"use\":" ); strcat( pageBuff, utoa( app.flags.use_port5, esp::tmpVal, 10 ) );
		strcat( pageBuff, ",\"leds\":" ); strcat( pageBuff, utoa( app.port5_leds, esp::tmpVal, 10 ) );
		strcat( pageBuff, ",\"zones\":[" );
		for( uint8_t i = 0; i < ZONES_AT_PORT; i++ ){
			if( i > 0 ) strcat( pageBuff, "," );
			strcat( pageBuff, "{\"count\":" ); strcat( pageBuff, utoa( app.port5zones[ i ].count, esp::tmpVal, 10 ) );
			strcat( pageBuff, ",\"color\":" ); strcat( pageBuff, utoa( app.port5zones[ i ].color, esp::tmpVal, 10 ) );
			strcat( pageBuff, "}" );
		}
		strcat( pageBuff, "]}" );
	strcat( pageBuff, "}}" );
	webServer.send ( 200, "application/json", pageBuff );
}

//-----------------------------------------------------------------------------------------
void setPageHeadler(void)
{
	strcpy( pageBuff, "{\"success\": " );

	bool success = false;

	if( esp::checkWebAuth( &webServer, "login", "password", ESP_AUTH_REALM, "access denied" ) ){
		if( webServer.hasArg( "param" ) && webServer.hasArg( "value" ) ){
			const String &param = webServer.arg( "param" );
			const String &value = webServer.arg( "value" );
		
			if( param == "port1use" ){
				app.flags.use_port1 = ( value.toInt() == 1 ) ? 1 : 0; success = true;
			}else if( param == "port2use" ){
				app.flags.use_port2 = ( value.toInt() == 1 ) ? 1 : 0; success = true;
			}else if( param == "port3use" ){
				app.flags.use_port3 = ( value.toInt() == 1 ) ? 1 : 0; success = true;
			}else if( param == "port4use" ){
				app.flags.use_port4 = ( value.toInt() == 1 ) ? 1 : 0; success = true;
			}else if( param == "port5use" ){
				app.flags.use_port5 = ( value.toInt() == 1 ) ? 1 : 0; success = true;
			}else if( param == "port1leds" ){
				app.port1_leds = (uint8_t)value.toInt(); success = true;
			}else if( param == "port2leds" ){
				app.port2_leds = (uint8_t)value.toInt(); success = true;
			}else if( param == "port3leds" ){
				app.port3_leds = (uint8_t)value.toInt(); success = true;
			}else if( param == "port4leds" ){
				app.port4_leds = (uint8_t)value.toInt(); success = true;
			}else if( param == "port5leds" ){
				app.port5_leds = (uint8_t)value.toInt(); success = true;
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
				ESP_DEBUG( "Set color: [%s][%u]\n", val.c_str(), color );

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
			}else if( param == "mode" ){
				app.mode = (uint8_t)value.toInt();
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
	}



	if( success ){
		strcat( pageBuff, "\"OK\"}" );
	}else{
		strcat( pageBuff, "\"ERROR\"}" );
	}
	
	webServer.send ( 200, "application/json", pageBuff );
}

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
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------