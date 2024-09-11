#include "settings.h"

int ScreenWidth = 120, ScreenHeight = 30;
int BaseFrame = 180;
int MaxHP = 100;

void waitForFrame()
{
	Sleep(1000 / BaseFrame);
}