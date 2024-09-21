#ifndef __SCENEMANAGER__
#define __SCENEMANAGER__
#include "loader/sceneloader.h"
#include "../../settings.h"
#include "../../render/renderer.h"

typedef enum SCENE_TYPE
{
	_SCENE_NONE_		= -1,
	_SCENE_EXIT_GAME_	= 0,
	_SCENE_MAINMENU_,
	_SCENE_SANS_BATTLE_,
} SCENE_TYPE;

static SCENE_TYPE currentScene;
static void (*startSceneFunc)(void);
static void (*runSceneFunc)(void);

void initFirstScene();
void gotoNextScene(SCENE_TYPE scene);
EXITCODE runScene();
void setCurrentScene(SCENE_TYPE scene);
SCENE_TYPE getCurrentScene();
RENDERER* getSceneRenderer(SCENE_TYPE scene);
#endif