#include "uieffect.h"

void fadeOut()
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
	currentRenderer = getSceneRenderer();
	setSceneRenderer(renderFadeEffect);
	while (fadeProgress < ScreenHeight)
	{
		currTime = clock();
		if (fadeSpeed < currTime - oldTime)
		{
			fadeProgress++;
			oldTime = currTime;
		}
		waitForFrame();
	}
	setSceneRenderer(NULL);
}

void fadeIn(Renderer* nextRenderer)
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
	currentRenderer = nextRenderer;
	setSceneRenderer(renderFadeEffect);
	while (1 < fadeProgress)
	{
		currTime = clock();
		if (fadeSpeed < currTime - oldTime)
		{
			fadeProgress--;
			oldTime = currTime;
		}
		waitForFrame();
	}
	setSceneRenderer(nextRenderer);
}

void renderFadeEffect()
{
	int i;
	(*currentRenderer)();
    for (i = 0; i < fadeProgress; i++)
        printLine(0, i, buffer, _BLACK_, _BLACK_);
}

void blackScreenEffect(float t)
{
	int oldTime, i;
    char flag;
	
	oldTime = clock();
	buffer = &flag;
	setSceneRenderer(renderBlackScreenEffect);
	while (clock() - oldTime < 1000 * t)
	{
		waitForFrame();
//	    fillColorToScreen(_BLACK_, _BLACK_);
	}
	buffer = NULL;
}

void renderBlackScreenEffect()
{
	if (buffer)
		fillColorToScreen(_BLACK_, _BLACK_);
}