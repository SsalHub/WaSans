#include "sceneloader.h"


void loadScene_Mainmenu(StartScene* start, RunScene* run)
{
	(*start) = initMainMenu;
	(*run) = runMainmenu;
//	setSceneRenderer(renderMainmenu);
}

void loadScene_SansBattle(StartScene* start, RunScene* run)
{
	(*start) = initSansBattle;
	(*run) = runSansBattle;
//	setSceneRenderer(renderBattleScene);
}

void loadScene_ExitGame(StartScene* start, RunScene* run)
{
	(*start) = NULL;
	(*run) = NULL;
//	setSceneRenderer(NULL);
}
