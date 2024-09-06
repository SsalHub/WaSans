#ifndef __RENDER__
#define __RENDER__
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "../settings.h"
#include "../utils.h"

extern int ScreenIndex;
extern HANDLE ScreenHandle[2];

typedef enum TextAlign
{
	_ALIGN_CENTER_ = -100,
	_ALIGN_LEFT_,
	_ALIGN_RIGHT_,
	_ALIGN_TOP_,
	_ALIGN_BOTTOM_,
} TextAlign;

void initScreen();
void setWindowInfo(int w, int h);
void flipScreen();
void clearScreen();
void fillColorToScreen(ConsoleColor tColor, ConsoleColor bColor);
void releaseScreen();
void printLine(int x, int y, char* str, ConsoleColor tColor, ConsoleColor bColor);
void printLines(int x, int y, char* str, ConsoleColor tColor, ConsoleColor bColor);
void render();
void renderCustom(void (*customRenderer)(void));
void printFrameInfo();
void setFrameSpeed();
#endif