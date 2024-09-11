#include "sans_battle.h"

//void Start()
//{
//	initSansBattle();
//}
//
//void Update()
//{
//	runSansBattle();
//}

/* Main func in sans battle */
void initSansBattle()
{
    playerHP = MaxHP;
    battleTurn = 0;
    battleSelect = 0;
    scriptIdx = -1;
    patternIdx = 0;
	
	///////
//    introPhase();
//    bossPhase();
//    battleTurn++;
	initSansPattern();
	
    sleep(1.0f);
    playBGM(_BGM_BIRDNOISE_, _SOUND_BEGIN_);
    fadeIn(renderSansBattle);
    introPhase();
}

void initSansPattern()
{
	int i;
	// init basic info
	for (i = 0; i < _SANS_PATTERN_LEN_; i++)
	{
		sansPattern[i].hThread = NULL;
		sansPattern[i].threadID = 0;
		sansPattern[i].isActive = 0;
		sansPattern[i].renderInfoLen = 0;
	}
	// init detail info
	sansPattern[0].pattern = (unsigned __stdcall (*)(int))fireBlastToCenter;
	sansPattern[0].data = (int)_BLAST_MID_RIGHT_;
	sansPattern[1].pattern = (unsigned __stdcall (*)(int))fireBlastToCenter;
	sansPattern[1].data = (int)_BLAST_MID_LEFT_;
}

void runSansBattle()
{
    char input;

    // Enter scene
    playerPhase();
    bossPhase();
    battleTurn++;

//    if (kbhit())
//    {
//        input = getch();
//        switch (input)
//        {
//        case _LEFT_:
//        case 'A':
//        case 'a':
//            0 < battleSelect ? battleSelect - 1 : battleSelect;
//            break;
//
//        case _RIGHT_:
//        case 'D':
//        case 'd':
//            battleSelect < _BATTLE_SELECT_LEN_ - 1 ? battleSelect + 1 : battleSelect;
//            break;
//
//        case _SPACE_:
//        case _CARRIAGE_RETURN_:
//        	switch (battleSelect)
//        	{
//        		case 0:
//        			if (0 < playerHP)
//        				playerHP -= 5;
//        			break;
//        		case 1:
//        			break;
//        		case 2:
//        			break;
//        		case 3:
//					playBGM(_BGM_MEGALOVANIA_, _SOUND_PAUSE_);
//					gotoMainmenuScene();
//	                return;
//			}
//			break;
//        }
//    }
}

/* Each Phase Func */
void introPhase()
{
    const int introScriptLen = 3;
    
	setSceneRenderer(renderIntroPhase);
    while (scriptIdx < introScriptLen)
    {
        waitForFrame();
	}
	setSceneRenderer(renderIntroPhase);
    playBGM(_BGM_BIRDNOISE_, _SOUND_PAUSE_);
    bossPhase();
}

void bossPhase()
{
	const int indexScriptLen = 4;
	int i;
    playerPos.X = 59;
    playerPos.Y = 19;
    
    switch (battleTurn)
    {
	    case 0: // intro turn
			playSFX(_SFX_MOMENT_);
			blackScreenEffect(1.0f);
			setSceneRenderer(renderBossPhase);
	    	playSFX(_SFX_MOMENT_);
	    	
        	// run boss pattern
			if (patternIdx < _SANS_PATTERN_LEN_ && indexScriptLen <= scriptIdx)
			{
				// run pattern 0
				sansPattern[patternIdx].hThread = startPattern(
					sansPattern[patternIdx].pattern,
					&(sansPattern[patternIdx].data),
					&(sansPattern[patternIdx].threadID)
				);
				patternIdx++;
				// run pattern 1
				sansPattern[patternIdx].hThread = startPattern(
					sansPattern[patternIdx].pattern,
					&(sansPattern[patternIdx].data),
					&(sansPattern[patternIdx].threadID)
				);
				patternIdx++;
			}
			// wait until all pattern completed
	        while (1)
	        {
	            for (i = 0; i < _SANS_PATTERN_LEN_; i++)
	            {
	            	GetExitCodeThread(sansPattern[i].hThread, &(sansPattern[i].isActive));
	            	if (sansPattern[i].isActive == STILL_ACTIVE)
	            		break;
				}
				if (patternIdx != 0 && _SANS_PATTERN_LEN_ <= i)
					break;
					
	        	movePlayer();
	        	waitForFrame();
	        }
    		releasePattern();
	        break;
	        
	    case 1:
//	        while (1)
//	        {
//	        	movePlayer();
//	            renderCustom(renderBossPhase);
//	        }
			setSceneRenderer(renderPlayerPhase);
	        break;
    }
}

