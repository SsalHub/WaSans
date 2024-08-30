#include "renderer.h"

int ScreenIndex;
HANDLE ScreenHandle[2];

void initScreen()
{
	
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
	char buffer[(ScreenWidth * 2) * ScreenHeight + 1];
	int i, j;
	
	SetConsoleCursorPosition(ScreenHandle[ScreenIndex], pos);
	SetConsoleTextAttribute(ScreenHandle[ScreenIndex], _WHITE_ | (_BLACK_ << 4));
	buffer[0] = '\0';
	for (i = 0; i < ScreenHeight; i++)
	{
		for (j = 0; j < ScreenWidth; j++)
		{	
			strcat(buffer, " ");
		}
		strcat(buffer, "\n");
	}
	WriteFile(ScreenHandle[ScreenIndex], buffer, strlen(buffer), &dw, NULL);
}

void fillColorToScreen(ConsoleColor bColor, ConsoleColor tColor)
{
	COORD pos = { 0, 0 };
	DWORD dw;
	char buffer[(ScreenWidth * 2) * ScreenHeight + 1];
	int i, j;
	
	SetConsoleCursorPosition(ScreenHandle[ScreenIndex], pos);
	SetConsoleTextAttribute(ScreenHandle[ScreenIndex], tColor | (bColor << 4));
	buffer[0] = '\0';
	for (i = 0; i < ScreenHeight; i++)
	{
		for (j = 0; j < ScreenWidth; j++)
		{	
			strcat(buffer, " ");
		}
		strcat(buffer, "\n");
	}
	WriteFile(ScreenHandle[ScreenIndex], buffer, strlen(buffer), &dw, NULL);
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
	
	char fps_info[30];
	if (1000 <= CurrTime - OldTime)
	{
		clearScreen();
		sprintf(fps_info, "FPS : %d", FPS);
		OldTime = CurrTime;
		FPS = 0;
	}
	
	FPS++;
	printScreen(ScreenWidth - 15, ScreenHeight - 1, fps_info);
	flipScreen();
}