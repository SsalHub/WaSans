#ifndef __SANS_BATTLE__
#define __SANS_BATTLE__
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "battlemanager.h"
#include "../scenemanager.h"
#include "../../utils.h"
#include "../../settings.h"
#include "../../ui/uieffect.h"
#include "../../render/renderer.h"
#include "../../sound/sounds.h"
#include "../../event/eventmanager.h"


#define _SANS_PATTERN_LEN_ 	11	// num of patterns
#define _SANS_SCRIPT_LEN_ 	5	// num of scripts

#define _BLAST_ANGLE_LEN_ 	6
#define _BATTLE_SELECT_LEN_	4
#define _PATTERN_CONTINUE_	-1


typedef enum BLASTER_POSITION
{
	// vertical
	_BLAST_TOP_LEFT_		= 330,
	_BLAST_TOP_CENTER_		= 0,
	_BLAST_TOP_RIGHT_		= 30,
	
	_BLAST_RIGHT_TOP_		= 60,
	_BLAST_RIGHT_CENTER_	= 90,
	_BLAST_RIGHT_BOT_		= 120,
	
	_BLAST_BOT_RIGHT_		= 150,
	_BLAST_BOT_CENTER_		= 180,
	_BLAST_BOT_LEFT_		= 210,
	
	_BLAST_LEFT_BOT_		= 240,
	_BLAST_LEFT_CENTER_		= 270,
	_BLAST_LEFT_TOP_		= 300,
	
	// diagonal
	_BLAST_EDGE_TOP_LEFT_	= 315,	
	_BLAST_EDGE_TOP_RIGHT_	= 45,
	_BLAST_EDGE_BOT_LEFT_	= 225,
	_BLAST_EDGE_BOT_RIGHT_	= 135,
} BLASTER_POSITION;

enum SANS_ENEMYTYPE
{
	_ENEMY_SANS_ 			= 0,
};

typedef struct SANS_PATTERN_ARGS
{
	unsigned int patternId;
	BATTLE_PATTERN_PTR pattern;
	int blasterPos;
	int direction;
} SANS_PATTERN_ARGS;


static int patternIdx, scriptIdx;
static PATTERN_INFO sansPattern[_SANS_PATTERN_LEN_];

/* Main func in sans battle */
void Sans_Start();
void Sans_Update();

/* init func */
void initSansObject(BATTLE_OBJECT (*enemy)[_ENEMY_INFO_LEN_]);
void initSansPattern();

/* Each Phase Func */
static void introPhase();
static void playerPhase();
static void enemyPhase();

/* Boss Phase func */
void Sans_runPattern(int pid);
void Sans_runPatternInRange(int begin, int end);
int Sans_runScript(int begin, int end, CONSOLE_COLOR tColor, unsigned int bVoice);
int isAnyPatternAlive();
ASSET_TYPE getBlasterType(BLASTER_POSITION blasterPos);
char* fixBlasterAngle(char *dst, size_t dstSize, BLASTER_POSITION blasterPos);
int explodeBlaster(BLASTER_POSITION angle, int pid, COORD begin, COORD end, CONSOLE_COLOR bColor);
void Sans_onDamaged(void *args);
void Sans_onHit(void *args);
void setGravityMode(INPUT_TYPE gravity);

/* Pattern Func */
BATTLE_PATTERN explodeBlasterToCenter(void *args);
BATTLE_PATTERN explodeBlasterToEdge(void *args);
BATTLE_PATTERN explodeBlasterToPlayer(void *args);
BATTLE_PATTERN swapGravity(void *args);
BATTLE_PATTERN riseFloorBone(void *args);
BATTLE_PATTERN makeBonePillars(void *args);

/* Main onCollision Func */
void Sans_onCollision(void *args);

/* Sub onCollision Func */
void Sans_DefaultCollision();
void Sans_MoveDetectionCollision();

/* etc */
int writeSpeechBubble(unsigned int scriptIdx, CONSOLE_COLOR tColor, int bVoice);
void setSansFace(ASSET_TYPE type);
void setSansBody(ASSET_TYPE type);
void movePlayerPos();
void fixPlayerPos(ENEMYBOX_STATUS gravityDir);

/* Terminate Func */
void Sans_releasePattern(int pid);
void Sans_releasePatternInRange(int begin, int end);
void Sans_releasePatterns();





/* 
		  Scripts
						 */
static const char scripts[_SANS_SCRIPT_LEN_][128] = {
				    "it's a beautiful day outside.",
				    "birds are singing. flowers are blooming.",
				    "on days like these, kids like you...",
				    "Should be burning in hell.",
//				    "huh.",
//					"always wondered why people never use their strongest attack first.",
				    "here we go.",
				};
				
				
				
				
/* 
	Pattern Details
						 */
static SANS_PATTERN_ARGS sansPatternInfo[_SANS_PATTERN_LEN_] = {
	/*		pattern id	   	  pattern func name		 	gaster blaster args	 	swap gravity args		*/
			{ 	0, 			swapGravity,				0,							_DOWN_ 			},
			{ 	1, 			riseFloorBone,				0,							0 				},
			{ 	2, 			swapGravity,				0,							_INPUT_NONE_	},
			
			{ 	3, 			explodeBlasterToEdge,		_BLAST_TOP_LEFT_,			0 				},
			{ 	4, 			explodeBlasterToEdge,		_BLAST_LEFT_TOP_,			0 				},
			{ 	5, 			explodeBlasterToEdge,		_BLAST_BOT_RIGHT_,			0 				},
			{ 	6, 			explodeBlasterToEdge,		_BLAST_RIGHT_BOT_,			0 				},
			
			{ 	7, 			explodeBlasterToCenter,		_BLAST_RIGHT_CENTER_, 		0 				},
			{ 	8, 			explodeBlasterToCenter,		_BLAST_LEFT_CENTER_, 		0 				},
			{ 	9, 			explodeBlasterToCenter,		_BLAST_TOP_CENTER_, 		0 				},
			{ 	10,			explodeBlasterToCenter,		_BLAST_BOT_CENTER_,			0 				},
	};
#endif