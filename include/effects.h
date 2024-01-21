#ifndef __EFFECTS_H__
#define __EFFECTS_H__

//-----------------------------------------------------------------------------------------
#include "main.h"

//-----------------------------------------------------------------------------------------


//----------- DEFINES ---------------------------------------------------------------------
#ifndef EFFECT_RAINBOW_ANIM_DELAY
	#define EFFECT_RAINBOW_ANIM_DELAY									0
#endif
#ifndef EFFECT_RAINBOW_SPEED
	#define EFFECT_RAINBOW_SPEED										12
#endif
#ifndef EFFECT_RAINBOW_STEP
	#define EFFECT_RAINBOW_STEP											2
#endif

#ifndef EFFECT_FIRE_ANIM_DELAY
	#define EFFECT_FIRE_ANIM_DELAY										0
#endif
#ifndef EFFECT_FIRE_SPEED
	#define EFFECT_FIRE_SPEED											1
#endif
#ifndef EFFECT_FIRE_STEP
	#define EFFECT_FIRE_STEP											15
#endif
#ifndef EFFECT_FIRE_HUE_GAP
	#define EFFECT_FIRE_HUE_GAP											21				 // заброс по hue
#endif
#ifndef EFFECT_FIRE_HUE_START
	#define EFFECT_FIRE_HUE_START										7				 // начальный цвет огня (0 красный, 80 зелёный, 140 молния, 190 розовый)
#endif
#ifndef EFFECT_FIRE_MIN_BRIGHT
	#define EFFECT_FIRE_MIN_BRIGHT										70				 // мин. яркость огня
#endif
#ifndef EFFECT_FIRE_MAX_BRIGHT
	#define EFFECT_FIRE_MAX_BRIGHT										255				 // макс. яркость огня
#endif
#ifndef EFFECT_FIRE_MIN_SAT
	#define EFFECT_FIRE_MIN_SAT											245				 // мин. насыщенность
#endif
#ifndef EFFECT_FIRE_MAX_SAT
	#define EFFECT_FIRE_MAX_SAT											255				 // макс. насыщенность
#endif

#ifndef EFFECT_PULSE_ANIM_DELAY
	#define EFFECT_PULSE_ANIM_DELAY										0
#endif
#ifndef EFFECT_PULSE_SPEED
	#define EFFECT_PULSE_SPEED											4
#endif
#ifndef EFFECT_PULSE_STEP
	#define EFFECT_PULSE_STEP											20
#endif

#define COLOR_STEP														151  // шаг цвета, интересные 151, 129

//----------- STRUCTURES ------------------------------------------------------------------
struct HSV
{
	uint16_t h;
	uint8_t s;
	uint8_t v;
};


//----------- VARIABLES -------------------------------------------------------------------


//----------- FUNCTIONS--------------------------------------------------------------------
void pulse(void);
void rainbow(void);
void fire(void);
void oneColor(const CRGB &color);
void oneColor(const struct Color &color);
void oneColor(const uint8_t r, const uint8_t g, const uint8_t b);
void zonesColor(void);

CRGB getNexLedRianbow(CRGB &pixel, const uint8_t topValue);
void setFireColor(CRGB *led, const uint16_t indx, const uint32_t counter);
HSV rgbToHSV(CRGB &pixel);
//-----------------------------------------------------------------------------------------




#endif //__EFFECTS_H__