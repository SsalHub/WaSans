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
}

void runGame()
{
	showMainmenu();
}

void exitGame()
{
	releaseScreen();
}