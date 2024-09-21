#include "main.h"


int main(int argc, char *argv[]) {
	initGame();
	runGame();
	exitGame();
	return 0;
}

void initGame()
{
	// init basic info and events
	initPlayerName("SJW");
	PlayerLevel = 1;
	
	// load assets
	initDataAsset();
	initBGMAsset();
	initVoiceAsset();
	
	// init event listener
	initEventListener();
	// init screen renderer
	initScreen();
	beginRenderThread();
}

void runGame()
{
//	onEvent(_EVENT_GAME_START_);
	gotoNextScene(_SCENE_MAINMENU_);
	
	EXITCODE ec = _EXIT_NONE_;
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