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
	initDataAsset();
	initBGMAsset();
	initVoiceAsset();
}

void runGame()
{
	int bExit;
	
	bRenderThread = 1;
	renderThread = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)beginRenderThread, NULL, 0, &pRenderThread);
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
			exitGame();
			exit(1);
		}
	}
}

void exitGame()
{
	// print game exit screen
	// printLine(_ALIGN_CENTER_, _ALIGN_TOP_, str, _WHITE_);
	
	releaseScreen();
	releaseAssetFile();
//	releaseBGMAsset();
//	releaseVoiceAsset();
	releaseSoundAssets();
}