#include "settings.h"


int ScreenWidth = 120, ScreenHeight = 30;
int BaseFrame = 60;
float FPS = 0;
char* PlayerName;
int PlayerLevel;



void waitForFrame()
{
	Sleep(1000 / BaseFrame);
}

void tuneFrameDelay()
{
	int delay = 1000 * (fpsCount / (float)BaseFrame) - (clock() - oldTime);
	if (0 < delay)
		Sleep(delay);
}

void checkFPS()
{
	int currTime;
	if (fpsCount <= 0)
		oldTime = clock();
	if (BaseFrame <= fpsCount)
	{
		currTime = clock();
		FPS = 1000 / ((currTime - oldTime) / (float)BaseFrame);
		fpsCount = 0;
		oldTime = currTime;
	}
	fpsCount++;
}

void initPlayerName(char* name)
{
	PlayerName = (char*)malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(PlayerName, name);
}

void releasePlayerName()
{
	free(PlayerName);
}