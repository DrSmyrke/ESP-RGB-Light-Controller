#ifndef __EFFECTS_H__
#define __EFFECTS_H__

//-----------------------------------------------------------------------------------------
#include "main.h"

//-----------------------------------------------------------------------------------------


//----------- DEFINES ---------------------------------------------------------------------
#ifndef EFFECT_RAINBOW_SPEED
	#define EFFECT_RAINBOW_SPEED										32
#endif
#ifndef EFFECT_RAINBOW_STEP
	#define EFFECT_RAINBOW_STEP											25
#endif
#ifndef EFFECT_FIRE_SPEED
	#define EFFECT_FIRE_SPEED											8
#endif
#ifndef EFFECT_FIRE_STEP
	#define EFFECT_FIRE_STEP											15
#endif
#ifndef EFFECT_PULSE_SPEED
	#define EFFECT_PULSE_SPEED											4
#endif
#ifndef EFFECT_PULSE_STEP
	#define EFFECT_PULSE_STEP											3
#endif

// настройки пламени
#define EFFECT_FIRE_HUE_GAP	21      // заброс по hue
#define EFFECT_FIRE_HUE_START 0     // начальный цвет огня (0 красный, 80 зелёный, 140 молния, 190 розовый)
#define EFFECT_FIRE_MIN_BRIGHT 70   // мин. яркость огня
#define EFFECT_FIRE_MAX_BRIGHT 255  // макс. яркость огня
#define EFFECT_FIRE_MIN_SAT 245     // мин. насыщенность
#define EFFECT_FIRE_MAX_SAT 255     // макс. насыщенность

#define COLOR_DEBTH 2   // цветовая глубина: 1, 2, 3 (в байтах)

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

CRGB getNexLedRianbow(CRGB pixel, const uint8_t topValue);
void setFireColor(CRGB *led, const uint16_t indx, const uint32_t counter);
HSV rgbToHSV(CRGB &pixel);
//-----------------------------------------------------------------------------------------




#endif //__EFFECTS_H__