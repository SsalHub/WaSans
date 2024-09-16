#include "sans_battle.h"


/* Main func in sans battle */
void initSansBattle()
{
	BattleObject sans[3];
	patternIdx = 0;
    scriptIdx = -1;

	// init battle
	initSansPattern();
	initSansObject(&sans);
	initBattle(1, &sans, _SANS_PATTERN_LEN_, sansPattern);
	// init speech bubble
	SpeechBubble[_ENEMY_SANS_].x = 74;
	SpeechBubble[_ENEMY_SANS_].y = 2;
	SpeechBubble[_ENEMY_SANS_].tColor = _BLACK_;
	SpeechBubble[_ENEMY_SANS_].bColor = _WHITE_;
	SpeechBubble[_ENEMY_SANS_].width = 24;
	SpeechBubble[_ENEMY_SANS_].height = 6;
	SpeechBubble[_ENEMY_SANS_].isActive = 0;
}

void initSansObject(BattleObject (*enemy)[3])
{
	// init leg
	(*enemy)[_ENEMY_LEG_].x = 52;
	(*enemy)[_ENEMY_LEG_].y = 13;
	(*enemy)[_ENEMY_LEG_].data = AssetFile[_SANS_LEG_NORMAL_];
	(*enemy)[_ENEMY_LEG_].tColor = _WHITE_;
	(*enemy)[_ENEMY_LEG_].bColor = _BLACK_;
	(*enemy)[_ENEMY_LEG_].isActive = 1;
	// init body
	(*enemy)[_ENEMY_BODY_].x = 50;
	(*enemy)[_ENEMY_BODY_].y = 9;
	(*enemy)[_ENEMY_BODY_].data = AssetFile[_SANS_BODY_NORMAL_];
	(*enemy)[_ENEMY_BODY_].tColor = _WHITE_;
	(*enemy)[_ENEMY_BODY_].bColor = _BLACK_;
	(*enemy)[_ENEMY_BODY_].isActive = 1;
	// init face
	(*enemy)[_ENEMY_FACE_].x = 51;
	(*enemy)[_ENEMY_FACE_].y = 0;
	(*enemy)[_ENEMY_FACE_].data = AssetFile[_SANS_FACE_NORMAL_A_];
	(*enemy)[_ENEMY_FACE_].tColor = _WHITE_;
	(*enemy)[_ENEMY_FACE_].bColor = _BLACK_;
	(*enemy)[_ENEMY_FACE_].isActive = 1;
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
	sansPattern[0].pattern = (Pattern)fireBlastToCenter;
	sansPattern[0].data = &(gasterBlasterPatternInfo[0]);
	sansPattern[1].pattern = (Pattern)fireBlastToCenter;
	sansPattern[1].data = &(gasterBlasterPatternInfo[1]);
}

void runSansBattle()
{
    introPhase();
    enemyPhase();
	// main phase
	while (1)
	{
	    playerPhase();
	    if (getCurrentScene() != _SCENE_SANS_BATTLE_)
	    	break;
	    enemyPhase();
	}
	
	// exit battle scene
	releasePatterns();
	releaseBattleAssets();
	gotoNextScene(_SCENE_MAINMENU_);
}



/* Each Phase Func */
static void introPhase()
{
    const int introScriptLen = 3;
    int speechFlag = 0;
	scriptIdx = 0;
    
	sleep(1.0f);
	// run intro phase
    playBGM(_BGM_BIRDNOISE_, _SOUND_BEGIN_);
    fadeIn(getSceneRenderer(_SCENE_SANS_BATTLE_));
    
	setSansFace(_SANS_FACE_NORMAL_A_);
	sleep(1.0f);
	while (scriptIdx < introScriptLen)
	{
		speechFlag = writeSpeechBubble(scripts[scriptIdx], _BLACK_, 1);
    	waitForFrame();
		if (speechFlag < 0)
			scriptIdx++;
	}
	SpeechBubble[_ENEMY_SANS_].isActive = 0;
	
	// end intro phase
    playBGM(_BGM_BIRDNOISE_, _SOUND_PAUSE_);
}

