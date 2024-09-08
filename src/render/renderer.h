#ifndef __RENDERER__
#define __RENDERER__
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "../settings.h"
#include "../utils.h"

typedef enum TextAlign
{
	_ALIGN_CENTER_ = -100,
	_ALIGN_LEFT_,
	_ALIGN_RIGHT_,
	_ALIGN_TOP_,
	_ALIGN_BOTTOM_,
} TextAlign;

extern int ScreenIndex;
extern HANDLE ScreenHandle[2];
extern char* ScreenBuffer;

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