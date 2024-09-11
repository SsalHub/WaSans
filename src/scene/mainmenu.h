#ifndef __MAINMENU__
#define __MAINMENU__
#include <conio.h>
#include <string.h>
#include <windows.h>
#include "scenemanager.h"
#include "gotoscene.h"
#include "../utils.h"
#include "../settings.h"
#include "../ui/uieffect.h"
#include "../render/renderer.h"
#include "../sound/sounds.h"

static int menuSelect;

//void Start();
//void Update();
void initMainMenu();
void runMainmenu();
void renderMainmenu();
#endif