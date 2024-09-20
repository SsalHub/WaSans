#ifndef __RENDERER__
#define __RENDERER__
#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <process.h>
#include "../settings.h"
#include "../utils.h"

typedef void (RENDERER)(void);

typedef struct RENDER_INFO
{
	COORD pos;
	char* s;
	CONSOLE_COLOR tColor;
	CONSOLE_COLOR bColor;
	int width;
	int height;
} RENDER_INFO;

typedef enum TEXT_ALIGN
{
	_ALIGN_CENTER_ = -100,
	_ALIGN_LEFT_,
	_ALIGN_RIGHT_,
	_ALIGN_TOP_,
	_ALIGN_BOTTOM_,
} TEXT_ALIGN;

extern int ScreenIndex;
extern HANDLE ScreenHandle[2];

static int bRender;
static RENDERER* sceneRenderer;
static int bRenderThread;
static unsigned int pRenderThread;
static HANDLE hRenderThread;
static char* lineBuffer, *blankBuffer;

void initScreen();
void setWindowInfo(int w, int h);
void flipScreen();
void clearScreen();
void fillColorInRange(COORD begin, COORD end, CONSOLE_COLOR bColor);
void fillColorToScreen(CONSOLE_COLOR bColor);
void printLine(int x, int y, char *str, CONSOLE_COLOR tColor, CONSOLE_COLOR bColor);
void printLines(int x, int y, char *str, CONSOLE_COLOR tColor, CONSOLE_COLOR bColor);

void setRenderInfo(RENDER_INFO *target, COORD pos, char *s, CONSOLE_COLOR tColor, CONSOLE_COLOR bColor);
void setRenderInfoAttr(RENDER_INFO *target, COORD pos, int w, int h, CONSOLE_COLOR tColor, CONSOLE_COLOR bColor);
void setRenderer(RENDERER* RENDERER);
RENDERER* getCurrentRenderer();

void printFPS();
void waitForFrame();

void beginRenderThread();
unsigned __stdcall renderThread();

void releaseScreen();
#endif