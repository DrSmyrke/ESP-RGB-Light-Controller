#include "functions.h"


//----------- DEFINES ---------------------------------------------------------------------


//----------- VARIABLES -------------------------------------------------------------------
uint32_t fire_counter = 0;

//----------- FUNCTIONS--------------------------------------------------------------------
void pulse(void)
{
	app.param.effects.pulse_step = 20;

	for( uint8_t oIndx = 0; oIndx < LENTS_COUNT_MAX; oIndx++ ){
		Order order = app.param.effects.pulse_orders[ oIndx ];
		CRGB* port = getPortPtr( order.port );
		uint16_t size = getPortSize( order.port );

		if(  port != nullptr ){
			if( order.dir ){
				uint16_t j = size - 1;
				// Move current led at next port
				CRGB* color_cur = &port[ j ];
				if( color_cur->r != 0 || color_cur->g != 0 || color_cur->b != 0 ){
					moveColorAtNextPort( oIndx, app.param.effects.pulse_orders, color_cur, 1, 0 );
				}

				while( j ){
					CRGB color_cur = port[ j ];
					CRGB color_prew = port[ j - 1 ];
					if( color_cur.r >= app.param.effects.pulse_step ) color_prew.r = color_cur.r - app.param.effects.pulse_step;
					if( color_cur.g >= app.param.effects.pulse_step ) color_prew.g = color_cur.g - app.param.effects.pulse_step;
					if( color_cur.b >= app.param.effects.pulse_step ) color_prew.b = color_cur.b - app.param.effects.pulse_step;
					if( color_prew.r < app.param.effects.pulse_step ) color_prew.r = 0;
					if( color_prew.g < app.param.effects.pulse_step ) color_prew.g = 0;
					if( color_prew.b < app.param.effects.pulse_step ) color_prew.b = 0;
					port[ j ] = color_prew;
					j--;
				}
			}else{
				// Move current led at next port
				CRGB* color_cur = &port[ 0 ];
				if( color_cur->r != 0 || color_cur->g != 0 || color_cur->b != 0 ){
					moveColorAtNextPort( oIndx, app.param.effects.pulse_orders, color_cur, 1, 0 );
				}

				for( uint16_t i = 0; i < size - 1; i++ ){
					CRGB color_cur = port[ i ];
					CRGB color_prew = port[ i + 1 ];
					if( color_cur.r >= app.param.effects.pulse_step ) color_prew.r = color_cur.r - app.param.effects.pulse_step;
					if( color_cur.g >= app.param.effects.pulse_step ) color_prew.g = color_cur.g - app.param.effects.pulse_step;
					if( color_cur.b >= app.param.effects.pulse_step ) color_prew.b = color_cur.b - app.param.effects.pulse_step;
					if( color_prew.r < app.param.effects.pulse_step ) color_prew.r = 0;
					if( color_prew.g < app.param.effects.pulse_step ) color_prew.g = 0;
					if( color_prew.b < app.param.effects.pulse_step ) color_prew.b = 0;
					port[ i ] = color_prew;
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------------------
void rainbow(void)
{
	uint8_t topValue = ( 255 - app.param.effects.rainbow_step );
	
	for( uint8_t oIndx = 0; oIndx < LENTS_COUNT_MAX; oIndx++ ){
		Order order = app.param.effects.rainbow_orders[ oIndx ];
		CRGB* port = getPortPtr( order.port );
		uint16_t size = getPortSize( order.port );

		if(  port != nullptr ){
			if( order.dir ){
				uint16_t j = size - 1;
				// Move current led at next port
				CRGB* color_cur = &port[ j ];
				if( color_cur->r != 0 || color_cur->g != 0 || color_cur->b != 0 ){
					moveColorAtNextPort( oIndx, app.param.effects.rainbow_orders, color_cur, 1, 0 );
				}
				while( j ){
					CRGB color_cur = port[ j ];
					CRGB color_prew = port[ j - 1 ];
					if( color_prew.r == 0 && color_prew.g == 0 && color_prew.b == 0 ) color_prew = getNexLedRianbow( color_cur, topValue );
					port[ j ] = color_prew;
					port[ j - 1 ] = getNexLedRianbow( color_cur, topValue );
					j--;
				}
			}else{
				// Move current led at next port
				CRGB* color_cur = &port[ 0 ];
				if( color_cur->r != 0 || color_cur->g != 0 || color_cur->b != 0 ){
					moveColorAtNextPort( oIndx, app.param.effects.rainbow_orders, color_cur, 1, 0 );
				}
				for( uint16_t i = 0; i < size - 1; i++ ){
					CRGB color_cur = port[ i ];
					CRGB color_prew = port[ i + 1 ];
					port[ i ] = color_prew;
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------------------
void fire(void)
{
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
}

//-----------------------------------------------------------------------------------------
void oneColor(const CRGB &color)
{
	oneColor( color.r, color.g, color.b );
}

//-----------------------------------------------------------------------------------------
void oneColor(const struct Color &color)
{
	oneColor( color.r, color.g, color.b );
}

//-----------------------------------------------------------------------------------------
void oneColor(const uint8_t r, const uint8_t g, const uint8_t b)
{
	if( app.param.use.port1 ){
		for( uint16_t i = 0; i < app.param.port1_size; i++ ){
			app.port1[ i ].r = r;
			app.port1[ i ].g = g;
			app.port1[ i ].b = b;
		}
	}
	if( app.param.use.port2 ){
		for( uint16_t i = 0; i < app.param.port2_size; i++ ){
			app.port2[ i ].r = r;
			app.port2[ i ].g = g;
			app.port2[ i ].b = b;
		}
	}
	if( app.param.use.port3 ){
		for( uint16_t i = 0; i < app.param.port3_size; i++ ){
			app.port3[ i ].r = r;
			app.port3[ i ].g = g;
			app.port3[ i ].b = b;
		}
	}
	if( app.param.use.port4 ){
		for( uint16_t i = 0; i < app.param.port4_size; i++ ){
			app.port4[ i ].r = r;
			app.port4[ i ].g = g;
			app.port4[ i ].b = b;
		}
	}
	if( app.param.use.port5 ){
		for( uint16_t i = 0; i < app.param.port5_size; i++ ){
			app.port5[ i ].r = r;
			app.port5[ i ].g = g;
			app.port5[ i ].b = b;
		}
	}
}

//-----------------------------------------------------------------------------------------
void zonesColor(void)
{
	for( uint16_t i = 0; i < ZONES_MAX; i++ ){
		Zone* zone = &app.param.zones[ i ];
		if( !zone->port ) continue;

		CRGB color = CRGB( app.param.effects.zonesColor[ i ].r, app.param.effects.zonesColor[ i ].g, app.param.effects.zonesColor[ i ].b );
		uint16_t finalPos = zone->start + zone->count;

		CRGB* port = getPortPtr( zone->port );
		uint16_t size = getPortSize( zone->port );

		if(  port != nullptr && finalPos <= size ){
			for( uint16_t j = zone->start; j < finalPos; j++ ) port[ j ] = color;
		}
	}
}

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
CRGB getNexLedRianbow(CRGB &pixel, const uint8_t topValue)
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
	uint8_t h = app.param.effects.fire_hue_start + map( value, 0, 255, 0, app.param.effects.fire_hue_gap );
	uint8_t s = constrain(map( value, 0, 255, app.param.effects.fire_max_sat, app.param.effects.fire_min_sat), 0, 255);
	uint8_t v = constrain(map( value, 0, 255, app.param.effects.fire_min_bright, app.param.effects.fire_max_bright ), 0, 255);
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