static void playerPhase()
{
	int select = -1;
	switch (getBattleTurn())
	{
		case 1:
			playBGM(_BGM_MEGALOVANIA_, _SOUND_BEGIN_);
			sleep(0.8f);
			
			while (1)
			{
				select = movePlayerSelectBox();
				switch (select)
				{
					case -1:
						break;
						
					case 0:
						if (!getPlayerDamage(5))
						{	
							// game over
							playBGM(_BGM_MEGALOVANIA_, _SOUND_PAUSE_);
							gotoNextScene(_SCENE_MAINMENU_);
							return;
						}
						break;
					case 1:
						break;
					case 2:
						break;
					case 3:
						// select mercy
						playBGM(_BGM_MEGALOVANIA_, _SOUND_PAUSE_);
						gotoNextScene(_SCENE_MAINMENU_);
						return;	
				}
				waitForFrame();
			}
			break;
		case 2:
			break;
	}
	gotoNextPhase();
}

static void enemyPhase()
{
	const int introScriptIdx_A = 4, introScriptIdx_B = 5;
	int i, speechFlag = 0;
    PlayerPos.X = 59;
    PlayerPos.Y = 19;
    
    switch (getBattleTurn())
    {
	    case 0: // intro turn
			setSansFace(_SANS_FACE_NORMAL_B_);
			playSFX(_SFX_MOMENT_);
			blackScreenEffect(1.0f);
	    	setBattlePhase(_ENEMY_PHASE_);
	    	playSFX(_SFX_MOMENT_);
	    	
	    	// run script
	    	sleep(1.0f);
			while (scriptIdx < introScriptIdx_A)
			{
				speechFlag = writeSpeechBubble(scripts[scriptIdx], _RED_, 0);
	        	movePlayerPos();
    			waitForFrame();
				if (speechFlag < 0)
					scriptIdx++;
			}
			SpeechBubble[_ENEMY_SANS_].isActive = 0;
			sleep(0.1f);
	    	
        	// run boss pattern
			setSansFace(_SANS_FACE_NORMAL_A_);
			runSansPattern(0);
			runSansPattern(1);
			// wait until all pattern completed
	        while (isAnyPatternAlive())
	        {
	        	movePlayerPos();
	        	waitForFrame();
	        }
	        CloseHandle(sansPattern[0].hThread);
	        CloseHandle(sansPattern[1].hThread);
	        sleep(1.0f);
			while (scriptIdx < introScriptIdx_B)
			{
				speechFlag = writeSpeechBubble(scripts[scriptIdx], _BLACK_, 0);
	        	movePlayerPos();
    			waitForFrame();
				if (speechFlag < 0)
					scriptIdx++;
			}
			SpeechBubble[_ENEMY_SANS_].isActive = 0;
			sleep(0.1f);
	        break;
	    
	    case 1:
			setSansFace(_SANS_FACE_NORMAL_A_);
	        while (1)
	        {
	        	movePlayerPos();
	        }
	        break;
    }
	gotoNextPhase();
}



/* Main Renderer */
//void renderSansBattle()
//{
//    renderSans(_SANS_FACE_NORMAL_A_);
//
//    //	renderBossPhaseBox();
////    renderPlayerPhaseBox();
//
//    renderPlayerInfo();
////    renderSelectBox();
//}

//void renderIntroPhase()
//{
//    static int oldTime = 0;
//    int flag;
//
//    renderSans(_SANS_FACE_NORMAL_A_);
//    renderPlayerInfo();
//    // render speech bubble
//    if (scriptIdx < 0) // waiting script before speech
//    {
//        if (oldTime == 0)
//        {
//            oldTime = clock();
//        }
//        else if (2000 < clock() - oldTime)
//        {
//			scriptIdx = 0;
//	        oldTime = 0;
//        }
//    }
//    else
//    {
//	    flag = renderSpeechBubble();
//	    if (flag < 0)
//	    {
//	        if (!oldTime)
//	        {
//	            oldTime = clock();
//	        }
//	        else if (2000 < clock() - oldTime)
//	        {
//	            scriptIdx++;
//	            oldTime = 0;
//	        }
//	    }
//	}
//}

