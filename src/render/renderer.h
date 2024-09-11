#ifndef __RENDERER__
#define __RENDERER__
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <windows.h>
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
extern char* ScreenBuffer;

static int bRender, FPS, oldFPS;
static Renderer* sceneRenderer;
static int bRenderThread;
static unsigned int pRenderThread;
static HANDLE hRenderThread;

void initScreen();
void setWindowInfo(int w, int h);
void flipScreen();
void clearScreen();
void fillColorToScreen(ConsoleColor tColor, ConsoleColor bColor);
void printLine(int x, int y, char* str, ConsoleColor tColor, ConsoleColor bColor);
void printLines(int x, int y, char* str, ConsoleColor tColor, ConsoleColor bColor);
void render();
void renderCustom(Renderer *renderer);
void setRenderInfo(RenderInfo* target, int x, int y, char* s, ConsoleColor tColor, ConsoleColor bColor);
void setSceneRenderer(Renderer* renderer);
Renderer* getSceneRenderer();
void printFPS();
void checkFPS();
void waitForFrame();
void beginRenderThread();
unsigned __stdcall renderThread();
void releaseScreen();
#endif