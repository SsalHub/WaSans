#include "sans_battle.h"

/* Main func in sans battle */
void runSansBattle()
{
    char input;
    playerHP = MaxHP;
    battleTurn = -1;
    battleSelect = 0;

    // Enter scene
    introPhase();
    blackScreenEffect(1.0f);
    bossPhase();
    battleTurn++;
    playBGM(_BGM_MEGALOVANIA_, _SOUND_BEGIN_);
//    playerPhase();

    while (1)
    {
        if (kbhit())
        {
            input = getch();
            switch (input)
            {
            case _LEFT_:
            case 'A':
            case 'a':
                if (0 < battleSelect)
                    battleSelect--;
                break;

            case _RIGHT_:
            case 'D':
            case 'd':
                if (battleSelect < 3)
                    battleSelect++;
                break;

            case _SPACE_:
            case _CARRIAGE_RETURN_:
                if (battleSelect == 0)
                {
                    playerHP -= 5;
                }
                else if (battleSelect == 3)
                {
    				playBGM(_BGM_MEGALOVANIA_, _SOUND_PAUSE_);
                    return;
                }
                break;
            }
        }
        renderCustom(renderSansBattle);
    }
}

/* Each Phase Func */
void introPhase()
{
    scriptIdx = -1;
    sleep(1.0f);
    fadeIn(renderIntroPhase);
    while (scriptIdx < _SANS_SCRIPT_LEN_)
    {
        renderCustom(renderIntroPhase);
    }
}

void bossPhase()
{
	static int oldTime = 0;
	static int pattern[3] = {
					1,
					2,
					3,
			};
	int patternIndex = 0;
    playerPos.X = 59;
    playerPos.Y = 19;
    
    switch (battleTurn)
    {
	    case -1: // intro turn
	        while (1)
	        {
	        	movePlayer();
	        	// runPattern(pattern[patternIndex]);
	            renderCustom(renderBossPhase);
	        }
	        break;
	        
	    case 0:
//	        while (1)
//	        {
//	        	movePlayer();
//	            renderCustom(renderBossPhase);
//	        }
	        break;
    }
}

void playerPhase()
{
    if (battleTurn == 0)
    {
        // action
    }
}

/* Main Renderer */
void renderSansBattle()
{
    renderSans(_SANS_FACE_NORMAL_A_);

    //	renderBossPhaseBox();
    renderPlayerPhaseBox();

    renderPlayerInfo();
    renderSelectBox();
}

void renderIntroPhase()
{
    static int oldTime = 0;
    int flag;

    // render Sans
    if (scriptIdx != 3)
        renderSans(_SANS_FACE_NORMAL_A_);
    else
        renderSans(_SANS_FACE_NORMAL_B_);

    // render player info
    renderPlayerInfo();
    // render speech bubble
    if (scriptIdx < 0) // waiting script before speech
    {
        if (oldTime == 0)
        {
            oldTime = clock();
            return;
        }
        if (clock() - oldTime < 2000)
        {
            return;
        }
        scriptIdx = 0;
        oldTime = 0;
    }
    if (scriptIdx == 3)
    	flag = renderSpeechBubble(scripts[scriptIdx], _RED_);
    else
    	flag = renderSpeechBubble(scripts[scriptIdx], _BLACK_);
    if (flag < 0)
    {
        if (!oldTime)
        {
            oldTime = clock();
            return;
        }
        if (2000 < clock() - oldTime)
        {
            scriptIdx++;
            oldTime = 0;
        }
    }
}

void renderBossPhase()
{
    renderSans(_SANS_FACE_NORMAL_A_);
    renderBossPhaseBox();
    renderPlayerInfo();
}

void renderPlayerPhase()
{
    renderSans(_SANS_FACE_NORMAL_A_);
    renderPlayerPhaseBox();
    renderPlayerInfo();
    renderSelectBox();
}

