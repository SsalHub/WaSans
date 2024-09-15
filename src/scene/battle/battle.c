#include "battle.h"

int MaxHP = 100;
COORD PlayerPos;
BattleObject **EnemyInfo, *SpeechBubble;
BattleObject EnemyPhaseBox, PlayerPhaseBox;
PatternInfo *Patterns;



/* Main Renderer */
void renderBattleScene()
{
    renderEnemy();
    renderPlayerInfo();
    renderSpeechBubble();
    if (battlePhase == _ENEMY_PHASE_)
    {
		renderEnemyPhaseBox();
		renderPattern();
		return;
	}
	if (battlePhase == _PLAYER_PHASE_)
	{
		renderPlayerPhaseBox();
		renderSelectBox();
		return;
	}
}



/* Init Functions */
void initBattle(int elen, BattleObject (*enemy)[3], int plen, PatternInfo* pattern)
{
	battlePhase = _INTRO_PHASE_;
	battleTurn = 0;
	playerHP = MaxHP;
	battleSelect = 0;
	
	initEnemyPhaseBox();
	initPlayerPhaseBox();
	initEnemyInfo(elen, enemy);
	initPatternInfo(plen, pattern);
}

void initEnemyPhaseBox()
{
	char ch[8];
	int i, j;
	
	EnemyPhaseBox.x = 49;
	EnemyPhaseBox.y = 16;
	EnemyPhaseBox.width = 18;
	EnemyPhaseBox.height = 8;
	EnemyPhaseBox.data = (char*)malloc(sizeof(char) * ((ScreenWidth + 1) * (ScreenHeight / 2)));
	
	// print enemy phase box
    EnemyPhaseBox.data[0] = '\0';
    for (i = 0; i < EnemyPhaseBox.height; i++)
    {
        strcat(EnemyPhaseBox.data, ":=");
        if (i == 0 || i == EnemyPhaseBox.height - 1)
            strcpy(ch, "=");
        else
            strcpy(ch, " ");
        for (j = 0; j < EnemyPhaseBox.width; j++)
            strcat(EnemyPhaseBox.data, ch);
        strcat(EnemyPhaseBox.data, "=: \n");
    }
}

void initPlayerPhaseBox()
{
	char ch[3];
	int i, j;

	PlayerPhaseBox.x = 6;
	PlayerPhaseBox.y = 16;
	PlayerPhaseBox.width = 103;
	PlayerPhaseBox.height = 8;
	PlayerPhaseBox.data = (char*)malloc(sizeof(char) * ((ScreenWidth + 1) * (ScreenHeight / 2)));

    PlayerPhaseBox.data[0] = '\0';
    for (i = 0; i < PlayerPhaseBox.height; i++)
    {
        strcat(PlayerPhaseBox.data, ":=");
        if (i == 0 || i == PlayerPhaseBox.height - 1)
            strcpy(ch, "=");
        else
            strcpy(ch, " ");
        for (j = 0; j < PlayerPhaseBox.width; j++)
            strcat(PlayerPhaseBox.data, ch);
        strcat(PlayerPhaseBox.data, "=: \n");
    }
}

void initEnemyInfo(int elen, BattleObject (*enemy)[3])
{
	int i;
	enemyLen = elen;
	EnemyInfo = (BattleObject**)malloc(sizeof(BattleObject*) * enemyLen);
	SpeechBubble = (BattleObject*)malloc(sizeof(BattleObject) * enemyLen);
	for (i = 0; i < enemyLen; i++)
	{
		EnemyInfo[i] = (BattleObject*)malloc(sizeof(BattleObject) * 3);
		EnemyInfo[i][_ENEMY_LEG_] = enemy[i][_ENEMY_LEG_];
		EnemyInfo[i][_ENEMY_BODY_] = enemy[i][_ENEMY_BODY_];
		EnemyInfo[i][_ENEMY_FACE_] = enemy[i][_ENEMY_FACE_];
		
		SpeechBubble[i].data = (char*)malloc(sizeof(char) * (ScreenWidth * 2));
		strcpy(SpeechBubble[i].data, "");
	}
}

void initPatternInfo(int plen, PatternInfo* pattern)
{
	patternLen = plen;
	Patterns = pattern;
}



/* Util Func */
void setBattlePhase(BattlePhase phase)
{
	battlePhase = phase;
}

void gotoNextPhase()
{
	if (_ENEMY_PHASE_ <= battlePhase)
	{
		battlePhase = _PLAYER_PHASE_;
    	gotoNextTurn();
	}
	else
	{
		battlePhase++;
	}
}

void gotoNextTurn()
{
	battleTurn++;
}

BattlePhase getBattlePhase()
{
	return battlePhase;
}

int getBattleTurn()
{
	return battleTurn;
}

HANDLE startPattern(Pattern pattern, void* args, unsigned int* threadID)
{
	return (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)pattern, args, 0, threadID);
}

