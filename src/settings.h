#ifndef __SETTINGS__
#define __SETTINGS__
#include <Windows.h>
#include <string.h>

typedef enum ExitCode
{
	_EXIT_NONE_			= -1,
	_EXIT_GAME_			= 0,
	_LOAD_ASSET_ERROR_	= 1,
} ExitCode;

extern int ScreenWidth, ScreenHeight;
extern int BaseFrame;
extern char* PlayerName;
extern int PlayerLevel;

void waitForFrame();
void initPlayerName(char* name);
void releasePlayerName();
#endif