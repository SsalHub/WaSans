#ifndef __UIEFFECT__
#define __UIEFFECT__
#include <string.h>
#include <windows.h>
#include "../render/renderer.h"

static int fadeProgress;
static char* buffer;
static void (*renderer)(void);

void fadeOut(void (*func)(void));
void fadeIn(void (*func)(void));
void renderFadeEffect();
void blackScreenEffect(float t);
#endif