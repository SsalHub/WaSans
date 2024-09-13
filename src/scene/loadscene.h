#ifndef __LOADSCENE__
#define __LOADSCENE__
#include "mainmenu.h"
#include "sans_battle.h"
#include "battle/battle.h"
#include "../render/renderer.h"

typedef void 	(*StartScene)(void);
typedef void 	(*RunScene)(void);

void loadScene_Mainmenu(StartScene* start, RunScene* run);
void loadScene_SansBattle(StartScene* start, RunScene* run);
void loadScene_ExitGame(StartScene* start, RunScene* run);
#endif