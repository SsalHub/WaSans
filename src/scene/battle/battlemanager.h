#ifndef __BATTLEMANAGER__
#define __BATTLEMANAGER__
#include <Windows.h>
#include <conio.h>
#include "../../settings.h"
#include "../../utils.h"
#include "../../render/renderer.h"
#include "../../event/eventmanager.h"


#define _PATTERN_LAYER_LEN_		3

typedef unsigned __stdcall (*BATTLE_PATTERN)(void*);
typedef void COLLIDER(void*);

typedef enum BATTLE_PHASE
{
	_INTRO_PHASE_	= -1,
	_PLAYER_PHASE_	= 0,
	_ENEMY_PHASE_,
} BATTLE_PHASE;

typedef enum ENEMY_INFO
{
	_ENEMY_LEG_		= 0,
	_ENEMY_BODY_, 
	_ENEMY_FACE_, 
} ENEMY_INFO;

typedef enum PLAYER_STATUS
{
	_PLAYER_DIED_ 	= 0,
	_PLAYER_ALIVE_,
	_PLAYER_IMMUNE_,
	_PLAYER_SANS_DOT_,
} PLAYER_STATUS;

typedef struct BATTLE_OBJECT
{
	COORD pos;
	char *data;
	CONSOLE_COLOR tColor;
	CONSOLE_COLOR bColor;
	int width;
	int height;
	int isActive;
	int HP;
	int mode;
} BATTLE_OBJECT;

typedef struct PATTERN_INFO
{
	// basic pattern info
	BATTLE_PATTERN pattern;
	void* data;
	// thread info
	HANDLE hThread;
	DWORD isActive;
	unsigned int threadID;
	// render info
	int renderInfoLen[_PATTERN_LAYER_LEN_];
	RENDER_INFO renderInfo[_PATTERN_LAYER_LEN_][30];
	COLLIDER *collider;
} PATTERN_INFO;

typedef struct COLLISION_INFO
{
	COORD pos;
	BATTLE_PATTERN pattern;	
	CONSOLE_COLOR tColor;
	CONSOLE_COLOR bColor;
} COLLISION_INFO;


extern int MaxHP;
extern BATTLE_OBJECT **EnemyInfo, *SpeechBubble;
extern BATTLE_OBJECT Player, EnemyPhaseBox, PlayerPhaseBox;
extern PATTERN_INFO *Patterns;


static int battleTurn, battleSelect, battlePhase;
static int enemyLen, patternLen;


/* Main Renderer */
void renderBattleScene();

/* Init Functions */
void initBattle(int elen, BATTLE_OBJECT (*enemy)[3], int plen, PATTERN_INFO *pattern);
void initPlayer();
void initEnemyPhaseBox();
void initPlayerPhaseBox();
void initEnemyInfo(int len, BATTLE_OBJECT (*enemy)[3]);
void initPatternInfo(int plen, PATTERN_INFO *pattern);

/* Util Func */
void 		setBattlePhase(BATTLE_PHASE phase);
void 		gotoNextPhase();
void 		gotoNextTurn();
BATTLE_PHASE getBattlePhase();
int 		getBattleTurn();
HANDLE 		startPattern(BATTLE_PATTERN pattern, void *args, unsigned int *threadID);
int 		movePlayerSelectBox();
void checkPlayerInfo();
/* Collider */
COORD* 		checkCollision(COORD *src, COORD *trg);
COORD* 		checkCollisionInRange(COORD *src, COORD *begin, COORD *end);
void 		setCollisionInfo(COLLISION_INFO *target, COORD pos, BATTLE_PATTERN pattern, RENDER_INFO *render);

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