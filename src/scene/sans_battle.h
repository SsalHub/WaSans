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
#define _BLAST_ANGLE_LEN_ 7

typedef enum BlastAngle
{
	_BLAST_TOP_LEFT_		= -100,
	_BLAST_TOP_CENTER_,
	_BLAST_TOP_RIGHT_,
	_BLAST_MID_LEFT_,
//	_BLAST_MID_CENTER_,
	_BLAST_MID_RIGHT_,
	_BLAST_BOT_LEFT_,
	_BLAST_BOT_CENTER_,
	_BLAST_BOT_RIGHT_,
} BlastAngle;

typedef struct BossPhaseBox
{
	int x;
	int y;
	int w;
	int h;
} BossPhaseBox;

static int battleTurn, battleSelect, playerHP;
static COORD playerPos;
static int scriptIdx;
static const char scripts[_SANS_SCRIPT_LEN_][64] = {
				    "it's a beautiful day outside.",
				    "birds are singing. flowers are blooming.",
				    "on days like these, kids like you...",
				    "Should be burning in hell.",
				};
static const BossPhaseBox bossPhaseBox = { 49, 16, 18, 8 };

void runSansBattle();
void introPhase();
void bossPhase();
void playerPhase();

void renderSansBattle();
void renderIntroPhase();
void renderBossPhase();
void renderPlayerPhase();

void renderSans(AssetFileType face);
int renderSpeechBubble(const char* script, ConsoleColor tColor);
void renderBossPhaseBox();
void renderPlayerPhaseBox();
void renderPlayerInfo();
void renderSelectBox();

void movePlayer();
void fireBlastToCenter(BlastType blastX, BlastType blastY);
void fireBlastToPlayer(int blastX, int blastY);
AssetFileType getBlastType(BlastAngle blastAngle);
void fixBlastAngle(char* target, BlastAngle blastAngle, AssetFileType blastType);
#endif