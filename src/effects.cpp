#include "functions.h"


//----------- VARIABLES --------------------------------------------------------------------

//----------- FUNCTIONS--------------------------------------------------------------------
void fire(void)
{
	if( m_animationCounter ){
		m_animationCounter--;
		return;
	}

	if( app.flags.use_port1 ){
		for( uint16_t i = 0; i < app.port1_leds; i++ ){
			uint32_t color = port1.getPixelColor( i );
			uint8_t g = color >> 8;
			g += random( 2, 10 );
			if( g < FIRE_GREEN_MIN ) g = FIRE_GREEN_MIN;
			setLed( port1, i, 255, g, 0 );
		}
		port1.show();
	}
	if( app.flags.use_port2 ){
		for( uint16_t i = 0; i < app.port2_leds; i++ ){
			uint32_t color = port2.getPixelColor( i );
			uint8_t g = color >> 8;
			g += random( 2, 10 );
			if( g < FIRE_GREEN_MIN ) g = FIRE_GREEN_MIN;
			setLed( port2, i, 255, g, 0 );
		}
		port2.show();
	}
	if( app.flags.use_port3 ){
		for( uint16_t i = 0; i < app.port3_leds; i++ ){
			uint32_t color = port3.getPixelColor( i );
			uint8_t g = color >> 8;
			g += random( 2, 10 );
			if( g < FIRE_GREEN_MIN ) g = FIRE_GREEN_MIN;
			setLed( port3, i, 255, g, 0 );
		}
		port3.show();
	}
	if( app.flags.use_port4 ){
		for( uint16_t i = 0; i < app.port4_leds; i++ ){
			uint32_t color = port4.getPixelColor( i );
			uint8_t g = color >> 8;
			g += random( 2, 10 );
			if( g < FIRE_GREEN_MIN ) g = FIRE_GREEN_MIN;
			setLed( port4, i, 255, g, 0 );
		}
		port4.show();
	}
	if( app.flags.use_port5 ){
		for( uint16_t i = 0; i < app.port5_leds; i++ ){
			uint32_t color = port5.getPixelColor( i );
			uint8_t g = color >> 8;
			g += random( 2, 10 );
			if( g < FIRE_GREEN_MIN ) g = FIRE_GREEN_MIN;
			setLed( port5, i, 255, g, 0 );
		}
		port5.show();
	}

	m_animationCounter = m_animationCounterMax;
}

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------