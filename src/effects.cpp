#include "functions.h"


//----------- DEFINES ---------------------------------------------------------------------


//----------- VARIABLES -------------------------------------------------------------------
uint32_t fire_counter = 0;

//----------- FUNCTIONS--------------------------------------------------------------------
void pulse(void)
{
	if( m_animationCounter ){
		m_animationCounter--;
		return;
	}

	if( app.param.use.port1 ){
		for( uint16_t i = app.param.port1_size - 1; i > 0; i-- ){
			CRGB color_prew = app.port1[ i - 1 ];
			if( color_prew.r != 0 || color_prew.g != 0 || color_prew.b != 0 ){
				app.port1[ i ] = color_prew;
				HSV hsv = rgbToHSV( color_prew );
				if( hsv.v > app.param.effects.pulse_step ) hsv.v -= app.param.effects.pulse_step;
				if( hsv.v < app.param.effects.pulse_step ) hsv.v = 0;
				color_prew.setHSV( hsv.h, hsv.s, hsv.v );
				app.port1[ i - 1 ] = color_prew;
			}else{
				if( i == ( app.param.port1_size - 1 ) ) app.port1[ i ] = color_prew;
			}
		}
	}
	
	if( app.param.use.port2 ){
		for( uint16_t i = app.param.port2_size - 1; i > 0; i-- ){
			CRGB color_prew = app.port2[ i - 1 ];
			if( color_prew.r != 0 || color_prew.g != 0 || color_prew.b != 0 ){
				app.port2[ i ] = color_prew;
				HSV hsv = rgbToHSV( color_prew );
				if( hsv.v > app.param.effects.pulse_step ) hsv.v -= app.param.effects.pulse_step;
				if( hsv.v < app.param.effects.pulse_step ) hsv.v = 0;
				color_prew.setHSV( hsv.h, hsv.s, hsv.v );
				app.port2[ i - 1 ] = color_prew;
			}else{
				if( i == ( app.param.port2_size - 1 ) ) app.port2[ i ] = color_prew;
			}
		}
	}
	
	if( app.param.use.port3 ){
		for( uint16_t i = app.param.port3_size - 1; i > 0; i-- ){
			CRGB color_prew = app.port3[ i - 1 ];
			if( color_prew.r != 0 || color_prew.g != 0 || color_prew.b != 0 ){
				app.port3[ i ] = color_prew;
				HSV hsv = rgbToHSV( color_prew );
				if( hsv.v > app.param.effects.pulse_step ) hsv.v -= app.param.effects.pulse_step;
				if( hsv.v < app.param.effects.pulse_step ) hsv.v = 0;
				color_prew.setHSV( hsv.h, hsv.s, hsv.v );
				app.port3[ i - 1 ] = color_prew;
			}else{
				if( i == ( app.param.port3_size - 1 ) ) app.port3[ i ] = color_prew;
			}
		}
	}
	
	if( app.param.use.port4 ){
		for( uint16_t i = app.param.port4_size - 1; i > 0; i-- ){
			CRGB color_prew = app.port4[ i - 1 ];
			if( color_prew.r != 0 || color_prew.g != 0 || color_prew.b != 0 ){
				app.port4[ i ] = color_prew;
				HSV hsv = rgbToHSV( color_prew );
				if( hsv.v > app.param.effects.pulse_step ) hsv.v -= app.param.effects.pulse_step;
				if( hsv.v < app.param.effects.pulse_step ) hsv.v = 0;
				color_prew.setHSV( hsv.h, hsv.s, hsv.v );
				app.port4[ i - 1 ] = color_prew;
			}else{
				if( i == ( app.param.port4_size - 1 ) ) app.port4[ i ] = color_prew;
			}
		}
	}
	
	if( app.param.use.port5 ){
		for( uint16_t i = app.param.port5_size - 1; i > 0; i-- ){
			CRGB color_prew = app.port5[ i - 1 ];
			if( color_prew.r != 0 || color_prew.g != 0 || color_prew.b != 0 ){
				app.port5[ i ] = color_prew;
				HSV hsv = rgbToHSV( color_prew );
				if( hsv.v > app.param.effects.pulse_step ) hsv.v -= app.param.effects.pulse_step;
				if( hsv.v < app.param.effects.pulse_step ) hsv.v = 0;
				color_prew.setHSV( hsv.h, hsv.s, hsv.v );
				app.port5[ i - 1 ] = color_prew;
			}else{
				if( i == ( app.param.port5_size - 1 ) ) app.port5[ i ] = color_prew;
			}
		}
	}

	FastLED.show();
	m_animationCounter = m_animationCounterMax;
}