void movePlayerPos()
{
	static int playerSpeed = 1, oldTime;
	if (playerSpeed == 0)
	{
		if (oldTime == 0)
		{
			oldTime = clock();
		}
		else if (50 < clock() - oldTime)
		{
			playerSpeed = 1;
			oldTime = 0;
		}
	}
	else
	{
		// key input
		if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(0x41))
		{
			PlayerPos.X -= playerSpeed;
			playerSpeed = 0;
		}
		else if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(0x44))
		{
			PlayerPos.X += playerSpeed;
			playerSpeed = 0;
		}
		if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(0x57))
		{
			PlayerPos.Y -= playerSpeed;
			playerSpeed = 0;
		}
		else if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(0x53))
		{
			PlayerPos.Y += playerSpeed;
			playerSpeed = 0;
		}
	}
}

int movePlayerSelectBox()
{
	static int oldTime = -1000;
	int currTime;
	
	// if not enough input cooltime passed
	currTime = clock();
	if (currTime - oldTime < 30)
		return -1;
	oldTime = currTime;
	
	if (kbhit())
	{
		char input = getch();
		
		switch (input)
		{
			case 'A':
			case 'a':
			case _LEFT_:
				if (0 < battleSelect)
					battleSelect--;
				break;
				
			case 'D':
			case 'd':
			case _RIGHT_:
				if (battleSelect < 3)
					battleSelect++;
				break;
				
			case _SPACE_:
			case _CARRIAGE_RETURN_:
				return battleSelect;
				
			default:
				break;
		}
	}
	return -1;
}

// if player died return 0. or return 1.
int getPlayerDamage(int damage)
{
	playerHP -= damage;
	if (playerHP <= 0)
		return 0;
	return 1;
}



/* Sub Renderer */
void renderEnemy()
{
	int i;
	for (i = 0; i < enemyLen; i++)
	{
		if (!EnemyInfo[i][_ENEMY_BODY_].isActive)
			continue;
		// render leg
		printLines(
				EnemyInfo[i][_ENEMY_LEG_].x, 
				EnemyInfo[i][_ENEMY_LEG_].y, 
				EnemyInfo[i][_ENEMY_LEG_].data, 
				EnemyInfo[i][_ENEMY_LEG_].tColor, 
				EnemyInfo[i][_ENEMY_LEG_].bColor
		);
		// render body
		printLines(
				EnemyInfo[i][_ENEMY_BODY_].x, 
				EnemyInfo[i][_ENEMY_BODY_].y, 
				EnemyInfo[i][_ENEMY_BODY_].data, 
				EnemyInfo[i][_ENEMY_BODY_].tColor, 
				EnemyInfo[i][_ENEMY_BODY_].bColor
		);
		// render face
		printLines(
				EnemyInfo[i][_ENEMY_FACE_].x, 
				EnemyInfo[i][_ENEMY_FACE_].y, 
				EnemyInfo[i][_ENEMY_FACE_].data, 
				EnemyInfo[i][_ENEMY_FACE_].tColor, 
				EnemyInfo[i][_ENEMY_FACE_].bColor
		);
	}
}

void renderEnemyPhaseBox()
{
    printLines(EnemyPhaseBox.x, EnemyPhaseBox.y, EnemyPhaseBox.data, _WHITE_, _BLACK_);
    renderPlayerPos();
}

void renderPlayerPos()
{
    char ch[8] = "O";
    // fix player x pos
    if (PlayerPos.X <= EnemyPhaseBox.x + 2)
        PlayerPos.X = EnemyPhaseBox.x + 2;
    else if (EnemyPhaseBox.x + 1 + EnemyPhaseBox.width <= PlayerPos.X)
        PlayerPos.X = EnemyPhaseBox.x + 1 + EnemyPhaseBox.width;
    // fix player y pos
    if (PlayerPos.Y <= EnemyPhaseBox.y)
        PlayerPos.Y = EnemyPhaseBox.y + 1;
    else if (EnemyPhaseBox.y + EnemyPhaseBox.height - 2 <= PlayerPos.Y)
        PlayerPos.Y = EnemyPhaseBox.y + EnemyPhaseBox.height - 2;
    // render   
    printLine(PlayerPos.X, PlayerPos.Y, ch, _HOTPINK_, _BLACK_);
}

void renderPlayerPhaseBox()
{
	const int w = 103, h = 8;
    static char buffer[1024];
	char ch[3];
	int i, j;

	if (strlen(buffer) < 1)
	{
	    buffer[0] = '\0';
	    for (i = 0; i < h; i++)
	    {
	        strcat(buffer, ":=");
	        if (i == 0 || i == h - 1)
	            strcpy(ch, "=");
	        else
	            strcpy(ch, " ");
	        for (j = 0; j < w; j++)
	            strcat(buffer, ch);
	        strcat(buffer, "=: \n");
	    }
	}
    printLines(6, 16, buffer, _WHITE_, _BLACK_);
}

