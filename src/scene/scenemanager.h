#ifndef __SCENEMANAGER__
#define __SCENEMANAGER__
#include "../settings.h"

typedef enum SceneType
{
	_SCENE_EXIT_GAME_	= 0,
	_SCENE_MAINMENU_,
	_SCENE_SANS_BATTLE_,
} SceneType;

typedef void (*Start)();
typedef void (*Update)();

static Start start;
static Update update;
static SceneType currentScene;

void gotoScene(SceneType scene, Start startFunc, Update updateFunc);
ExitCode runScene();
void setCurrentScene(SceneType scene);
SceneType getCurrentScene();
void initFirstScene();
#endif