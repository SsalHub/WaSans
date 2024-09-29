#include "battlemanager.h"

int MaxHP = 100;
BATTLE_OBJECT **EnemyInfo = NULL, *SpeechBubble;
BATTLE_OBJECT Player, EnemyPhaseBox, PlayerPhaseBox;
PATTERN_INFO *Patterns;



/* Main Renderer */
void renderBattleScene()
{
	checkPlayerInfo();
    renderPlayerInfo();
    renderSpeechBubble();
    renderEnemy();
    if (battlePhase == _ENEMY_PHASE_)
    {
		renderEnemyPhaseBox();
		renderPattern();
		renderPlayerPos();
	}
	else if (battlePhase == _PLAYER_PHASE_)
	{
		renderPlayerPhaseBox();
		renderSelectBox();
	}
}



/* Init Functions */
void initBattle(int elen, BATTLE_OBJECT (*enemy)[_ENEMY_INFO_LEN_], int plen, PATTERN_INFO *pattern)
{
	battlePhase = _INTRO_PHASE_;
	battleTurn = 0;
	Player.HP = MaxHP;
	battleSelect = 0;
	
	initPlayer();
	initEnemyPhaseBox();
	initPlayerPhaseBox();
	initEnemyInfo(elen, enemy);
	initPatternInfo(plen, pattern);
}

void initPlayer()
{
	setCOORD(&(Player.pos), 0, 0);
	Player.data = (char*)malloc(sizeof(char) * 8);
	strcpy(Player.data, "O");
	Player.isActive = 0;
	Player.tColor = _HOTPINK_;
	Player.bColor = _BLACK_;
}

