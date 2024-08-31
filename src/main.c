#include "main.h"

int main(int argc, char *argv[]) {
	initGame();
	runGame();
	exitGame();
	return 0;
}

void initGame()
{
	initScreen();
	initDataFile();
}

void runGame()
{
	int bExit;
	while (1)
	{
		bExit = showMainmenu();
		if (0 <= bExit)
		{
			if (bExit == 1) // exit game
				break;
		}
		else // error
		{
			perror("Mainmenu selection error");
			exit(1);
		}
	}
}

void exitGame()
{
	// print game exit screen
	// printLine(_ALIGN_CENTER_, _ALIGN_TOP_, str, _WHITE_);
	
	releaseScreen();
	releaseDataFile();
}