#include "scenemanager.h"

void gotoScene(SceneType scene, Start startFunc, Update updateFunc)
{
	currentScene = scene;
	start = startFunc;
	update = updateFunc;
}

ExitCode runScene()
{
	SceneType lastScene = currentScene;
	(*start)();
	while (lastScene == currentScene)
	{
		lastScene = currentScene;
		(*update)();
		waitForFrame();
	}
	
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

void initFirstScene()
{
	currentScene = _SCENE_MAINMENU_;
}