//void renderBossPhase()
//{
//    static int oldTime = 0, bWait = 1;
//	const int introScriptLen = 4;
//	int flag;
//	
//	// render sans face
//	if (battleTurn == -1)
//	{
//		renderSans(_SANS_FACE_NORMAL_B_);
//	}
//	else
//	{
//		renderSans(_SANS_FACE_NORMAL_A_);
//	}
//    renderBossPhaseBox();
//    renderPlayerInfo();
//    // render speech bubble
//	if (battleTurn == -1)
//	{
//		if (bWait)	// wait before rendering speech bubble
//		{
//	        if (oldTime == 0)
//	        {
//	            oldTime = clock();
//	        }
//	        else if (1000 < clock() - oldTime)
//	        {
//	        	bWait = 0;
//	        	oldTime = 0;
//	        }
//		}
//		else if (scriptIdx < introScriptLen)
//		{
//			flag = renderSpeechBubble(scripts[scriptIdx], _RED_, 0);
//			if (flag < 0)
//		    {
//		        if (oldTime == 0)
//		        {
//		            oldTime = clock();
//		        }
//		        else if (2000 < clock() - oldTime)
//		        {
//		            scriptIdx++;
//		            oldTime = 0;
//		        }
//		    }
//		}
//	}
//	renderPattern();
//}

//void renderPlayerPhase()
//{
//    renderSans(_SANS_FACE_NORMAL_A_);
//    renderPlayerPhaseBox();
//    renderPlayerInfo();
//    renderSelectBox();
//}



/* Sub Renderer */
//void renderSans(AssetType face)
//{
//    const int MAX_TICK = 12;
//    static int tick, oldTime;
//    int face_move[12][2] = {{0, 0}, {1, 0}, {1, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
//        body_move[12][2] = {{1, 0}, {1, 0}, {1, 0}, {1, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
//        leg_move[12][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
//    int x = 50, y = 0, currTime;
//
//    // leg
//    //	printLines(x + 1 + leg_move[tick][0], y + 11 + leg_move[tick][1], AssetFile[_SANS_LEG_NORMAL_], _WHITE_);
//    printLines(x + 2, y + 13, AssetFile[_SANS_LEG_NORMAL_], _WHITE_, _BLACK_);
//    // body
//    //	printLines(x + body_move[tick][0], y + 7 + body_move[tick][1], AssetFile[_SANS_BODY_NORMAL_], _WHITE_);
//    printLines(x, y + 9, AssetFile[_SANS_BODY_NORMAL_], _WHITE_, _BLACK_);
//    // face
//    //	printLines(x + face_move[tick][0], y + face_move[tick][1], AssetFile[face], _WHITE_);
//    printLines(x + 1, y, AssetFile[face], _WHITE_, _BLACK_);
//
//    // set ticks based on current/old time
//    if (oldTime)
//    {
//        currTime = clock();
//        if (100 <= currTime - oldTime) // 100ms == 0.1sec
//        {
//            tick = MAX_TICK <= tick + 1 ? 0 : tick + 1;
//            oldTime = currTime;
//        }
//    }
//    else
//    {
//        oldTime = clock();
//    }
//}

int writeSpeechBubble(const char* script, ConsoleColor tColor, int bVoice)
{
    static int currLen = 0, oldTime = 0, readOver = 0;
    static const char *pScript = NULL;
    char buffer[128], ch[3], input;
	int i, j, currTime, slen = strlen(script);

	// init data
    if (pScript != script)
    {
        pScript = script;
		readOver = 0;
        currLen = 1;
        oldTime = clock() - 1000;
		SpeechBubble[_ENEMY_SANS_].isActive = 1;
		SpeechBubble[_ENEMY_SANS_].tColor = tColor;
    }
    
    // if already script read fully
	if (readOver)
	{
		if (readOver < clock())
			return -1;
		return currLen;
	}
	// if not enough time passed
    currTime = clock();
    if (currTime - oldTime < 120)
    	return currLen;
    	
	// main action
    if (kbhit())
    {
        input = getch();
        if (input == _SPACE_ || input == _CARRIAGE_RETURN_)
        {
        	if (readOver)
        		return -1;
			currLen = slen;
			readOver = clock() + 1500;
		}
		else
		{
			if (slen <= currLen)
	    	{
				readOver = currTime + 1500;
				return currLen;
			}
			currLen++;
	        oldTime = currTime;
		}
    }
    else
    {
		if (slen <= currLen)
    	{
			readOver = currTime + 1500;
			return currLen;
		}
		currLen++;
        oldTime = currTime;
	}
    // write script on 'SpeechBubble' until currLen
    memcpy(buffer, script, currLen);
    buffer[currLen] = '\0';
    memcpy(SpeechBubble[_ENEMY_SANS_].data, buffer, currLen + 1);
    if (bVoice)
		playVoice(_VOICE_SANS_);
    return currLen;
}

