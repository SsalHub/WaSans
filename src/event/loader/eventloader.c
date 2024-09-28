#include "eventloader.h"

#include "../../render/renderer.h"
#include "../../scene/battle/battlemanager.h"
#include "../../scene/battle/sans.h"
#include "../../scene/mainmenu.h"
#include "../../scene/scenemanager.h"
#include "../../ui/uieffect.h"


/* Init, Scene Events */
void onStartGame()
{
	gotoNextScene(_SCENE_MAINMENU_);
	beginRenderThread();
}



/* Battle Events */
void onDamaged()
{
	
}