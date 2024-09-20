#ifndef __MAINMENU__
#define __MAINMENU__
#include <conio.h>
#include <string.h>
#include <windows.h>
#include "manager/scenemanager.h"
#include "../utils.h"
#include "../settings.h"
#include "../ui/uieffect.h"
#include "../render/renderer.h"
#include "../sound/sounds.h"

static int menuSelect = 0;

void initMainMenu();
void runMainmenu();
void renderMainmenu();
#endif