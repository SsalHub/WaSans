#ifndef __SANS_BATTLE__
#define __SANS_BATTLE__
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "../utils.h"
#include "../settings.h"
#include "../ui/uieffect.h"
#include "../render/renderer.h"
#include "../sound/sounds.h"

#define _SANS_SCRIPT_LEN_ 4

static int battleTurn, battleSelect, playerHP;
static COORD playerPos;
static int scriptIdx;
static const char scripts[_SANS_SCRIPT_LEN_][64] = {
    "it's a beautiful day outside.",
    "birds are singing. flowers are blooming...",
    "on days like these, kids like you...",
    "Should be burning in hell.",
};

void runSansBattle();
void introPhase();
void bossPhase();
void playerPhase();

void renderSansBattle();
void renderIntroPhase();
void renderBossPhase();
void renderPlayerPhase();

void renderSans(AssetFileType face);
int renderSpeechBubble(const char* script);
void renderBossPhaseBox();
void renderPlayerPhaseBox();
void renderPlayerInfo();
void renderSelectBox();
#endif