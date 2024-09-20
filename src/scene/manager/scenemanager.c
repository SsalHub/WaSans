#include "scenemanager.h"

void initFirstScene()
{
	currentScene = _SCENE_MAINMENU_;
}

void gotoNextScene(SCENE_TYPE scene)
{
	currentScene = scene;
	switch (scene)
	{
		case _SCENE_MAINMENU_:
			loadScene_Mainmenu(&startSceneFunc, &runSceneFunc);
			break;
		case _SCENE_SANS_BATTLE_:
			loadScene_SansBattle(&startSceneFunc, &runSceneFunc);
			break;
		case _SCENE_EXIT_GAME_:
			loadScene_ExitGame(&startSceneFunc, &runSceneFunc);
			break;
	}
}

EXITCODE runScene()
{
	startSceneFunc();
	runSceneFunc();
	
	if (currentScene == _SCENE_EXIT_GAME_)
		return _EXIT_GAME_;
	return _EXIT_NONE_;
}

void setCurrentScene(SCENE_TYPE scene)
{
	currentScene = scene;
}

SCENE_TYPE getCurrentScene()
{
	return currentScene;
}

RENDERER* getSceneRenderer(SCENE_TYPE scene)
{
	switch (scene)
	{
		case _SCENE_MAINMENU_:
			return renderMainmenu;
		case _SCENE_SANS_BATTLE_:
			return renderBattleScene;
		case _SCENE_EXIT_GAME_:
			return NULL;
	}
	return NULL;
}