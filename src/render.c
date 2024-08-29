#include "render.h"

int ScreenIndex;
HANDLE ScreenHandle[2];

void initScreen()
{
//	char command[50];
//	sprintf(command, "mode con:cols=%d lines=%d", ScreenWidth, ScreenHeight);
//	system(command);
	
	CONSOLE_CURSOR_INFO cinfo;
	
	ScreenIndex = 0;
	ScreenHandle[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	ScreenHandle[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	
	cinfo.dwSize = 1;
	cinfo.bVisible = FALSE;
	SetConsoleCursorInfo(ScreenHandle[0], &cinfo);
	SetConsoleCursorInfo(ScreenHandle[1], &cinfo);
	
	setWindowInfo(ScreenWidth, ScreenHeight);
}

void setWindowInfo(int w, int h)
{
	SMALL_RECT rect;
	rect.Left = 0;
	rect.Top = 0;
	rect.Right = w;
	rect.Bottom = h;
	SetConsoleWindowInfo(ScreenHandle[0], FALSE, &rect);
	SetConsoleWindowInfo(ScreenHandle[1], FALSE, &rect);
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
}

void releaseScreen()
{
	CloseHandle(ScreenHandle[0]);
	CloseHandle(ScreenHandle[1]);
}

void printScreen(int x, int y, char* str)
{
	COORD pos = { x, y };
	DWORD dw;
	SetConsoleCursorPosition(ScreenHandle[ScreenIndex], pos);
	WriteFile(ScreenHandle[ScreenIndex], str, strlen(str), &dw, NULL);
}

void renderScreen()
{
	clearScreen();
	
	char fps_info[30];
	if (1000 <= CurrTime - OldTime)
	{
		sprintf(fps_info, "FPS : %d", FPS);
		OldTime = CurrTime;
		FPS = 0;
	}
	
	FPS++;
	printScreen(2, 1, fps_info);
	flipScreen();
}