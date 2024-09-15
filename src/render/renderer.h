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
	int x;
	int y;
	char* s;
	ConsoleColor tColor;
	ConsoleColor bColor;
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
void fillColorInRange(int begX, int begY, int endX, int endY, ConsoleColor bColor);
void fillColorToScreen(ConsoleColor bColor);
void printLine(int x, int y, char* str, ConsoleColor tColor, ConsoleColor bColor);
void printLines(int x, int y, char* str, ConsoleColor tColor, ConsoleColor bColor);
void setRenderInfo(RenderInfo* target, int x, int y, char* s, ConsoleColor tColor, ConsoleColor bColor);
void setRenderer(Renderer* renderer);
Renderer* getCurrentRenderer();
void printFPS();
void waitForFrame();
void beginRenderThread();
unsigned __stdcall renderThread();
void releaseScreen();
#endif