#ifndef __SCENELOADER__
#define __SCENELOADER__

#include "../../render/renderer.h"


typedef void 	(*StartScene)(void);
typedef void 	(*RunScene)(void);


void loadScene_Mainmenu(StartScene* start, RunScene* run);
RENDERER* getSceneRenderer_Mainmenu();
void loadScene_Battle_Sans(StartScene* start, RunScene* run);
RENDERER* getSceneRenderer_Battle_Sans();
#endif