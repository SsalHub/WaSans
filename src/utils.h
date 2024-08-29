#ifndef __UTILS__
#define __UTILS__
#include <windows.h>

typedef enum ConsoleColor
{
	_BLACK_ 			= 0,
	_BLUE_				= 1,
	_DARK_GREEN_		= 2,
	_SKYBLUE_			= 3,
	_RED_				= 4,
	_DARK_PURPLE_		= 5,
	_YELLOW_			= 6,
	_BRIGHT_GRAY_		= 7,
	_GRAY_				= 8,
	_OCEAN_BLUE_		= 9,
	_GREEN_				= 10,
	_SKY_				= 11,
	_HOTPINK_			= 12,
	_PURPLE_			= 13,
	_LIGHT_YELLOW_		= 14,
	_WHITE_				= 15,
} ConsoleColor;

typedef enum InputType
{
	_SPACE_ 			= 32,
	_ESCAPE_ 			= 27,
	_LEFT_ 				= 75,
	_RIGHT_ 			= 77,
	_UP_ 				= 72,
	_DOWN_ 				= 80,
	_UPPER_R_ 			= 82,
	_LOWER_R_ 			= 114,
	_CARRIGE_RETURN_ 	= 13,
} InputType;

void sleep(float sec);
#endif