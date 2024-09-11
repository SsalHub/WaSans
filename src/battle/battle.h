#ifndef __BATTLE__
#define __BATTLE__
#include "../settings.h"
#include "../utils.h"
#include "../render/renderer.h"

#define _ENEMY_LEG_		0
#define _ENEMY_BODY_	1
#define _ENEMY_FACE_	2

typedef unsigned __stdcall (*Pattern)(void*);

typedef enum BattlePhase
{
	_INTRO_PHASE_	= -1,
	_ENEMY_PHASE_	= 0,
	_PLAYER_PHASE_,
} BattlePhase;

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
extern BattleObject (*EnemyInfo)[3];
extern BattleObject EnemyPhaseBox, PlayerPhaseBox, *SpeechBubble;


static int battleTurn, battleSelect, battlePhase, playerHP;
static int enemyLen;


/* Init Functions */
void initBattle(int len, BattleObject enemy[][3]);
void initEnemyPhaseBox();
void initPlayerPhaseBox();
void initEnemyInfo(int len, BattleObject enemy[][3]);
void initSpeechBubble();

/* Util Func */
void setBattlePhase(BattlePhase phase);
BattlePhase getBattlePhase();
void gotoNextPhase();
HANDLE startPattern(Pattern pattern, void* args, unsigned int* threadID);

/* Main Renderer */
void renderBattleScene();

/* Sub Renderer */
void renderEnemy();
void renderEnemyPhaseBox();
void renderPlayerPos();
void renderPlayerPhaseBox();
void renderPlayerInfo();
void renderSelectBox();
void renderSpeechBubble();

/* Terminate Func */
void releaseBattleAssets();
#endif