#include "main.h"

int main(int argc, char *argv[]) {
	initScreen();
	
	OldTime = clock();
	
	while (1)
	{
		if (kbhit())
			break;
		CurrTime = clock();
		renderScreen();
		sleep(0.1f);
	}
	
	releaseScreen();
	
	return 0;
}