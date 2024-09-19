#include "renderer.h"

int RendererThreadFlag;
HANDLE RendererThread;  
unsigned int RendererThreadAddr;
int ScreenIndex;
HANDLE ScreenHandle[2];



/*  */
void initScreen()
{
	CONSOLE_CURSOR_INFO cinfo;
	int i;
	
	ScreenIndex = 0;
	ScreenHandle[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	ScreenHandle[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	
	cinfo.dwSize = 1;
	cinfo.bVisible = FALSE;
	SetConsoleCursorInfo(ScreenHandle[0], &cinfo);
	SetConsoleCursorInfo(ScreenHandle[1], &cinfo);
	
	setWindowInfo(ScreenWidth, ScreenHeight);
	
	lineBuffer = (char*)malloc(sizeof(char) * (ScreenWidth + 1));
	blankBuffer = (char*)malloc(sizeof(char) * (ScreenWidth + 1));
	for (i = 0; i < ScreenWidth; i++)
		blankBuffer[i] = ' ';	
	blankBuffer[ScreenWidth] = '\0';
	setRenderer(NULL);
}

void setWindowInfo(int w, int h)
{
	COORD size = { w, h };
	SMALL_RECT rect;
	rect.Left = 0;
	rect.Top = 0;
	rect.Right = w - 1;
	rect.Bottom = h - 1;
	SetConsoleScreenBufferSize(ScreenHandle[0], size);
	SetConsoleScreenBufferSize(ScreenHandle[1], size);
	SetConsoleWindowInfo(ScreenHandle[0], TRUE, &rect);
	SetConsoleWindowInfo(ScreenHandle[1], TRUE, &rect);
	SetConsoleTitle("SANS with Clang");
}

void flipScreen() 
{
	SetConsoleActiveScreenBuffer(ScreenHandle[ScreenIndex]);
	ScreenIndex = !ScreenIndex;
}

void clearScreen()
{
	COORD pos = { 0, 0 };
	DWORD dw;
	FillConsoleOutputCharacter(ScreenHandle[ScreenIndex], ' ', ScreenWidth * ScreenHeight, pos, &dw);
	FillConsoleOutputAttribute(ScreenHandle[ScreenIndex], 0, ScreenWidth * ScreenHeight, pos, &dw);
//	printLines(x, y, ScreenBuffer, _BLACK_, _BLACK_);
}

void fillColorInRange(COORD begin, COORD end, ConsoleColor bColor)
{
	if (end.Y < begin.Y)
		return;
	COORD pos = { begin.X, begin.Y };
	DWORD dw;
	int w = end.X - begin.X + 1, h = end.Y - begin.Y + 1, targetY = pos.Y + h;
	do
	{
		FillConsoleOutputCharacter(ScreenHandle[ScreenIndex], ' ', w, pos, &dw);
		FillConsoleOutputAttribute(ScreenHandle[ScreenIndex], 0 | (bColor << 4), w, pos, &dw);
		pos.Y++;
	} while (pos.Y < targetY);
}

void fillColorToScreen(ConsoleColor bColor)
{
	COORD pos = { 0, 0 };
	DWORD dw;
	FillConsoleOutputCharacter(ScreenHandle[ScreenIndex], ' ', ScreenWidth * ScreenHeight, pos, &dw);
	FillConsoleOutputAttribute(ScreenHandle[ScreenIndex], 0 | (bColor << 4), ScreenWidth * ScreenHeight, pos, &dw);
}

void printLine(int x, int y, char* str, ConsoleColor tColor, ConsoleColor bColor)
{
	COORD pos = { x, y };
	DWORD dw;
	if (x < 0)
	{
		if (x == _ALIGN_CENTER_)
		{
			pos.X = (ScreenWidth - strlen(str)) / 2;
		}
		else
		{
			if (x == _ALIGN_LEFT_)
			{
				pos.X = 0;
			}
			else
			{
				if (x == _ALIGN_RIGHT_)
					pos.X = ScreenWidth - strlen(str) - 1;
				
			}
		}
	}
	if (y < 0)
	{
		if (y == _ALIGN_CENTER_)
		{
			pos.Y = (ScreenHeight - 1) / 2;
		}
		else
		{
			if (y == _ALIGN_TOP_)
			{
				pos.Y = 0;
			}
			else
			{
				if (y == _ALIGN_BOTTOM_)
					pos.Y = ScreenHeight - 1;
				
			}
		}
	}
	SetConsoleTextAttribute(ScreenHandle[ScreenIndex], tColor | (bColor << 4));
	SetConsoleCursorPosition(ScreenHandle[ScreenIndex], pos);
	WriteFile(ScreenHandle[ScreenIndex], str, strlen(str), &dw, NULL);
}

void printLines(int x, int y, char *str, ConsoleColor tColor, ConsoleColor bColor)
{
	COORD pos = { x, y };
	DWORD dw;
	char *beg, *end;
	int len;
	
	SetConsoleTextAttribute(ScreenHandle[ScreenIndex], tColor | (bColor << 4));
	end = str;
	beg = end;
	while (*end)
	{
		if (*end != '\n')
		{
			end++;
			continue;
		}
		// print this line
		len = end - beg;
		memcpy(lineBuffer, beg, sizeof(char) * len);
		lineBuffer[len] = '\0';
		SetConsoleCursorPosition(ScreenHandle[ScreenIndex], pos);
		WriteFile(ScreenHandle[ScreenIndex], lineBuffer, strlen(lineBuffer), &dw, NULL);
		pos.Y++;
		end++;
		beg = end;
	}
	if (*beg)
	{
		len = end - beg;
		memcpy(lineBuffer, beg, sizeof(char) * len);
		lineBuffer[len] = '\0';
		SetConsoleCursorPosition(ScreenHandle[ScreenIndex], pos);
		WriteFile(ScreenHandle[ScreenIndex], lineBuffer, strlen(lineBuffer), &dw, NULL);
	}
}



/*  */
void setRenderInfo(RenderInfo *target, COORD pos, char *s, ConsoleColor tColor, ConsoleColor bColor)
{
	if (s != NULL)
		target->s = s;
	target->pos = pos;
	target->tColor = tColor;
	target->bColor = bColor;
}

void setRenderInfoAttr(RenderInfo *target, COORD pos, int w, int h, ConsoleColor tColor, ConsoleColor bColor)
{
	target->s = NULL;
	target->pos = pos;
	target->width = w;
	target->height = h;
	target->tColor = tColor;
	target->bColor = bColor;
}

void setRenderer(Renderer* renderer)
{
	sceneRenderer = renderer;
}

Renderer* getCurrentRenderer()
{
	return sceneRenderer;
}



/*  */
void printFPS()
{
	char fps_text[30], itoa_text[10];
	checkFPS();
	strcpy(fps_text, "FPS : ");
	itoa(FPS, itoa_text, 10);
	strcat(fps_text, itoa_text);
	printLine(ScreenWidth - 12, _ALIGN_TOP_, fps_text, _WHITE_, _BLACK_);
}

void beginRenderThread()
{
	bRenderThread = 1;
	hRenderThread = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)renderThread, NULL, 0, &pRenderThread);
}



/*  */
unsigned __stdcall renderThread()
{
	while (bRenderThread)
	{
		clearScreen();
		if (sceneRenderer != NULL)
			(*sceneRenderer)();
		else
			fillColorToScreen(_BLACK_);
		printFPS();
		flipScreen();
		waitForFrame();
	}
}



/*  */
void releaseScreen()
{
	bRenderThread = 0;
    WaitForSingleObject(hRenderThread, INFINITE); 	// wait until render thread closed
	CloseHandle(hRenderThread);
	CloseHandle(ScreenHandle[0]);
	CloseHandle(ScreenHandle[1]);
	free(lineBuffer);
	free(blankBuffer);
}