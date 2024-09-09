#include "uieffect.h"

void fadeOut(void (*renderer)(void))
{
	const int fadeSpeed = 40;
    int fadeProgress, oldTime, currTime, i;
    char fadeLine[ScreenWidth + 1];

    for (i = 0; i < ScreenWidth; i++)
        fadeLine[i] = ' ';
    fadeLine[ScreenWidth] = '\0';

	oldTime = clock();
	fadeProgress = 1;
	while (fadeProgress <= ScreenHeight)
	{
		clearScreen();
        (*renderer)();
	    for (i = 0; i < fadeProgress; i++)
	        printLine(0, i, fadeLine, _BLACK_, _BLACK_);
		printFrameInfo();
	    flipScreen();
		setFrameSpeed();
		
		currTime = clock();
		if (fadeSpeed < currTime - oldTime)
		{
			fadeProgress++;
			oldTime = currTime;
		}
	}
}

void fadeIn(void (*renderer)(void))
{
	const int fadeSpeed = 40;
    int fadeProgress, oldTime, currTime, i;
    char fadeLine[ScreenWidth + 1];

    for (i = 0; i < ScreenWidth; i++)
        fadeLine[i] = ' ';
    fadeLine[ScreenWidth] = '\0';

	oldTime = clock();
	fadeProgress = ScreenHeight;
	while (1 <= fadeProgress)
	{
		clearScreen();
        (*renderer)();
	    for (i = 0; i < fadeProgress; i++)
	        printLine(0, i, fadeLine, _BLACK_, _BLACK_);
		printFrameInfo();
	    flipScreen();
		setFrameSpeed();
		
		currTime = clock();
		if (fadeSpeed < currTime - oldTime)
		{
			fadeProgress--;
			oldTime = currTime;
		}
	}
}

void blackScreenEffect(float t)
{
	int oldTime, i;
    char fadeLine[ScreenWidth + 1];
	
    for (i = 0; i < ScreenWidth; i++)
        fadeLine[i] = ' ';
    fadeLine[ScreenWidth] = '\0';
	
	oldTime = clock();
	while (clock() - oldTime < 1000 * t)
	{
	    clearScreen();
	    for (i = 0; i < ScreenHeight; i++)
	        printLine(0, i, fadeLine, _BLACK_, _BLACK_);
		printFrameInfo();
	    flipScreen();
		setFrameSpeed();
	}
}