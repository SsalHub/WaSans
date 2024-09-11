#include "gotoscene.h"

void gotoMainmenuScene()
{
	gotoScene(_SCENE_MAINMENU_, initMainMenu, runMainmenu);
}

void gotoSansBattleScene()
{
	gotoScene(_SCENE_SANS_BATTLE_, initSansBattle, runSansBattle);
}

void gotoExitGame()
{
	setCurrentScene(_SCENE_EXIT_GAME_);
}

void initSceneInfo(SceneType scene)
{
	switch (scene)
	{
		case _SCENE_MAINMENU_:
			gotoMainmenuScene();
			break;
		case _SCENE_SANS_BATTLE_:
			gotoSansBattleScene();
			break;
	}
}