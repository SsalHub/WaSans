#include "uieffect.h"

void fadeOut(void (*renderer)(void))
{
	int fadeProgress, i;
	char fadeLine[ScreenWidth + 1];
	
	for (i = 0; i < ScreenWidth; i++)
		fadeLine[i] = ' ';
	fadeLine[ScreenWidth] = '\0';
	
	for (fadeProgress = 1; fadeProgress <= ScreenHeight; fadeProgress++)
	{
		clearScreen();
		(*renderer)();
		for (i = 0; i < fadeProgress; i++)
			printLine(0, i, fadeLine, _BLACK_, _BLACK_);
		flipScreen();
		Sleep(30);
	}
}

void fadeIn(void (*renderer)(void))
{
	int fadeProgress, i;
	char fadeLine[ScreenWidth + 1];
	
	for (i = 0; i < ScreenWidth; i++)
		fadeLine[i] = ' ';
	fadeLine[ScreenWidth] = '\0';
	
	for (fadeProgress = ScreenHeight; 1 <= fadeProgress; fadeProgress--)
	{
		clearScreen();
		(*renderer)();
		for (i = 0; i < fadeProgress; i++)
			printLine(0, i, fadeLine, _BLACK_, _BLACK_);
		flipScreen();
		Sleep(30);
	}
}