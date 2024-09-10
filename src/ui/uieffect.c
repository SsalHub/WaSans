#include "uieffect.h"

void fadeOut(void (*func)(void))
{
	const int fadeSpeed = 40;
    int oldTime, currTime, i;
    char fadeLine[ScreenWidth + 1];

    for (i = 0; i < ScreenWidth; i++)
        fadeLine[i] = ' ';
    fadeLine[ScreenWidth] = '\0';
    buffer = fadeLine;

	oldTime = clock();
	fadeProgress = 1;
	renderer = func;
	setCustomRenderer(renderFadeEffect);
	while (fadeProgress <= ScreenHeight)
	{
        (*renderer)();
		
		currTime = clock();
		if (fadeSpeed < currTime - oldTime)
		{
			fadeProgress++;
			oldTime = currTime;
		}
	}
}

void fadeIn(void (*func)(void))
{
	const int fadeSpeed = 40;
    int oldTime, currTime, i;
    char fadeLine[ScreenWidth + 1];

    for (i = 0; i < ScreenWidth; i++)
        fadeLine[i] = ' ';
    fadeLine[ScreenWidth] = '\0';
    buffer = fadeLine;

	oldTime = clock();
	fadeProgress = ScreenHeight;
	renderer = func;
	setCustomRenderer(renderFadeEffect);
	while (1 <= fadeProgress)
	{
		currTime = clock();
		if (fadeSpeed < currTime - oldTime)
		{
			fadeProgress--;
			oldTime = currTime;
		}
	}
}

void renderFadeEffect()
{
	int i;
    for (i = 0; i < fadeProgress; i++)
        printLine(0, i, buffer, _BLACK_, _BLACK_);
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
	    fillColorToScreen(_BLACK_, _BLACK_);
	}
}