//void renderBossPhaseBox()
//{
//    int i, j;
//    char buffer[(ScreenWidth + 1) * (ScreenHeight / 2)], ch[8];
//
//    // print boss phase box
//    buffer[0] = '\0';
//    for (i = 0; i < EnemyPhaseBox.height; i++)
//    {
//        strcat(buffer, ":=");
//        if (i == 0 || i == EnemyPhaseBox.height - 1)
//            strcpy(ch, "=");
//        else
//            strcpy(ch, " ");
//        for (j = 0; j < EnemyPhaseBox.width; j++)
//            strcat(buffer, ch);
//        strcat(buffer, "=: \n");
//    }
//    printLines(EnemyPhaseBox.x, EnemyPhaseBox.y, buffer, _WHITE_, _BLACK_);
//
//    // print player
//    // fix player x pos
//    if (PlayerPos.X <= EnemyPhaseBox.x + 2)
//        PlayerPos.X = EnemyPhaseBox.x + 2;
//    else if (EnemyPhaseBox.x + 1 + EnemyPhaseBox.width <= PlayerPos.X)
//        PlayerPos.X = EnemyPhaseBox.x + 1 + EnemyPhaseBox.width;
//    // fix player y pos
//    if (PlayerPos.Y <= EnemyPhaseBox.y)
//        PlayerPos.Y = EnemyPhaseBox.y + 1;
//    else if (EnemyPhaseBox.y + EnemyPhaseBox.height - 2 <= PlayerPos.Y)
//        PlayerPos.Y = EnemyPhaseBox.y + EnemyPhaseBox.height - 2;
//    strcpy(ch, "♥");
//    printLine(PlayerPos.X, PlayerPos.Y, ch, _HOTPINK_, _BLACK_);
//}

//void renderPlayerPhaseBox()
//{
//    int x = 6, y = 16, w = 103, h = 8, i, j;
//    char buffer[(ScreenWidth + 1) * (ScreenHeight / 2)], ch[3];
//
//    buffer[0] = '\0';
//    for (i = 0; i < h; i++)
//    {
//        strcat(buffer, ":=");
//        if (i == 0 || i == h - 1)
//            strcpy(ch, "=");
//        else
//            strcpy(ch, " ");
//        for (j = 0; j < w; j++)
//            strcat(buffer, ch);
//        strcat(buffer, "=: \n");
//    }
//    printLines(x, y, buffer, _WHITE_, _BLACK_);
//}

//void renderPlayerInfo()
//{
//    int x, y = 24, i, damaged;
//    char hpText[11];
//
//    // render player info
//    x = 12;
//    printLine(x, y, PlayerName, _WHITE_, _BLACK_);
//    x = 26;
//    printLine(x, y, "LV 1", _WHITE_, _BLACK_);
//
//    // render HP info
//    // set max HP text
//    for (int i = 0; i < 10; i++)
//        hpText[i] = '@';
//    hpText[10] = '\0';
//    x = 43;
//    printLine(x, y, "HP", _WHITE_, _BLACK_);
//    x += 3;
//    printLine(x, y, hpText, _YELLOW_, _BLACK_);
//    damaged = (MaxHP - playerHP) / 10;
//    // set player HP info
//    for (i = 0; i < damaged; i++)
//        hpText[i] = '#';
//    hpText[damaged] = '\0';
//    printLine(x + 10 - damaged, y, hpText, _RED_, _BLACK_);
//
//    // render numeric HP info
//    x += 12;
//    itoa(playerHP, hpText, 10);
//    printLine(x, y, hpText, _WHITE_, _BLACK_);
//    x += strlen(hpText);
//    printLine(x, y, " / ", _WHITE_, _BLACK_);
//    x += 3;
//    itoa(MaxHP, hpText, 10);
//    printLine(x, y, hpText, _WHITE_, _BLACK_);
//}