void initEnemyPhaseBox()
{
	char ch[8];
	int i, j;
	
	EnemyPhaseBox.mode = _ENEMYBOX_DEFAULT_;
	setEnemyBoxSize(_ENEMYBOX_SMALL_);
	
	EnemyPhaseBox.data = (char*)malloc(sizeof(char) * ((ScreenWidth + 1) * (ScreenHeight / 2)));
	// set enemy phase box string
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

	setCOORD(&(PlayerPhaseBox.pos), 6, 14);
	PlayerPhaseBox.width = 103;
	PlayerPhaseBox.height = 10;
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

void initEnemyInfo(int elen, BATTLE_OBJECT (*enemy)[_ENEMY_INFO_LEN_])
{
	int i;
	enemyLen = elen;
	EnemyInfo = (BATTLE_OBJECT**)malloc(sizeof(BATTLE_OBJECT*) * enemyLen);
	SpeechBubble = (BATTLE_OBJECT*)malloc(sizeof(BATTLE_OBJECT) * enemyLen);
	for (i = 0; i < enemyLen; i++)
	{
		EnemyInfo[i] = (BATTLE_OBJECT*)malloc(sizeof(BATTLE_OBJECT) * _ENEMY_INFO_LEN_);
		EnemyInfo[i][_ENEMY_BODY_] = enemy[i][_ENEMY_BODY_];
		EnemyInfo[i][_ENEMY_FACE_] = enemy[i][_ENEMY_FACE_];
		
		SpeechBubble[i].data = (char*)malloc(sizeof(char) * (ScreenWidth * 2));
		strcpy(SpeechBubble[i].data, "");
	}
}

void initPatternInfo(int plen, PATTERN_INFO *pattern)
{
	patternLen = plen;
	Patterns = pattern;
}

void setEnemyBoxSize(ENEMYBOX_STATUS size)
{
	switch (size)
	{
		case _ENEMYBOX_SMALL_:
			setCOORD(&(EnemyPhaseBox.pos), 47, 14);
			EnemyPhaseBox.width = 22;
			EnemyPhaseBox.height = 10;
			break;
		case _ENEMYBOX_NORMAL:
			setCOORD(&(EnemyPhaseBox.pos), 38, 14);
			EnemyPhaseBox.width = 30;
			EnemyPhaseBox.height = 10;
			break;
		case _ENEMYBOX_LARGE:
			setCOORD(&(EnemyPhaseBox.pos), 24, 14);
			EnemyPhaseBox.width = 68;
			EnemyPhaseBox.height = 10;
			break;
	}
}



/* Util Func */
void setBattlePhase(BATTLE_PHASE phase)
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

BATTLE_PHASE getBattlePhase()
{
	return battlePhase;
}

int getBattleTurn()
{
	return battleTurn;
}

HANDLE startPattern(BATTLE_PATTERN_PTR pattern, void *args, unsigned int *threadID)
{
	return (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)pattern, args, 0, threadID);
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

void checkPlayerInfo()
{
	/*  		Player.width == DOT
				Player.height == jump speed (gravity) 		*/
	static int DOT_oldTime = -5000, Gravity_oldTime = -5000;
	static float cumulGravity = 0.0f;
	
	if (Player.width)
	{
		if (500 < clock() - DOT_oldTime)
		{
			Player.HP--;
			Player.width--;
			if (Player.HP <= 0)
				Player.mode = _PLAYER_DIED_;
			DOT_oldTime = clock();
		}
	}
	if (EnemyPhaseBox.mode != _ENEMYBOX_DEFAULT_)
	{
		if (Player.height)
		{
			if (30 < clock() - Gravity_oldTime)
			{
				cumulGravity += 0.3f;
				if (1 <= cumulGravity)
				{
					Player.pos.Y -= 1;
					cumulGravity -= 0.7f;
					Player.height = Player.height < 1 ? 0 : Player.height - 1;
				}
				Gravity_oldTime = clock();
			}
		}
		else if (Player.pos.Y < EnemyPhaseBox.pos.Y + EnemyPhaseBox.height - 2)
		{
			// if player is not on platform
			if (30 < clock() - Gravity_oldTime)
			{
				cumulGravity += 0.3f;
				if (1 <= cumulGravity)
				{
					Player.pos.Y += 1;
					cumulGravity -= 0.7f;
					Player.height = Player.height < 1 ? 0 : Player.height - 1;
				}
				Gravity_oldTime = clock();
			}
		}
		else
		{
			cumulGravity = 0;
		}
	}
}



/* Collider */
COORD* checkCollision(COORD *src, COORD *trg)
{
	if (src->X == trg->X && src->Y == trg->Y)
		return src;
	return NULL;
}

COORD* checkCollisionInRange(COORD *src, COORD *begin, COORD *end)
{
	int bCollision = (begin->X <= src->X && src->X <= end->X) && (begin->Y <= src->Y && src->Y <= end->Y);
	if (bCollision)
		return src;
	return NULL;
}

void setCollisionInfo(COLLISION_INFO *target, COORD pos, BATTLE_PATTERN_PTR pattern, RENDER_INFO *render)
{
	target->pattern = pattern;
	cpyCOORD(&(target->pos), &(pos));
	target->tColor = render->tColor;
	target->bColor = render->bColor;
}



/* Sub Renderer */
void renderEnemy()
{
	if (!EnemyInfo)
		return;
	int i;
	for (i = 0; i < enemyLen; i++)
	{
		// render body
		printLines(
				EnemyInfo[i][_ENEMY_BODY_].pos.X, 
				EnemyInfo[i][_ENEMY_BODY_].pos.Y, 
				EnemyInfo[i][_ENEMY_BODY_].data, 
				EnemyInfo[i][_ENEMY_BODY_].tColor, 
				EnemyInfo[i][_ENEMY_BODY_].bColor
		);
		// render face
		printLines(
				EnemyInfo[i][_ENEMY_FACE_].pos.X, 
				EnemyInfo[i][_ENEMY_FACE_].pos.Y, 
				EnemyInfo[i][_ENEMY_FACE_].data, 
				EnemyInfo[i][_ENEMY_FACE_].tColor, 
				EnemyInfo[i][_ENEMY_FACE_].bColor
		);
	}
}

void renderEnemyPhaseBox()
{
    printLines(EnemyPhaseBox.pos.X, EnemyPhaseBox.pos.Y, EnemyPhaseBox.data, _WHITE_, _BLACK_);
}

void renderPlayerPos()
{
    printLine(Player.pos.X, Player.pos.Y, Player.data, Player.tColor, Player.bColor);
}

void renderPlayerPhaseBox()
{
    printLines(PlayerPhaseBox.pos.X, PlayerPhaseBox.pos.Y, PlayerPhaseBox.data, _WHITE_, _BLACK_);
}

void renderPlayerInfo()
{
	static const int y = 24;
    int x = 11;
    char itoa_str[8], level_str[8], hp_str[12], hp_bar[12];
    int damaged, DOT, currHP;
    int i, idx = 0;
    
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
    itoa(Player.HP, hp_str, 10);
    strcat(hp_str, " / ");
    itoa(MaxHP, itoa_str, 10);
    strcat(hp_str, itoa_str);
    printLine(60, y, hp_str, _WHITE_, _BLACK_);
    
	// set string that HP info
    strcpy(hp_bar, "==========");
    printLine(48, y, hp_bar, _RED_, _BLACK_);
    // calculate current damage
    // set current player HP info
    if (Player.width)	// player's sub HP
    {
		currHP = Player.HP / 10;
	    for (i = 0; i < currHP; i++)
	        hp_bar[i] = '@';
	    hp_bar[i] = '\0';
	    printLine(48, y, hp_bar, _YELLOW_, _BLACK_);
	    // print DoT
		DOT = Player.width / 10;
	    for (i = 0; i < DOT; i++)
	        hp_bar[i] = '#';
		if (Player.width % 10)
			hp_bar[i++] = '#';
	    hp_bar[i] = '\0';
	    printLine(48 + currHP - DOT, y, hp_bar, _PURPLE_, _BLACK_);
	}
	else
	{
		currHP = Player.HP / 10;
	    for (i = 0; i < currHP; i++)
	        hp_bar[i] = '@';
	    hp_bar[i] = '\0';
	    printLine(48, y, hp_bar, _YELLOW_, _BLACK_);
	}
}

void renderSelectBox()
{
	const int y = 25;
    CONSOLE_COLOR tSelect[4] = {_YELLOW_, _YELLOW_, _YELLOW_, _YELLOW_};
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
	COORD begin, end, pos;
	int slen, x, y, w, h;
	int i, j;
	
	for (i = 0; i < enemyLen; i++)
	{
		if (SpeechBubble[i].isActive)
		{
			x = SpeechBubble[i].pos.X;	
			y = SpeechBubble[i].pos.Y;
			w = SpeechBubble[i].width;
			h = SpeechBubble[i].height;
			// render bubble box
			begin.X = x;
			begin.Y = y;
			end.X = x + w;
			end.Y = y + h;
			fillColorInRange(begin, end, _WHITE_);
			pos.X = x - 1;
			pos.Y = y + 3;
			fillColorInRange(pos, pos, _WHITE_);
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
	COORD pos, begin, end, playerpos;
	DWORD dw;
	RENDER_INFO *render;
	COLLISION_INFO colInfo;
	int i, j, k, layer;
	
	for (layer = (_PATTERN_LAYER_LEN_ - 1); 0 <= layer; layer--)
	{
		for (i = 0; i < patternLen; i++)
		{
			if (Patterns[i].isActive != STILL_ACTIVE)
			continue;
			
			for (j = (Patterns[i].renderInfoLen[layer] - 1);  0 <= j; j--)
			{
				render = &(Patterns[i].renderInfo[layer][j]);
				if (render->s != NULL)
				{
					// render pattern
					printLines(
						render->pos.X,
						render->pos.Y,
						render->s,
						render->tColor,
						render->bColor
					);
					// check collision
					if (render->isCollidable)
					{
						cpyCOORD(&begin, &(render->pos));
						setCOORD(
							&(end),
							render->pos.X + render->width, 
							render->pos.Y + render->height
						);
						if (checkCollisionInRange(&(Player.pos), &begin, &end))
						{
							// set collision info
							setCollisionInfo(&colInfo, Player.pos, Patterns[i].pattern, render);
							Patterns[i].collider(&colInfo);
						}
					}
				}
				else
				{
					pos.X = render->pos.X;
					for (k = 0; k < render->height; k++)
					{
						pos.Y = render->pos.Y + k;
						// render pattern
						FillConsoleOutputAttribute(
							ScreenHandle[ScreenIndex], 
							render->tColor | (render->bColor << 4), 
							render->width,
							pos, 
							&dw
						);
						// check collision
						if (render->isCollidable)
						{
							cpyCOORD(&begin, &(render->pos));
							setCOORD(	
								&(end),
								render->pos.X + render->width, 
								render->pos.Y + render->height
							);
							if (checkCollisionInRange(&(Player.pos), &begin, &end))
							{
								// set collision info
								setCollisionInfo(&colInfo, Player.pos, Patterns[i].pattern, render);
								Patterns[i].collider(&colInfo);
							}
						}
					}
					
				}
			}
		}
	}
}



/* Terminate Func */
void releaseBattleAssets()
{
	int i;
	for (i = 0; i < enemyLen; i++)
	{
		free(SpeechBubble[i].data);
		free(EnemyInfo[i]);
	}
	free(EnemyInfo);
	free(SpeechBubble);
	free(EnemyPhaseBox.data);
	free(PlayerPhaseBox.data);
	free(Player.data);
}