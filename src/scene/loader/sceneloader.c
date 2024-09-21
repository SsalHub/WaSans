#include "sceneloader.h"

#include "../mainmenu.h"
#include "../battle/battlemanager.h"
#include "../battle/sans.h"
#include "../../render/renderer.h"

void loadScene_Mainmenu(StartScene* start, RunScene* run)
{
	(*start) = Mainmenu_Start;
	(*run) = Mainmenu_Update;
//	setSceneRenderer(renderMainmenu);
}

RENDERER* getSceneRenderer_Mainmenu()
{
	return renderMainmenu;
}

void loadScene_Battle_Sans(StartScene* start, RunScene* run)
{
	(*start) = Sans_Start;
	(*run) = Sans_Update;
//	setSceneRenderer(renderBattleScene);
}

RENDERER* getSceneRenderer_Battle_Sans()
{
	return renderBattleScene;
}