void playerPhase()
{
	switch (battleTurn)
	{
		case 0:
			playBGM(_BGM_MEGALOVANIA_, _SOUND_BEGIN_);
			break;
			
		case 1:
			break;
	}
}

/* Main Renderer */
void renderSansBattle()
{
    renderSans(_SANS_FACE_NORMAL_A_);

    //	renderBossPhaseBox();
//    renderPlayerPhaseBox();

    renderPlayerInfo();
//    renderSelectBox();
}

void renderIntroPhase()
{
    static int oldTime = 0;
    int flag;

    renderSans(_SANS_FACE_NORMAL_A_);
    renderPlayerInfo();
    // render speech bubble
    if (scriptIdx < 0) // waiting script before speech
    {
        if (oldTime == 0)
        {
            oldTime = clock();
        }
        else if (2000 < clock() - oldTime)
        {
			scriptIdx = 0;
	        oldTime = 0;
        }
    }
    else
    {
	    flag = renderSpeechBubble(scripts[scriptIdx], _BLACK_, 1);
	    if (flag < 0)
	    {
	        if (!oldTime)
	        {
	            oldTime = clock();
	        }
	        else if (2000 < clock() - oldTime)
	        {
	            scriptIdx++;
	            oldTime = 0;
	        }
	    }
	}
}