//void renderSelectBox()
//{
//    int x, y = 25;
//    ConsoleColor tSelect[4] = {_YELLOW_, _YELLOW_, _YELLOW_, _YELLOW_};
//    tSelect[battleSelect] = _LIGHT_YELLOW_;
//    x = 7;
//    printLines(x, y, AssetFile[_SELECT_BOX_], tSelect[0], _BLACK_);
//    printLine(x + 8, y + 2, "FIGHT", tSelect[0], _BLACK_);
//    x += 27;
//    printLines(35, y, AssetFile[_SELECT_BOX_], tSelect[1], _BLACK_);
//    printLine(x + 11, y + 2, "ACT", tSelect[1], _BLACK_);
//    x += 27;
//    printLines(63, y, AssetFile[_SELECT_BOX_], tSelect[2], _BLACK_);
//    printLine(x + 11, y + 2, "ITEM", tSelect[2], _BLACK_);
//    x += 27;
//    printLines(91, y, AssetFile[_SELECT_BOX_], tSelect[3], _BLACK_);
//    printLine(x + 11, y + 2, "MERCY", tSelect[3], _BLACK_);
//}

//void renderPattern()
//{
//	int i, j;
//	for (i = 0; i < patternIdx; i++)
//	{
//		for (j = 0; j < sansPattern[i].renderInfoLen; j++)
//		{
//			printLines(
//				sansPattern[i].renderInfo[j].x,
//				sansPattern[i].renderInfo[j].y,
//				sansPattern[i].renderInfo[j].s,
//				sansPattern[i].renderInfo[j].tColor,
//				sansPattern[i].renderInfo[j].bColor
//			);
//		}
//	}
//}



