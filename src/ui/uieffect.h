#ifndef __UIEFFECT__
#define __UIEFFECT__
#include <string.h>
#include <windows.h>
#include "../render/renderer.h"

static RENDERER* currentRenderer;
static int fadeProgress;
static char* buffer;

void fadeOut();
void fadeIn(RENDERER* renderer);
void renderFadeEffect();
void blackScreenEffect(float t);
void renderBlackScreenEffect();
#endif