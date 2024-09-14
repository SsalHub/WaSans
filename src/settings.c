#include "settings.h"

int ScreenWidth = 120, ScreenHeight = 30;
int BaseFrame = 60;
float FPS = 0;
char* PlayerName;
int PlayerLevel;

void waitForFrame()
{
	int delay = fpsCount * 1000 / BaseFrame - (clock() - oldTime);
	if (0 < delay)
		Sleep(delay);
}

void checkFPS()
{
	int currTime;
	
	if (fpsCount <= 0)
	{
		oldTime = clock();
	}
	else
	{
		if (60 <= fpsCount)
		{
			currTime = clock();
			FPS = 1000 / ((currTime - oldTime) / BaseFrame);
			fpsCount = 0;
			oldTime = currTime;
		}
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