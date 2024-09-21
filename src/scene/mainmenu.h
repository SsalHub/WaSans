#ifndef __MAINMENU__
#define __MAINMENU__
#include <conio.h>
#include <string.h>
#include <windows.h>
#include "scenemanager.h"
#include "../utils.h"
#include "../settings.h"
#include "../ui/uieffect.h"
#include "../render/renderer.h"
#include "../sound/sounds.h"

static int menuSelect = 0;

void Mainmenu_Start();
void Mainmenu_Update();
void renderMainmenu();
#endif