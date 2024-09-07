#ifndef __UIEFFECT__
#define __UIEFFECT__
#include <string.h>
#include <windows.h>
#include "../render/renderer.h"

void fadeOut(void (*renderer)(void));
void fadeIn(void (*renderer)(void));
void blackScreenEffect(float t);
#endif