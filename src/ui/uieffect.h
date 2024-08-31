#ifndef __UIEFFECT__
#define __UIEFFECT__
#include <string.h>
#include <windows.h>
#include "../render/renderer.h"

void fadeIn(void (*renderer)(void));
void fadeOut(void (*renderer)(void));
#endif