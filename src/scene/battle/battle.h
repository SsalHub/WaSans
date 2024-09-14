#ifndef __BATTLE__
#define __BATTLE__
#include "../../settings.h"
#include "../../utils.h"
#include "../../render/renderer.h"


typedef unsigned __stdcall (*Pattern)(void*);

typedef enum BattlePhase
{
	_INTRO_PHASE_	= -1,
	_PLAYER_PHASE_	= 0,
	_ENEMY_PHASE_,
} BattlePhase;

typedef enum EnemyPart
{
	_ENEMY_LEG_		= 0,
	_ENEMY_BODY_, 
	_ENEMY_FACE_, 
} EnemyPart;

typedef struct BattleObject
{
	int x;
	int y;
	char* data;
	ConsoleColor tColor;
	ConsoleColor bColor;
	int width;
	int height;
	int isActive;
} BattleObject;


typedef struct PatternInfo
{
	Pattern pattern;
	int data;
	HANDLE hThread;
	DWORD isActive;
	unsigned int threadID;
	int renderInfoLen;
	RenderInfo renderInfo[10];
} PatternInfo;

extern int MaxHP;
extern COORD PlayerPos;
extern BattleObject **EnemyInfo, *SpeechBubble;
extern BattleObject EnemyPhaseBox, PlayerPhaseBox;
extern PatternInfo *Patterns;



static int battleTurn, battleSelect, battlePhase, playerHP;
static int enemyLen, patternLen;


/* Main Renderer */
void renderBattleScene();

/* Init Functions */
void initBattle(int elen, BattleObject (*enemy)[3], int plen, PatternInfo* pattern);
void initEnemyPhaseBox();
void initPlayerPhaseBox();
void initEnemyInfo(int len, BattleObject (*enemy)[3]);
void initPatternInfo(int plen, PatternInfo* pattern);

/* Util Func */
void 		setBattlePhase(BattlePhase phase);
void 		gotoNextPhase();
void 		gotoNextTurn();
BattlePhase getBattlePhase();
int 		getBattleTurn();
HANDLE 		startPattern(Pattern pattern, void* args, unsigned int* threadID);

/* Sub Renderer */
void renderEnemy();
void renderEnemyPhaseBox();
void renderPlayerPos();
void renderPlayerPhaseBox();
void renderPlayerInfo();
void renderSelectBox();
void renderSpeechBubble();
void renderPattern();

/* Terminate Func */
void releaseBattleAssets();
#endif