//-----------------------------------------------------------------------------------------
void rainbow(void)
{
	if( m_animationCounter ){
		m_animationCounter--;
		return;
	}

	uint8_t topValue = ( 255 - app.param.effects.rainbow_step );

	if( app.param.use.port1 ){
		for( uint16_t i = 0; i < app.param.port1_size; i++ ){
			CRGB color_cur = app.port1[ i ];
			// ESP_DEBUG( "[%u] %u, %u, %u\n", i, color_cur.r, color_cur.g, color_cur.b );

			if( color_cur.r == 0 && color_cur.g == 0 && color_cur.b == 0 ) continue;

			uint16_t nextIndx = i + 1;
			if( nextIndx >= app.param.port1_size ) nextIndx = 0;
			app.port1[ nextIndx ] = getNexLedRianbow( color_cur, topValue );
		}
	}

	if( app.param.use.port2 ){
		for( uint16_t i = 0; i < app.param.port2_size; i++ ){
			CRGB color_cur = app.port2[ i ];
			if( color_cur.r == 0 && color_cur.g == 0 && color_cur.b == 0 ) continue;
			uint16_t nextIndx = i + 1;
			if( nextIndx >= app.param.port2_size ) nextIndx = 0;
			app.port2[ nextIndx ] = getNexLedRianbow( color_cur, topValue );
		}
	}

	if( app.param.use.port3 ){
		for( uint16_t i = 0; i < app.param.port3_size; i++ ){
			CRGB color_cur = app.port3[ i ];
			if( color_cur.r == 0 && color_cur.g == 0 && color_cur.b == 0 ) continue;
			uint16_t nextIndx = i + 1;
			if( nextIndx >= app.param.port3_size ) nextIndx = 0;
			app.port3[ nextIndx ] = getNexLedRianbow( color_cur, topValue );
		}
	}

	if( app.param.use.port4 ){
		for( uint16_t i = 0; i < app.param.port4_size; i++ ){
			CRGB color_cur = app.port4[ i ];
			if( color_cur.r == 0 && color_cur.g == 0 && color_cur.b == 0 ) continue;
			uint16_t nextIndx = i + 1;
			if( nextIndx >= app.param.port4_size ) nextIndx = 0;
			app.port4[ nextIndx ] = getNexLedRianbow( color_cur, topValue );
		}
	}

	if( app.param.use.port5 ){
		for( uint16_t i = 0; i < app.param.port5_size; i++ ){
			CRGB color_cur = app.port5[ i ];
			if( color_cur.r == 0 && color_cur.g == 0 && color_cur.b == 0 ) continue;
			uint16_t nextIndx = i + 1;
			if( nextIndx >= app.param.port5_size ) nextIndx = 0;
			app.port5[ nextIndx ] = getNexLedRianbow( color_cur, topValue );
		}
	}

	FastLED.show();
	m_animationCounter = m_animationCounterMax;
}

//-----------------------------------------------------------------------------------------
void fire(void)
{
	if( m_animationCounter ){
		m_animationCounter--;
		return;
	}

	if( app.param.use.port1 ){
		for( uint16_t i = 0; i < app.param.port1_size; i++ ){
			setFireColor( &app.port1[ i ], i, fire_counter );
		}
		fire_counter += 20;
	}
	if( app.param.use.port2 ){
		for( uint16_t i = 0; i < app.param.port2_size; i++ ){
			setFireColor( &app.port2[ i ], i, fire_counter );
		}
		fire_counter += 20;
	}
	if( app.param.use.port3 ){
		for( uint16_t i = 0; i < app.param.port3_size; i++ ){
			setFireColor( &app.port3[ i ], i, fire_counter );
		}
		fire_counter += 20;
	}
	if( app.param.use.port4 ){
		for( uint16_t i = 0; i < app.param.port4_size; i++ ){
			setFireColor( &app.port4[ i ], i, fire_counter );
		}
		fire_counter += 20;
	}
	if( app.param.use.port5 ){
		for( uint16_t i = 0; i < app.param.port5_size; i++ ){
			setFireColor( &app.port5[ i ], i, fire_counter );
		}
		fire_counter += 20;
	}

	FastLED.show();
	m_animationCounter = m_animationCounterMax;
}


