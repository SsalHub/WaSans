#include "main.h"

int main(int argc, char *argv[]) {
	initScreen();
	
	while ()
	{
		if (kbhit())
			break;
		renderScreen();
//		sleep(0.1f);
	}
	
	releaseScreen();
	
	return 0;
}