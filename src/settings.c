#include "settings.h"

int ScreenWidth = 120, ScreenHeight = 30;
int BaseFrame = 240;
char* PlayerName;
int PlayerLevel;

void waitForFrame()
{
	Sleep(1000 / BaseFrame);
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