void renderBossPhase()
{
    static int oldTime = 0, bWait = 1;
	const int introScriptLen = 4;
	int flag;
	
	// render sans face
	if (battleTurn == -1)
	{
		renderSans(_SANS_FACE_NORMAL_B_);
	}
	else
	{
		renderSans(_SANS_FACE_NORMAL_A_);
	}
    renderBossPhaseBox();
    renderPlayerInfo();
    // render speech bubble
	if (battleTurn == -1)
	{
		if (bWait)	// wait before rendering speech bubble
		{
	        if (oldTime == 0)
	        {
	            oldTime = clock();
	        }
	        else if (1000 < clock() - oldTime)
	        {
	        	bWait = 0;
	        	oldTime = 0;
	        }
		}
		else if (scriptIdx < introScriptLen)
		{
			flag = renderSpeechBubble(scripts[scriptIdx], _RED_, 0);
			if (flag < 0)
		    {
		        if (oldTime == 0)
		        {
		            oldTime = clock();
		        }
		        else if (2000 < clock() - oldTime)
		        {
		            scriptIdx++;
		            oldTime = 0;
		        }
		    }
		}
	}
	renderPattern();
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

int renderSpeechBubble(const char* script, ConsoleColor tColor, int bVoice)
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
        if (120 < currTime - oldTime)
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
    if (bVoice && 0 < currLen && script[currLen - 1] != ' ')
    	playVoiceOnThread(_VOICE_SANS_);
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
    strcpy(ch, "♥");
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

void renderPattern()
{
	int i, j;
	for (i = 0; i < patternIdx; i++)
	{
		for (j = 0; j < sansPattern[i].renderInfoLen; j++)
		{
			printLines(
				sansPattern[i].renderInfo[j].x,
				sansPattern[i].renderInfo[j].y,
				sansPattern[i].renderInfo[j].s,
				sansPattern[i].renderInfo[j].tColor,
				sansPattern[i].renderInfo[j].bColor
			);
		}
	}
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

unsigned __stdcall fireBlastToCenter(void* arg)
{
	int pId = getLastPatternIdx();
	if (pId < 0)
		return 1;
	BlastAngle blastAngle = *((BlastAngle*)arg);
	AssetFileType blastType = getBlastType(blastAngle);
	int targetX, targetY, beginX, beginY;
	int x, y, oldTime, renderInfoIdx = 0;
	char* blast;
	size_t blastSize;
	float t;
	
	blastSize = (sizeof(char) * strlen(AssetFile[blastType])) * 2;
	sansPattern[pId].renderInfo[renderInfoIdx].s = (char*)malloc(blastSize);
	blast = sansPattern[pId].renderInfo[renderInfoIdx].s;
	
	switch (blastAngle)
	{
		case _BLAST_TOP_CENTER_:
			targetX = bossPhaseBox.x + (bossPhaseBox.w / 2) - 5;
			targetY = bossPhaseBox.y - 9;
			beginX 	= targetX - 18;
			beginY 	= targetY;
			break;
		case _BLAST_TOP_RIGHT_:
			targetX = bossPhaseBox.x + bossPhaseBox.w + 5;
			targetY = bossPhaseBox.y - 9;
			break;
			
		case _BLAST_MID_RIGHT_:
			targetX = bossPhaseBox.x + bossPhaseBox.w + 8;
			targetY = bossPhaseBox.y + (bossPhaseBox.h / 2) - 5;
			beginX 	= targetX;
			beginY 	= targetY - 5;
			break;
		case _BLAST_BOT_RIGHT_:
			targetX = bossPhaseBox.x + bossPhaseBox.w + 5;
			targetY = bossPhaseBox.y + bossPhaseBox.h + 2;
			break;
			
		case _BLAST_BOT_CENTER_:
			targetX = bossPhaseBox.x + (bossPhaseBox.w / 2) - 5;
			targetY = bossPhaseBox.y + bossPhaseBox.h - 2;
			beginX 	= targetX + 18;
			beginY 	= targetY;
			break;
		case _BLAST_BOT_LEFT_:
			targetX = bossPhaseBox.x - 20;
			targetY = bossPhaseBox.y + (bossPhaseBox.h / 2) - 5;
			beginX 	= targetX;
			beginY 	= targetY + 5;
			break;
			
		case _BLAST_MID_LEFT_:
			targetX = bossPhaseBox.x - 18;
			targetY = bossPhaseBox.y + (bossPhaseBox.h / 2) - 5;
			beginX 	= targetX;
			beginY 	= targetY + 5;
			break;
		case _BLAST_TOP_LEFT_:
			targetX = bossPhaseBox.x - 20;
			targetY = bossPhaseBox.y - 8;
			break;
	}
	
	fixBlastAngle(blast, blastSize, blastAngle);
	sansPattern[pId].renderInfoLen = 1;
	
	playSFXOnThread(_SFX_GASTERBLASTER_);
	oldTime = clock();
	while (1)
	{
		t = (clock() - oldTime) / 400.0f;
		setRenderInfo(
			&(sansPattern[pId].renderInfo[renderInfoIdx]), 
			(int)lerp(beginX, targetX, t),
			(int)lerp(beginY, targetY, t),
			NULL,
			_WHITE_,
			_BLACK_
		);
		sleep(0.01f);
	}
	free(blast);
}

void fireBlastToPlayer(BlastAngle blastAngle)
{
	int pIdx = patternIdx - 1;
	int playerX = playerPos.X, playerY = playerPos.Y;
}

AssetFileType getBlastType(BlastAngle blastAngle)
{
	switch (blastAngle)
	{
		case _BLAST_TOP_CENTER_:
		case _BLAST_MID_RIGHT_:
		case _BLAST_BOT_CENTER_:
		case _BLAST_MID_LEFT_:
			return _SANS_BLAST_VERTICAL_A_;
		
		case _BLAST_TOP_RIGHT_:
		case _BLAST_BOT_RIGHT_:
		case _BLAST_BOT_LEFT_:
		case _BLAST_TOP_LEFT_:
			return _SANS_BLAST_DIAGONAL_A_;
	}
	return -1;
}

char* fixBlastAngle(char* dst, size_t dstSize, BlastAngle blastAngle)
{
	AssetFileType blastType = getBlastType(blastAngle);
	char* src;
	
	src = (char*)malloc(dstSize);
	strcpy(src, AssetFile[blastType]);
	
	// string rotation 
	rotateString(dst, src, blastAngle);
	free(src);
	return dst;
}

int getLastPatternIdx()
{
	static int idx = 0;
	return idx++;
}

void releasePattern()
{
	int i;
	for (i = 0; i < _SANS_PATTERN_LEN_; i++)
	{
		CloseHandle(sansPattern[i].hThread);
	}
}