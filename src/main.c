#include "main.h"

int main(int argc, char *argv[]) {
	initGame();
	runGame();
	exitGame();
	return 0;
}

void initGame()
{
	// init basic info
	initPlayerName("SJW");
	PlayerLevel = 1;
	// load assets
	initDataAsset();
	initBGMAsset();
	initVoiceAsset();
	// init screen renderer
	initScreen();
	initFirstScene();
	beginRenderThread();
}

void runGame()
{
	ExitCode ec = _EXIT_NONE_;
	gotoNextScene(getCurrentScene());
	while (ec != _EXIT_GAME_)
	{
		ec = runScene();
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
	releasePlayerName();
}