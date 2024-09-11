#include "battle.h"

int MaxHP = 100;
COORD PlayerPos;
BattleObject (*EnemyInfo)[3];
BattleObject EnemyPhaseBox, PlayerPhaseBox, *SpeechBubble;

/* Init Functions */
void initBattle(int len, BattleObject enemy[][3])
{
	battleTurn = 0;
	battlePhase = 0;
	playerHP = MaxHP;
	battleSelect = 0;
	
	initEnemyPhaseBox();
	initPlayerPhaseBox();
	initEnemyInfo(len, enemy);
	setSceneRenderer(renderBattleScene);
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

void initEnemyInfo(int len, BattleObject enemy[][3])
{
	int i;
	enemyLen = len;
	EnemyInfo = (BattleObject**)malloc(sizeof(BattleObject) * 3 * len);
	for (i = 0; i < len; i++)
	{
		EnemyInfo[i][_ENEMY_LEG_] = enemy[i][_ENEMY_LEG_];
		EnemyInfo[i][_ENEMY_BODY_] = enemy[i][_ENEMY_BODY_];
		EnemyInfo[i][_ENEMY_FACE_] = enemy[i][_ENEMY_FACE_];
	}
	SpeechBubble = (BattleObject*)malloc(sizeof(BattleObject) * enemyLen);
}



/* Util Func */
void setBattlePhase(BattlePhase phase)
{
	battlePhase = phase;
}

void gotoNextPhase()
{
	battlePhase = _PLAYER_PHASE_ <= battlePhase ? _ENEMY_PHASE_ : battlePhase + 1;
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



/* Main Renderer */
void renderBattleScene()
{
    renderEnemy();
    renderPlayerInfo();
	switch (battlePhase)
	{
		case _INTRO_PHASE_:
		case _ENEMY_PHASE_:
    		renderEnemyPhaseBox();
    		break;
    	case _PLAYER_PHASE_:
    		renderPlayerPhaseBox();
    		renderSelectBox();
    		break;
	}
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
    char ch[8] = "♥";
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
	int x = 6, y = 16, w = 103, h = 8, i, j;
    char buffer[(ScreenWidth + 1) * (ScreenHeight / 2)], ch[3];

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
    printLines(x, y, buffer, _WHITE_, _BLACK_);
}

void renderPlayerInfo()
{
	static const int y = 24;
    int x = 11, i, damaged;
    char tempstr[11];
	char playerInfo[ScreenWidth];
    int idx = 0;

    strcpy(playerInfo, "");
    // set string that player basic info
    fillSpaceChar(playerInfo, 0, 11);
	strcat(playerInfo, PlayerName);
    fillSpaceChar(playerInfo, 12 + strlen(PlayerName), 26);
	strcat(playerInfo, "LV ");
    itoa(PlayerLevel, tempstr, 10);
	strcat(playerInfo, tempstr);

    // set string that numeric HP info
    fillSpaceChar(playerInfo, 27 + strlen("LV ") + strlen(tempstr), 42);
	strcat(playerInfo, "HP");
    fillSpaceChar(playerInfo, 43 + strlen("HP"), 57);
    itoa(playerHP, tempstr, 10);
	strcat(playerInfo, tempstr);
	strcat(playerInfo, " / ");
    itoa(MaxHP, tempstr, 10);
	strcat(playerInfo, tempstr);
	// render 
    printLine(12, y, playerInfo, _WHITE_, _BLACK_);
    
    // set string that HP info
    for (int i = 0; i < 10; i++)
        tempstr[i] = '@';
    tempstr[10] = '\0';
    printLine(46, y, tempstr, _YELLOW_, _BLACK_);
    damaged = (MaxHP - playerHP) / 10;
    // set player HP info
    for (i = 0; i < damaged; i++)
        tempstr[i] = '#';
    tempstr[damaged] = '\0';
    printLine(56 - damaged, y, tempstr, _RED_, _BLACK_);
}
    
void renderSelectBox()
{
	int x, y = 25;
    ConsoleColor tSelect[4] = {_YELLOW_, _YELLOW_, _YELLOW_, _YELLOW_};
    tSelect[battleSelect] = _LIGHT_YELLOW_;
    x = 7;
    printLines(x, y, AssetFile[_SELECT_BOX_], tSelect[0], _BLACK_);
    printLine(x + 8, y + 2, "FIGHT", tSelect[0], _BLACK_);
    x += 27;
    printLines(35, y, AssetFile[_SELECT_BOX_], tSelect[1], _BLACK_);
    printLine(x + 11, y + 2, "ACT", tSelect[1], _BLACK_);
    x += 27;
    printLines(63, y, AssetFile[_SELECT_BOX_], tSelect[2], _BLACK_);
    printLine(x + 11, y + 2, "ITEM", tSelect[2], _BLACK_);
    x += 27;
    printLines(91, y, AssetFile[_SELECT_BOX_], tSelect[3], _BLACK_);
    printLine(x + 11, y + 2, "MERCY", tSelect[3], _BLACK_);
}

void renderSpeechBubble()
{
//	int bubbleX = 74, bubbleY = 2;
	static const int bubbleWidth = 24, bubbleHeight = 6;
	char buffer[(ScreenWidth + 1) * (ScreenHeight / 2)];
	int i, j;
	
	for (i = 0; i < enemyLen; i++)
	{
		if (!SpeechBubble[i].isActive)
			continue;
		// render bubble box
	    buffer[0] = '\0';
	    for (j = 0; j < bubbleWidth; j++)
	        strcat(buffer, " ");
	    for (i = 0; i < bubbleHeight; i++)
	        printLine(x, y + i, buffer, _WHITE_, _WHITE_);
	    strcat(buffer, " ");
	    printLine(SpeechBubble[i].x - 1, SpeechBubble[i].y + (bubbleHeight / 2 - 1), buffer, _WHITE_, _WHITE_);
	    // render text
    	printLine(SpeechBubble[i].x, SpeechBubble[i].y, SpeechBubble[i].data, SpeechBubble[i].tColor, _WHITE_);
	}
}



/* Terminate Func */
void releaseBattleAssets()
{
	int i;
	for (i = 0; i < enemyLen; i++)
	{
		free(EnemyInfo[i]);
	}
	free(SpeechBubble);
	free(EnemyPhaseBox.data);
	free(PlayerPhaseBox.data);
}