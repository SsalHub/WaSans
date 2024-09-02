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
	OldTime = clock();
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
}

void fillColorToScreen(ConsoleColor bColor, ConsoleColor tColor)
{
	COORD pos = { 0, 0 };
	DWORD dw;
	char buffer[(ScreenWidth * 2) * ScreenHeight + 1];
	int i, j;
	
	SetConsoleCursorPosition(ScreenHandle[ScreenIndex], pos);
	SetConsoleTextAttribute(ScreenHandle[ScreenIndex], tColor | (bColor << 4));
	FillConsoleOutputCharacter(ScreenHandle[ScreenIndex], ' ', ScreenWidth * ScreenHeight, pos, &dw);
//	buffer[0] = '\0';
//	for (i = 0; i < ScreenHeight; i++)
//	{
//		for (j = 0; j < ScreenWidth; j++)
//		{	
//			strcat(buffer, " ");
//		}
//		strcat(buffer, "\n");
//	}
//	WriteFile(ScreenHandle[ScreenIndex], buffer, strlen(buffer), &dw, NULL);
}

void releaseScreen()
{
	CloseHandle(ScreenHandle[0]);
	CloseHandle(ScreenHandle[1]);
}

void printLine(int x, int y, char* str, ConsoleColor tColor)
{
	COORD pos;
	DWORD dw;
	
	switch (x)
	{
		case _ALIGN_CENTER_:
			pos.X = (ScreenWidth - strlen(str)) * 0.5f;
			break;
		case _ALIGN_LEFT_:
			pos.X = 0;
			break;
		case _ALIGN_RIGHT_:
			pos.X = ScreenWidth - strlen(str) - 1;
			break;
		default:
			pos.X = x;
			break;
	}
	switch (y)
	{
		case _ALIGN_CENTER_:
			pos.Y = (ScreenHeight - 1) * 0.5f;
			break;
		case _ALIGN_TOP_:
			pos.Y = 0;
			break;
		case _ALIGN_BOTTOM_:
			pos.Y = ScreenHeight - 1;
			break;
		default:
			pos.Y = y;
			break;
	}
	SetConsoleTextAttribute(ScreenHandle[ScreenIndex], tColor | (_BLACK_ << 4));
	SetConsoleCursorPosition(ScreenHandle[ScreenIndex], pos);
	WriteFile(ScreenHandle[ScreenIndex], str, strlen(str), &dw, NULL);
}

void printLines(int x, int y, char* str, ConsoleColor tColor)
{
	COORD pos;
	DWORD dw;
	char* token, *copy;
	
	copy = (char*)malloc(sizeof(char) * (strlen(str) + 1));
	strcpy(copy, str);
	
	SetConsoleTextAttribute(ScreenHandle[ScreenIndex], tColor | (_BLACK_ << 4));
	token = strtok(copy, "\n");
	switch (x)
	{
		case _ALIGN_CENTER_:
			pos.X = (ScreenWidth - strlen(token)) * 0.5f;
			break;
		case _ALIGN_LEFT_:
			pos.X = 0;
			break;
		case _ALIGN_RIGHT_:
			pos.X = ScreenWidth - strlen(token) - 8;
			break;
		default:
			pos.X = x;
			break;
	}
	switch (y)
	{
		case _ALIGN_CENTER_:
			pos.Y = (ScreenHeight - 1) * 0.5f;
			break;
		case _ALIGN_TOP_:
			pos.Y = 0;
			break;
		case _ALIGN_BOTTOM_:
			// should count num of \n in 'str', but bothering
			pos.Y = 15;
			break;
		default:
			pos.Y = y;
			break;
	}
	while (token != NULL)
	{
		SetConsoleCursorPosition(ScreenHandle[ScreenIndex], pos);
		WriteFile(ScreenHandle[ScreenIndex], token, strlen(token), &dw, NULL);
		pos.Y++;
		token = strtok(NULL, "\n");
	}
	free(copy);
}

void renderScreen()
{
	clearScreen();
	printFrameInfo();
	flipScreen();
	setFrameSpeed();
}

void renderCustomScreen(void (*customRenderer)(void))
{
	clearScreen();
	(*customRenderer)();
	printFrameInfo();
	flipScreen();
	setFrameSpeed();
}

void printFrameInfo()
{
	char fps_info[30], fps_itoa[10];
	strcpy(fps_info, "FPS : ");
	if (0 < OldFPS)
		itoa(OldFPS, fps_itoa, 10);
		
	CurrTime = clock();
	if (1000 <= CurrTime - OldTime)
	{
		itoa(FPS, fps_itoa, 10);
		OldTime = CurrTime;
		OldFPS = FPS;
		FPS = 0;
	}
	strcat(fps_info, fps_itoa);
	FPS++;
	printLine(ScreenWidth - 12, _ALIGN_TOP_, fps_info, _WHITE_);
}

void setFrameSpeed()
{
	Sleep(1000 / BaseFrame);	
}