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
	currentRenderer = getCurrentRenderer();
	setRenderer(renderFadeEffect);
	while (fadeProgress < ScreenHeight)
	{
		currTime = clock();
		if (fadeSpeed < currTime - oldTime)
		{
			fadeProgress++;
			oldTime = currTime;
		}
		sleep(0.01f);
	}
	setRenderer(NULL);
}

void fadeIn(SCENE_TYPE scene)
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
	currentRenderer = getSceneRenderer(scene);
	setRenderer(renderFadeEffect);
	while (0 < fadeProgress)
	{
		currTime = clock();
		if (fadeSpeed < currTime - oldTime)
		{
			fadeProgress--;
			oldTime = currTime;
		}
		sleep(0.01f);
	}
	setRenderer(currentRenderer);
}

void renderFadeEffect()
{
	COORD begin = { 0, 0 }, end = { ScreenWidth, fadeProgress };
	currentRenderer();
    fillColorInRange(begin, end, _BLACK_);
}

void blackScreenEffect(float t)
{
	int oldTime, i;
    char flag;
    RENDERER* lastRenderer;
	
	oldTime = clock();
	lastRenderer = getCurrentRenderer();
	setRenderer(renderBlackScreenEffect);
//	while (clock() - oldTime < 1000 * t)
//	{
//		sleep(0.1f);
//	}
	sleep(t);
	setRenderer(lastRenderer);
}

void renderBlackScreenEffect()
{
	fillColorToScreen(_BLACK_);
}