/* Sub Renderer */
void renderSans(AssetFileType face)
{
    const int MAX_TICK = 12;
    static int tick, oldTime;
    int face_move[12][2] = {{0, 0}, {1, 0}, {1, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
        body_move[12][2] = {{1, 0}, {1, 0}, {1, 0}, {1, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
        leg_move[12][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
    int x = 50, y = 0, currTime;

    // leg
    //	printLines(x + 1 + leg_move[tick][0], y + 11 + leg_move[tick][1], AssetFile[_SANS_LEG_NORMAL_], _WHITE_);
    printLines(x + 2, y + 13, AssetFile[_SANS_LEG_NORMAL_], _WHITE_, _BLACK_);
    // body
    //	printLines(x + body_move[tick][0], y + 7 + body_move[tick][1], AssetFile[_SANS_BODY_NORMAL_], _WHITE_);
    printLines(x, y + 9, AssetFile[_SANS_BODY_NORMAL_], _WHITE_, _BLACK_);
    // face
    //	printLines(x + face_move[tick][0], y + face_move[tick][1], AssetFile[face], _WHITE_);
    printLines(x + 1, y, AssetFile[face], _WHITE_, _BLACK_);

    // set ticks based on current/old time
    if (oldTime)
    {
        currTime = clock();
        if (100 <= currTime - oldTime) // 100ms == 0.1sec
        {
            tick = MAX_TICK <= tick + 1 ? 0 : tick + 1;
            oldTime = currTime;
        }
    }
    else
    {
        oldTime = clock();
    }
}

int renderSpeechBubble(const char* script, ConsoleColor tColor)
{
    int x = 74, y = 2, w = 24, h = 6, i, j, currTime;
    static int currLen = 0, oldTime = 0;
    static const char *pScript;
    char buffer[ScreenWidth * 2], ch[3], *copy, tmp, input;

    if (pScript != script)
    {
        currLen = 0;
        pScript = script;
    }

    if (kbhit())
    {
        input = getch();
        if (input == _SPACE_ || input == _CARRIAGE_RETURN_)
            currLen = strlen(script);
    }
    else
    {
        if (!oldTime)
            oldTime = clock();
        currTime = clock();
        if (80 < currTime - oldTime)
        {
            currLen = strlen(script) < currLen + 1 ? currLen : currLen + 1;
            oldTime = currTime;
        }
    }

    // print bubble box
    buffer[0] = '\0';
    for (j = 0; j < w; j++)
        strcat(buffer, " ");
    for (i = 0; i < h; i++)
        printLine(x, y + i, buffer, _WHITE_, _WHITE_);
    strcat(buffer, " ");
    printLine(x - 1, y + (h / 2 - 1), buffer, _WHITE_, _WHITE_);

    // print script until currLen
    x += 1;
    y += 1;
    copy = (char *)malloc(sizeof(char) * strlen(script) + 1);
    strcpy(copy, script);
    copy[currLen] = '\0';
    i = 0;
    while (strlen(copy) / (w - 2))
    {
        tmp = copy[w - 2];
        copy[w - 2] = '\0';
        printLine(x, y + i, copy, tColor, _WHITE_);
        copy[w - 2] = tmp;
        strcpy(copy, copy + (w - 2));
        i++;
    }
    printLine(x, y + i, copy, tColor, _WHITE_);
    free(copy);

    if (strlen(script) <= currLen)
        return -1;
    if (0 < currLen && script[currLen - 1] != ' ')
    	playVoice(_VOICE_SANS_);
    return currLen;
}

void renderBossPhaseBox()
{
    int i, j;
    char buffer[(ScreenWidth + 1) * (ScreenHeight / 2)], ch[8];

    // print boss phase box
    buffer[0] = '\0';
    for (i = 0; i < bossPhaseBox.h; i++)
    {
        strcat(buffer, ":=");
        if (i == 0 || i == bossPhaseBox.h - 1)
            strcpy(ch, "=");
        else
            strcpy(ch, " ");
        for (j = 0; j < bossPhaseBox.w; j++)
            strcat(buffer, ch);
        strcat(buffer, "=: \n");
    }
    printLines(bossPhaseBox.x, bossPhaseBox.y, buffer, _WHITE_, _BLACK_);

    // print player
    // fix player x pos
    if (playerPos.X <= bossPhaseBox.x + 2)
        playerPos.X = bossPhaseBox.x + 2;
    else if (bossPhaseBox.x + 1 + bossPhaseBox.w <= playerPos.X)
        playerPos.X = bossPhaseBox.x + 1 + bossPhaseBox.w;
    // fix player y pos
    if (playerPos.Y <= bossPhaseBox.y)
        playerPos.Y = bossPhaseBox.y + 1;
    else if (bossPhaseBox.y + bossPhaseBox.h - 2 <= playerPos.Y)
        playerPos.Y = bossPhaseBox.y + bossPhaseBox.h - 2;
    strcpy(ch, "¢¾");
    printLine(playerPos.X, playerPos.Y, ch, _HOTPINK_, _BLACK_);
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
    int x, y = 24, i, damaged;
    char hpText[11];

    // render player info
    x = 12;
    printLine(x, y, "HSU", _WHITE_, _BLACK_);
    x = 26;
    printLine(x, y, "LV 1", _WHITE_, _BLACK_);

    // render HP info
    // set max HP text
    for (int i = 0; i < 10; i++)
        hpText[i] = '@';
    hpText[10] = '\0';
    x = 43;
    printLine(x, y, "HP", _WHITE_, _BLACK_);
    x += 3;
    printLine(x, y, hpText, _YELLOW_, _BLACK_);
    damaged = (MaxHP - playerHP) / 10;
    // set player HP info
    for (i = 0; i < damaged; i++)
        hpText[i] = '#';
    hpText[damaged] = '\0';
    printLine(x + 10 - damaged, y, hpText, _RED_, _BLACK_);

    // render numeric HP info
    x += 12;
    itoa(playerHP, hpText, 10);
    printLine(x, y, hpText, _WHITE_, _BLACK_);
    x += strlen(hpText);
    printLine(x, y, " / ", _WHITE_, _BLACK_);
    x += 3;
    itoa(MaxHP, hpText, 10);
    printLine(x, y, hpText, _WHITE_, _BLACK_);
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



/* Boss Phase func */
void movePlayer()
{
	static int playerSpeed = 1, oldTime;
	if (playerSpeed == 0)
	{
		if (oldTime == 0)
		{
			oldTime = clock();
		}
		else if (30 < clock() - oldTime)
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
			playerPos.X -= playerSpeed;
			playerSpeed = 0;
		}	
		else if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(0x44))
		{
			playerPos.X += playerSpeed;
			playerSpeed = 0;
		}
		if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(0x57))
		{
			playerPos.Y -= playerSpeed;
			playerSpeed = 0;
		}
		else if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(0x53))
		{
			playerPos.Y += playerSpeed;
			playerSpeed = 0;
		}
	}
}

void fireBlastToCenter(BlastAngle blastAngle)
{
	char* blast;
	int x, y;
	
	switch (blastAngle)
	{
		case _BLAST_BOT_LEFT_:
			x = bossPhaseBox.x - 20;
			y = bossPhaseBox.y - 8;
			break;
		case _BLAST_BOT_CENTER_:
			x = bossPhaseBox.x + (bossPhaseBox.w / 2);
			y = bossPhaseBox.y - 8;
			break;
		case _BLAST_BOT_RIGHT_:
			x = bossPhaseBox.x + bossPhaseBox.w + 5;
			y = bossPhaseBox.y - 8;
			break;
		
		case _BLAST_MID_LEFT_:
			x = bossPhaseBox.x - 20;
			y = bossPhaseBox.y + (bossPhaseBox.h / 2);
			break;
//		case _BLAST_MID_CENTER_:
//			x = bossPhaseBox.x + (bossPhaseBox.w / 2);
//			y = bossPhaseBox.y - 8;
//			break;
		case _BLAST_MID_RIGHT_:
			x = bossPhaseBox.x + bossPhaseBox.w + 5;
			y = bossPhaseBox.y + (bossPhaseBox.h / 2);
			break;
			
		case _BLAST_TOP_LEFT_:
			x = bossPhaseBox.x - 20;
			y = bossPhaseBox.y + bossPhaseBox.h + 2;
			break;
		case _BLAST_TOP_LEFT_:
			y = bossPhaseBox.y + bossPhaseBox.h + 2;
			y = bossPhaseBox.y - 8;
			break;
		case _BLAST_TOP_LEFT_:
			x = bossPhaseBox.x + bossPhaseBox.w + 5;
			y = bossPhaseBox.y + bossPhaseBox.h + 2;
			break;
	}
	fixBlastAngle(blast, blastAngle);
	
	free(blast);
}

void fireBlastToPlayer(int blastX, int blastY)
{
	int playerX = playerPos.X, playerY = playerPos.Y;
}

void fixBlastAngle(char* target, BlastAngle blastAngle)
{
	AssetFileType blastType = getBlastType(blastAngle);
	int angle = 0;
	char* rotate;
	size_t blastLen = sizeof(char) * strlen(AssetFile[blastType] + 1);
	
	target = (char*)malloc(blastLen);
	rotate = (char*)malloc(blastLen);
	strcpy(rotate, AssetFile[blastType]);
	
	switch (blastAngle)
	{
		// angle 0
		case _BLAST_TOP_CENTER_:	// vertical
		case _BLAST_TOP_RIGHT_:		// diagonal
			angle = 0;
			break;
		// angle 90
		case _BLAST_MID_RIGHT_:
		case _BLAST_BOT_RIGHT_:
			angle = 90;
			break;
		// angle 180
		case _BLAST_BOT_CENTER_:
		case _BLAST_TOP_LEFT_:
			angle = 180;
			break;
		// angle 270
		case _BLAST_MID_LEFT_:
		case _BLAST_BOT_LEFT_:
			angle = 270;
			break;
		
	}
	
	// string rotation 
	
	
	strcpy(target, rotate);
	
	free(rotate);
}

AssetFileType getBlastType(BlastAngle blastAngle)
{
	switch (blastAngle)
	{
		case _BLAST_MID_LEFT_:
//		case _BLAST_MID_CENTER_:
		case _BLAST_MID_RIGHT_:
		case _BLAST_BOT_CENTER_:
		case _BLAST_TOP_CENTER_:
			return _SANS_BLAST_VERTICAL_A_;
		
		case _BLAST_TOP_LEFT_:
		case _BLAST_TOP_RIGHT_:
		case _BLAST_BOT_LEFT_:
		case _BLAST_BOT_RIGHT_:
			return _SANS_BLAST_DIAGONAL_A_;
	}
	return -1;
}