/* Boss Phase func */
unsigned __stdcall fireBlastToCenter(void* args)
{
	// receive args
	PatternArgs_Blaster *data = (PatternArgs_Blaster*)args;
	int pId = data->patternId;
	BlasterAngle blasterAngle = data->blasterAngle;
	// other vars
	AssetType blasterType = getBlastType(blasterAngle);
	int oldTime, renderInfoIdx = 0, blastId = 0;
	char chargeBlaster[16];
	COORD pos, begin, end;
	float t;
	
	strcpy(chargeBlaster, "   \n   \n   ");
	
	switch (blasterAngle)
	{
		// angle 0
		case _BLAST_TOP_CENTER_:
			end.X 		= EnemyPhaseBox.x + (EnemyPhaseBox.width / 2) - 5;
			end.Y 		= EnemyPhaseBox.y - 9;
			begin.X 	= end.X - 18;
			begin.Y 	= end.Y;
			break;
		case _BLAST_TOP_RIGHT_:
			end.X = EnemyPhaseBox.x + EnemyPhaseBox.width + 5;
			end.Y = EnemyPhaseBox.y - 9;
			break;
			
		// angle 90
		case _BLAST_MID_RIGHT_:
			end.X 		= EnemyPhaseBox.x + EnemyPhaseBox.width + 10;
			end.Y 		= EnemyPhaseBox.y + (EnemyPhaseBox.height / 2) - 3;
			begin.X 	= end.X;
			begin.Y 	= end.Y - 5;
			break;
		case _BLAST_BOT_RIGHT_:
			end.X = EnemyPhaseBox.x + EnemyPhaseBox.width + 5;
			end.Y = EnemyPhaseBox.y + EnemyPhaseBox.height + 2;
			break;
			
		// angle 180
		case _BLAST_BOT_CENTER_:
			end.X = EnemyPhaseBox.x + (EnemyPhaseBox.width / 2) - 5;
			end.Y = EnemyPhaseBox.y + EnemyPhaseBox.height - 2;
			begin.X 	= end.X + 18;
			begin.Y 	= end.Y;
			break;
		case _BLAST_BOT_LEFT_:
			end.X = EnemyPhaseBox.x - 20;
			end.Y = EnemyPhaseBox.y + (EnemyPhaseBox.height / 2) - 5;
			begin.X 	= end.X;
			begin.Y 	= end.Y + 5;
			break;
			
		// angle 270
		case _BLAST_MID_LEFT_:
			end.X = EnemyPhaseBox.x - 23;
			end.Y = EnemyPhaseBox.y + (EnemyPhaseBox.height / 2) - 3;
			begin.X 	= end.X;
			begin.Y 	= end.Y + 5;
			break;
		case _BLAST_TOP_LEFT_:
			end.X = EnemyPhaseBox.x - 20;
			end.Y = EnemyPhaseBox.y - 8;
			break;
	}
	
	oldTime = clock();
	setRenderInfo(
		&(sansPattern[pId].renderInfo[renderInfoIdx]), 
		begin.X,
		begin.Y,
		AssetFile[blasterType],
		_WHITE_,
		_BLACK_
	);
	sansPattern[pId].renderInfoLen = 1;
	playSFXOnThread(_SFX_GASTERBLASTER_);
	
	while (t < 1)
	{
		t = (clock() - oldTime) / 3000.0f;
		blastId = (int)(t / 0.05f);
		if (4 < blastId)
			blastId = 4;
			
		if (t < 0.1f)
		{
			pos.X = (int)lerp(begin.X, end.X, t * 10);
			pos.Y = (int)lerp(begin.Y, end.Y, t * 10);
			setRenderInfo(
				&(sansPattern[pId].renderInfo[renderInfoIdx]), 
				pos.X,
				pos.Y,
				AssetFile[blasterType + blastId],
				_WHITE_,
				_BLACK_
			);
			sansPattern[pId].renderInfoLen = 1;
			continue;
		}
		if (t < 0.2f)
		{
			renderInfoIdx = 0;
			setRenderInfo(
				&(sansPattern[pId].renderInfo[renderInfoIdx]), 
				end.X,
				end.Y,
				AssetFile[blasterType + blastId],
				_WHITE_,
				_BLACK_
			);
			renderInfoIdx++;
			setRenderInfoAttr(
				&(sansPattern[pId].renderInfo[renderInfoIdx]), 
				end.X + 5,
				end.Y + 2,
				8,
				2,
				_WHITE_,
				_GRAY_
			);
			renderInfoIdx++;
			sansPattern[pId].renderInfoLen = renderInfoIdx;
			continue;
		}
		
		waitForFrame();
	}
	return 0;
	//
//	while (t < 1)
//	{
//		t = (clock() - oldTime) / 400.0f;
//		blastId = (int)(t / 0.4f);
//		
//		setRenderInfo(
//			&(sansPattern[pId].renderInfo[renderInfoIdx]), 
//			(int)lerp(beginX, targetX, t),
//			(int)lerp(beginY, targetY, t),
//			AssetFile[blasterType + blastId],
//			_WHITE_,
//			_BLACK_
//		);
//		waitForFrame();
//	}
//	sleep(0.2f);
//	blastId = 3;
//	setRenderInfo(
//		&(sansPattern[pId].renderInfo[renderInfoIdx]), 
//		targetX,
//		targetY,
//		AssetFile[blasterType + blastId],
//		_WHITE_,
//		_BLACK_
//	);
//	sleep(0.6f);
//	blastId = 4;
//	setRenderInfo(
//		&(sansPattern[pId].renderInfo[renderInfoIdx]), 
//		targetX,
//		targetY,
//		AssetFile[blasterType + blastId],
//		_WHITE_,
//		_BLACK_
//	);
//	sleep(3.0f);
}

unsigned __stdcall fireBlastToPlayer(void* args)
{
	// receive args
	PatternArgs_Blaster *data = (PatternArgs_Blaster*)args;
	int pId = data->patternId;
	BlasterAngle blasterAngle = data->blasterAngle;
	// other vars
	int playerX = PlayerPos.X, playerY = PlayerPos.Y;
}

