#ifndef __SANS_BATTLE__
#define __SANS_BATTLE__
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "manager/scenemanager.h"
#include "battle/battle.h"
#include "../utils.h"
#include "../settings.h"
#include "../ui/uieffect.h"
#include "../render/renderer.h"
#include "../sound/sounds.h"

#define _SANS_SCRIPT_LEN_ 	5
#define _SANS_PATTERN_LEN_ 	4
#define _BLAST_ANGLE_LEN_ 	6
#define _BATTLE_SELECT_LEN_	4
#define _PATTERN_CONTINUE_	-1

typedef enum BLASTER_ANGLE
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
} BLASTER_ANGLE;

enum SANS_ENEMYTYPE
{
	_ENEMY_SANS_ 			= 0,
};

typedef struct SANS_ARGS_BLASTER
{
	unsigned int patternId;
	BLASTER_ANGLE blasterAngle;
} SANS_ARGS_BLASTER;



static int patternIdx, scriptIdx;
static PatternInfo sansPattern[_SANS_PATTERN_LEN_];
static const char scripts[_SANS_SCRIPT_LEN_][128] = {
				    "it's a beautiful day outside.",
				    "birds are singing. flowers are blooming.",
				    "on days like these, kids like you...",
				    "Should be burning in hell.",
//				    "huh.",
//					"always wondered why people never use their strongest attack first.",
				    "here we go.",
				};
static SANS_ARGS_BLASTER gasterBlasterPatternInfo[_SANS_PATTERN_LEN_] = {
					{ 0, _BLAST_MID_RIGHT_ },
					{ 1, _BLAST_MID_LEFT_ },
					{ 2, _BLAST_TOP_CENTER_ },
					{ 3, _BLAST_BOT_CENTER_ },
				};

/* Main func in sans battle */
void initSansBattle();
void initSansObject(BattleObject (*enemy)[3]);
void initSansPattern();
void runSansBattle();

/* Each Phase Func */
static void introPhase();
static void playerPhase();
static void enemyPhase();

/* Boss Phase func */
unsigned __stdcall explodeBlasterToCenter(void* args);
unsigned __stdcall explodeBlasterToPlayer(void* args);
ASSET_TYPE getBlasterType(BLASTER_ANGLE blasterAngle);
char* fixBlasterAngle(char* dst, size_t dstSize, BLASTER_ANGLE blasterAngle);
int explodeBlaster(BLASTER_ANGLE angle, int pid, COORD begin, COORD end, CONSOLE_COLOR bColor);
void runSansPattern(int pid);
void runSansPatternInRange(int begin, int end);
int isAnyPatternAlive();
/* Pattern Collider */
void SansPatternCollider_Basic();
void SansPatternCollider_IsStop();

/* etc */
int writeSpeechBubble(unsigned int scriptIdx, CONSOLE_COLOR tColor, int bVoice);
void movePlayerPos();
void setSansFace(ASSET_TYPE facetype);

/* Terminate Func */
void releasePatternInRange(int begin, int end);
void releasePatterns();
#endif