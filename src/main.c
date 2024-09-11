#include "main.h"

int main(int argc, char *argv[]) {
	initGame();
	runGame();
	exitGame();
	return 0;
}

void initGame()
{
	initPlayerName("SJW");
	PlayerLevel = 1;
	initScreen();
	initFirstScene();
	// load assets
	initDataAsset();
	initBGMAsset();
	initVoiceAsset();
	// begin multi renderer thread
	beginRenderThread();
}

void runGame()
{
	ExitCode ec = _EXIT_NONE_;
	while (ec != _EXIT_GAME_)
	{
		initSceneInfo(getCurrentScene());
		ec = runScene();
	}
}

void exitGame()
{
	// print game exit screen
	// printLine(_ALIGN_CENTER_, _ALIGN_TOP_, str, _WHITE_);
	
	releaseScreen();
	releasePlayerName();
	releaseAssetFile();
//	releaseBGMAsset();
//	releaseVoiceAsset();
	releaseSoundAssets();
}