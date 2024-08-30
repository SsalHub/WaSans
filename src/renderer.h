#ifndef __RENDER__
#define __RENDER__
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "settings.h"
#include "utils.h"

extern int ScreenIndex;
extern HANDLE ScreenHandle[2];

void initScreen();
void setWindowInfo(int w, int h);
void flipScreen();
void clearScreen();
void fillColorToScreen(ConsoleColor bColor, ConsoleColor tColor);
void releaseScreen();
void printScreen(int x, int y, char* str);
void renderScreen();
void printFPS();
#endif