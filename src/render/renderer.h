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

typedef void (Renderer)(void);

typedef struct RenderInfo
{
	COORD pos;
	char* s;
	CONSOLE_COLOR tColor;
	CONSOLE_COLOR bColor;
	int width;
	int height;
} RenderInfo;

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

static int bRender;
static Renderer* sceneRenderer;
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

void setRenderInfo(RenderInfo *target, COORD pos, char *s, CONSOLE_COLOR tColor, CONSOLE_COLOR bColor);
void setRenderInfoAttr(RenderInfo *target, COORD pos, int w, int h, CONSOLE_COLOR tColor, CONSOLE_COLOR bColor);
void setRenderer(Renderer* renderer);
Renderer* getCurrentRenderer();

void printFPS();
void waitForFrame();

void beginRenderThread();
unsigned __stdcall renderThread();

void releaseScreen();
#endif