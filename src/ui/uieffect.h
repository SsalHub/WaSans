#ifndef __UIEFFECT__
#define __UIEFFECT__
#include <string.h>
#include <windows.h>
#include "../render/renderer.h"
#include "../scene/scenemanager.h"


static RENDERER* currentRenderer;
static int fadeProgress;
static char* buffer;


void fadeOut();
void fadeIn(SCENE_TYPE scene);
void renderFadeEffect();
void blackScreenEffect(float t);
void renderBlackScreenEffect();
#endif