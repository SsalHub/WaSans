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
	int mainmenu;
	while (1)
	{
		mainmenu = showMainmenu();
		if (mainmenu == 0)
		{
			// run boss battle
			runSansBattle();
		}
		else if(mainmenu == 1)
		{
			// exit game
			break;
		}
		else
		{
			// error
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