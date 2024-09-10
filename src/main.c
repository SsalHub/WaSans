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
	int currentScene;
	
	beginRenderThread();
	currentScene = _SCENE_MAINMENU_;
	while (currentScene != _SCENE_EXIT_)
	{
		// mainmenu
		if (currentScene == _SCENE_MAINMENU_)
		{
			initMainMenu();
			currentScene = runMainmenu();
		}
		// sans battle
		if (currentScene == _SCENE_SANS_BATTLE_)
		{
			initSansBattle();
			currentScene = runSansBattle();
		}
		waitForFrame();
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