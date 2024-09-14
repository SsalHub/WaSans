#include "scenemanager.h"

void initFirstScene()
{
	currentScene = _SCENE_MAINMENU_;
}

void gotoNextScene(SceneType scene)
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

ExitCode runScene()
{
	startSceneFunc();
	runSceneFunc();
	
	if (currentScene == _SCENE_EXIT_GAME_)
		return _EXIT_GAME_;
	return _EXIT_NONE_;
}

void setCurrentScene(SceneType scene)
{
	currentScene = scene;
}

SceneType getCurrentScene()
{
	return currentScene;
}

Renderer* getSceneRenderer(SceneType scene)
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