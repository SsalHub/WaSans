#ifndef __BATTLE__
#define __BATTLE__
#include <Windows.h>
#include <conio.h>
#include "../../settings.h"
#include "../../utils.h"
#include "../../render/renderer.h"

#define _PATTERN_LAYER_LEN_		3


typedef unsigned __stdcall (*Pattern)(void*);
typedef void Collider(void);

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
	COORD pos;
	char *data;
	ConsoleColor tColor;
	ConsoleColor bColor;
	int width;
	int height;
	int isActive;
	int HP;
	int mode;
} BattleObject;


typedef struct PatternInfo
{
	// basic pattern info
	Pattern pattern;
	void* data;
	// thread info
	HANDLE hThread;
	DWORD isActive;
	unsigned int threadID;
	// render info
	int renderInfoLen[_PATTERN_LAYER_LEN_];
	RenderInfo renderInfo[_PATTERN_LAYER_LEN_][30];
	Collider *collider;
} PatternInfo;

extern int MaxHP;
extern BattleObject **EnemyInfo, *SpeechBubble;
extern BattleObject Player, EnemyPhaseBox, PlayerPhaseBox;
extern PatternInfo *Patterns;


static int battleTurn, battleSelect, battlePhase;
static int enemyLen, patternLen;


/* Main Renderer */
void renderBattleScene();

/* Init Functions */
void initBattle(int elen, BattleObject (*enemy)[3], int plen, PatternInfo* pattern);
void initPlayer();
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
int 		movePlayerSelectBox();
int 		getPlayerDamage(int damage);
/* Collider */
COORD* 		getCollision(COORD *src, COORD *trg);
COORD* 		getCollisionInRange(COORD *src, COORD *begin, COORD *end);

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