void renderPlayerInfo()
{
	static const int y = 24;
    int x = 11, i, damaged;
    char itoa_str[8], level_str[8], hp_str[12], hp_bar[12];
//	char playerInfo[ScreenWidth];
    int idx = 0;
    
    // player name
    printLine(12, y, PlayerName, _WHITE_, _BLACK_);
    // player level
    if (strlen(level_str) < 1)
    {
		strcpy(level_str, "LV ");
	    itoa(PlayerLevel, itoa_str, 10);
		strcat(level_str, itoa_str);
	}
    printLine(26, y, level_str, _WHITE_, _BLACK_);
    // player hp
    printLine(44, y, "HP", _WHITE_, _BLACK_);
    itoa(playerHP, hp_str, 10);
    strcat(hp_str, " / ");
    itoa(MaxHP, itoa_str, 10);
    strcat(hp_str, itoa_str);
    printLine(60, y, hp_str, _WHITE_, _BLACK_);
    
	// set string that HP info
    for (int i = 0; i < 10; i++)
        hp_bar[i] = '@';
    hp_bar[10] = '\0';
    printLine(48, y, hp_bar, _YELLOW_, _BLACK_);
    // calculate current damage
    damaged = (MaxHP - playerHP) / 10;
    if (damaged <= 0)
    	return;
    // set current player HP info
    for (i = 0; i < damaged; i++)
        hp_bar[i] = '#';
    hp_bar[damaged] = '\0';
    printLine(58 - damaged, y, hp_bar, _RED_, _BLACK_);
}
    
void renderSelectBox()
{
	const int y = 25;
    ConsoleColor tSelect[4] = {_YELLOW_, _YELLOW_, _YELLOW_, _YELLOW_};
    tSelect[battleSelect] = _LIGHT_YELLOW_;
    // FIGHT 
    printLines(7, y, AssetFile[_SELECT_BOX_], tSelect[0], _BLACK_);
    printLine(7 + 8, y + 2, "FIGHT", tSelect[0], _BLACK_);
    // ACT
    printLines(35, y, AssetFile[_SELECT_BOX_], tSelect[1], _BLACK_);
    printLine(34 + 11, y + 2, "ACT", tSelect[1], _BLACK_);
    // ITEM
    printLines(63, y, AssetFile[_SELECT_BOX_], tSelect[2], _BLACK_);
    printLine(61 + 11, y + 2, "ITEM", tSelect[2], _BLACK_);
    // MERCY
    printLines(91, y, AssetFile[_SELECT_BOX_], tSelect[3], _BLACK_);
    printLine(88 + 11, y + 2, "MERCY", tSelect[3], _BLACK_);
}

void renderSpeechBubble()
{
	if (SpeechBubble == NULL)
		return;
	static int bubbleWidth = 0;
	char buffer[ScreenWidth * 2];
	int i, j, slen, x, y, w, h;
	
	for (i = 0; i < enemyLen; i++)
	{
		if (SpeechBubble[i].isActive)
		{
			x = SpeechBubble[i].x;	
			y = SpeechBubble[i].y;
			w = SpeechBubble[i].width;
			h = SpeechBubble[i].height;
			// render bubble box
			fillColorInRange(x, y, x + w, y + h, _WHITE_);
			fillColorInRange(x - 1, y + 3, x - 1, y + 3, _WHITE_);
		    // render text
		    j = 0;
			slen = strlen(SpeechBubble[i].data);
		    while (w - 2 < slen)
		    {
		    	memcpy(buffer, SpeechBubble[i].data + ((w - 2) * j), w - 2);
		    	buffer[w - 2] = '\0';
	    		printLine(x + 1, y + 1 + j, buffer, SpeechBubble[i].tColor, _WHITE_);
	    		j++;
	    		slen -= (w - 2);
			}
			strcpy(buffer, SpeechBubble[i].data + ((w - 2) * j));
	    	printLine(x + 1, y + 1 + j, buffer, SpeechBubble[i].tColor, _WHITE_);
		}
	}
}

void renderPattern()
{
	int i, j;
	for (i = 0; i < patternLen; i++)
	{
		if (Patterns[i].isActive != STILL_ACTIVE)
			continue;
		for (j = 0; j < Patterns[i].renderInfoLen; j++)
		{
			if (Patterns[i].renderInfo[j].s == NULL)
				continue;
			printLines(
				Patterns[i].renderInfo[j].x,
				Patterns[i].renderInfo[j].y,
				Patterns[i].renderInfo[j].s,
				Patterns[i].renderInfo[j].tColor,
				Patterns[i].renderInfo[j].bColor
			);
		}
	}
}



/* Terminate Func */
void releaseBattleAssets()
{
	int i;
	for (i = 0; i < enemyLen; i++)
	{
		free(EnemyInfo[i]);
		free(SpeechBubble[i].data);
	}
	free(EnemyInfo);
	free(SpeechBubble);
	free(EnemyPhaseBox.data);
	free(PlayerPhaseBox.data);
}