//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
CRGB getNexLedRianbow(CRGB pixel, const uint8_t topValue)
{
	if( pixel.r == 255 && pixel.b == 0 ){
		if( pixel.g <= topValue ) pixel.g += app.param.effects.rainbow_step;
		if( pixel.g > topValue ){
			pixel.g = 255;
			pixel.r--;
		}
	}else if( pixel.r > 0 && pixel.g == 255 && pixel.b == 0 ){
		if( pixel.r >= app.param.effects.rainbow_step ) pixel.r -= app.param.effects.rainbow_step;
		if( pixel.r < app.param.effects.rainbow_step ) pixel.r = 0;
	}else if( pixel.r == 0 && pixel.g == 255 ){
		if( pixel.b <= topValue ) pixel.b += app.param.effects.rainbow_step;
		if( pixel.b > topValue ){
			pixel.b = 255;
			pixel.g--;
		}
	}else if( pixel.r == 0 && pixel.b == 255 ){
		if( pixel.g >= app.param.effects.rainbow_step ) pixel.g -= app.param.effects.rainbow_step;
		if( pixel.g < app.param.effects.rainbow_step ) pixel.g = 0;
		if( pixel.g == 0 ) pixel.r++;
	}else if( pixel.g == 0 && pixel.b == 255 ){
		if( pixel.r <= topValue ) pixel.r += app.param.effects.rainbow_step;
		if( pixel.r > topValue ){
			pixel.r = 255;
			pixel.b--;
		}
	}else if( pixel.r == 255 && pixel.b > 0 ){
		if( pixel.b >= app.param.effects.rainbow_step ) pixel.b -= app.param.effects.rainbow_step;
		if( pixel.b < app.param.effects.rainbow_step ) pixel.b = 0;
	}

	return pixel;
}

//-----------------------------------------------------------------------------------------
void setFireColor(CRGB *led, const uint16_t indx, const uint32_t counter)
{
	uint8_t value = inoise8( indx * app.param.effects.fire_step, counter );
	uint8_t h = EFFECT_FIRE_HUE_START + map( value, 0, 255, 0, EFFECT_FIRE_HUE_GAP);
	uint8_t s = constrain(map( value, 0, 255, EFFECT_FIRE_MAX_SAT, EFFECT_FIRE_MIN_SAT), 0, 255);
	uint8_t v = constrain(map( value, 0, 255, EFFECT_FIRE_MIN_BRIGHT, EFFECT_FIRE_MAX_BRIGHT), 0, 255);
	led->setHSV( h, s, v );
}

//-----------------------------------------------------------------------------------------
HSV rgbToHSV(CRGB &pixel)
{
	HSV hsv = {h: 0, s: 0, v: 0};

	uint8_t min = pixel.r;
	if( pixel.g < min ) min = pixel.g;
	if( pixel.b < min ) min = pixel.b;
	uint8_t max = pixel.r;
	if( pixel.g > max ) max = pixel.g;
	if( pixel.b > max ) max = pixel.b;
	uint8_t delta = max - min;
	hsv.v = max;
	hsv.s = max != 0 ? 255 * delta / max : 0;
	if( hsv.s != 0 ){
		if( pixel.r == max ) hsv.h = ( pixel.g - pixel.b ) / delta;
		else if( pixel.g == max ) hsv.h = 2 + ( pixel.b - pixel.r ) / delta;
		else hsv.h = 4 + ( pixel.r - pixel.g ) / delta;
	}else hsv.h = -1;
	hsv.h *= 60;
	if (hsv.h < 0) hsv.h += 360;
	hsv.s *= 100 / 255;
	hsv.v *= MAX_BRIGHTNESS / 255;
	return hsv;
}

//-----------------------------------------------------------------------------------------