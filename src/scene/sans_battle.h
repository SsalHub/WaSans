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

#define _SANS_SCRIPT_LEN_ 	4
#define _BLAST_ANGLE_LEN_ 	7
#define _SANS_PATTERN_LEN_ 	2

typedef enum BlastAngle
{
	_BLAST_TOP_CENTER_		= 0,	// vertical
	_BLAST_TOP_RIGHT_		= 45,	// diagonal
	
	_BLAST_MID_RIGHT_		= 90,
	_BLAST_BOT_RIGHT_		= 135,
	
	_BLAST_BOT_CENTER_		= 180,
	_BLAST_BOT_LEFT_		= 225,
	
	_BLAST_TOP_LEFT_		= 270,
	_BLAST_MID_LEFT_		= 315,
//	_BLAST_MID_CENTER_,
} BlastAngle;

typedef struct BossPhaseBox
{
	int x;
	int y;
	int w;
	int h;
} BossPhaseBox;

static int battleTurn, battleSelect, playerHP, patternIdx, scriptIdx;
static COORD playerPos;
static const char scripts[_SANS_SCRIPT_LEN_][64] = {
				    "it's a beautiful day outside.",
				    "birds are singing. flowers are blooming.",
				    "on days like these, kids like you...",
				    "Should be burning in hell.",
				};
static const BossPhaseBox bossPhaseBox = { 49, 16, 18, 8 };
static Pattern sansPattern[_SANS_PATTERN_LEN_];

/* Main func in sans battle */
void runSansBattle();
void initSansPattern();

/* Each Phase Func */
void introPhase();
void bossPhase();
void playerPhase();

/* Main Renderer */
void renderSansBattle();
void renderIntroPhase();
void renderBossPhase();
void renderPlayerPhase();

/* Sub Renderer */
void renderSans(AssetFileType face);
int renderSpeechBubble(const char* script, ConsoleColor tColor, int bVoice);
void renderBossPhaseBox();
void renderPlayerPhaseBox();
void renderPlayerInfo();
void renderSelectBox();
void renderPattern();

/* Boss Phase func */
void movePlayer();
unsigned __stdcall fireBlastToCenter(void* arg);
void fireBlastToPlayer(BlastAngle blastAngle);
AssetFileType getBlastType(BlastAngle blastAngle);
char* fixBlastAngle(char* dst, size_t dstSize, BlastAngle blastAngle);
int getLastPatternIdx();
void releasePattern();
#endif