int explodeBlaster(BlasterAngle angle, int pId, COORD begin, COORD end, ConsoleColor bColor)	
{
	if (end.X < begin.X)
	{
		COORD tmp = begin;
		begin = end;
		end = tmp;
	}
	const int blasterWidth = 3;
	char line[ScreenWidth + 1], block[4];
	int width, height;
	int i = 0;
	
	if (strlen(block) < 1)
		strcpy(block, "   ");
	
	switch (angle)
	{
		// vertical
		case _BLAST_TOP_CENTER_:
		case _BLAST_BOT_CENTER_:
			width = blasterWidth;
			height = (end.Y - begin.Y) + 1;
			
			for (i = 0; i < height; i++)
			{
				setRenderInfo(
					&(sansPattern[pId].renderInfo[i]), 
					begin.X,
					begin.Y + i,
					block,
					_WHITE_,
					bColor
				);
			}
			sansPattern[pId].renderInfoLen = height;
			break;
		
		// horizontal
		case _BLAST_MID_RIGHT_:
		case _BLAST_MID_LEFT_:
			width = (end.X - begin.X) + 1;
			height = blasterWidth;
			
			for (i = 0; i < width; i++)
				line[i] = ' ';
			line[width] = '\0';
			for (i = 0; i < height; i++)
			{
				setRenderInfo(
					&(sansPattern[pId].renderInfo[i]), 
					begin.X,
					begin.Y + i - 1,
					line,
					_WHITE_,
					bColor
				);
			}
			sansPattern[pId].renderInfoLen = height;
			break;
			
		// diagonal
		case _BLAST_TOP_RIGHT_:
		case _BLAST_BOT_RIGHT_:
		case _BLAST_BOT_LEFT_:
		case _BLAST_TOP_LEFT_:
			width = blasterWidth;
			height = (end.Y - begin.Y) + 1;
			
			for (i = 0; i < height; i++)
			{
				setRenderInfo(
					&(sansPattern[pId].renderInfo[i]), 
					lerp(begin.X, end.X, i / (float)height),
					begin.Y + i,
					block,
					_WHITE_,
					bColor
				);
			}
			sansPattern[pId].renderInfoLen = height;
			break; 
	}
	sansPattern[pId].renderInfoLen = i;
	return i;
}

AssetType getBlastType(BlasterAngle blasterAngle)
{
	switch (blasterAngle)
	{
		case _BLAST_TOP_CENTER_:
			return _SANS_BLASTER_VERT_0A_;
		case _BLAST_MID_RIGHT_:
			return _SANS_BLASTER_VERT_90A_;
		case _BLAST_BOT_CENTER_:
			return _SANS_BLASTER_VERT_180A_;
		case _BLAST_MID_LEFT_:
			return _SANS_BLASTER_VERT_270A_;
		
		case _BLAST_TOP_RIGHT_:
			return _SANS_BLASTER_DIAG_0A_;
		case _BLAST_BOT_RIGHT_:
			return _SANS_BLASTER_DIAG_90A_;
		case _BLAST_BOT_LEFT_:
			return _SANS_BLASTER_DIAG_180A_;
		case _BLAST_TOP_LEFT_:
			return _SANS_BLASTER_DIAG_270A_;
	}
	return -1;
}

char* fixBlastAngle(char* dst, size_t dstSize, BlasterAngle blasterAngle)
{
	AssetType blasterType = getBlastType(blasterAngle);
	dst = AssetFile[blasterType];
	
	// string rotation 
	return dst;
}

void runSansPattern(int pid)
{
	static int patternId = 0;
	patternId = pid < 0 ? patternId : pid;
	sansPattern[patternId].hThread = startPattern(
			sansPattern[patternId].pattern,
			sansPattern[patternId].data,
			&(sansPattern[patternId].threadID));
			
	GetExitCodeThread(sansPattern[patternId].hThread, &(sansPattern[patternId].isActive));
	patternId++;
}

int isAnyPatternAlive()
{
	int i;
    for (i = 0; i < _SANS_PATTERN_LEN_; i++)
    {
    	GetExitCodeThread(sansPattern[i].hThread, &(sansPattern[i].isActive));
    	if (sansPattern[i].isActive == STILL_ACTIVE)
    		return 1;
	}
	return 0;
}



/* etc */
void setSansFace(AssetType facetype)
{
	EnemyInfo[0][_ENEMY_FACE_].data = AssetFile[facetype];
}

/* Terminate Func */
void releasePatterns()
{
	int i;
	for (i = 0; i < _SANS_PATTERN_LEN_; i++)
	{
		CloseHandle(sansPattern[i].hThread);
	}
}