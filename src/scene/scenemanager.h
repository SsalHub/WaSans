#ifndef __SCENEMANAGER__
#define __SCENEMANAGER__
#include "loadscene.h"
#include "../settings.h"

typedef enum SceneType
{
	_SCENE_NONE_		= -1,
	_SCENE_EXIT_GAME_	= 0,
	_SCENE_MAINMENU_,
	_SCENE_SANS_BATTLE_,
} SceneType;

static SceneType currentScene;
static void (*startSceneFunc)(void);
static void (*runSceneFunc)(void);

void initFirstScene();
void gotoNextScene(SceneType scene);
ExitCode runScene();
void setCurrentScene(SceneType scene);
SceneType getCurrentScene();
#endif