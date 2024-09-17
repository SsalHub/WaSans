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
	int i, j;
	// init basic info
	for (i = 0; i < _SANS_PATTERN_LEN_; i++)
	{
		sansPattern[i].hThread = NULL;
		sansPattern[i].threadID = 0;
		sansPattern[i].isActive = 0;
		for (j = 0; j < _PATTERN_LAYER_LEN_; j++)
			sansPattern[i].renderInfoLen[j] = 0;
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
	flushIstream();
	while (scriptIdx < introScriptLen)
	{
		speechFlag = writeSpeechBubble(scripts[scriptIdx], _BLACK_, 1);
    	waitForFrame();
		if (speechFlag < 0)
		{
			flushIstream();
			scriptIdx++;
		}
	}
//	SpeechBubble[_ENEMY_SANS_].isActive = 0;
	
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
			
			flushIstream();
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
			playSFX(_SFX_MOMENT_);
			blackScreenEffect(1.0f);
			setSansFace(_SANS_FACE_NORMAL_B_);
	    	setBattlePhase(_ENEMY_PHASE_);
	    	playSFX(_SFX_MOMENT_);
	    	
	    	// run script
	    	sleep(1.0f);
	    	flushIstream();
			while (scriptIdx < introScriptIdx_A)
			{
				speechFlag = writeSpeechBubble(scripts[scriptIdx], _RED_, 0);
	        	movePlayerPos();
    			waitForFrame();
				if (speechFlag < 0)
				{
					flushIstream();
					scriptIdx++;
				}
			}
//			SpeechBubble[_ENEMY_SANS_].isActive = 0;
	    	
        	// run boss pattern
			setSansFace(_SANS_FACE_NORMAL_A_);
			runSansPatternInRange(0, 1);
			// wait until all pattern completed
			flushIstream();
	        while (isAnyPatternAlive())
	        {
	        	movePlayerPos();
	        	waitForFrame();
	        }
	        releasePatternInRange(0, 1);
	        flushIstream();
			while (scriptIdx < introScriptIdx_B)
			{
				speechFlag = writeSpeechBubble(scripts[scriptIdx], _BLACK_, 0);
	        	movePlayerPos();
    			waitForFrame();
				if (speechFlag < 0)
				{
					flushIstream();
					scriptIdx++;
				}
			}
//			SpeechBubble[_ENEMY_SANS_].isActive = 0;
			sleep(0.1f);
	        break;
	    
	    case 1:
			setSansFace(_SANS_FACE_NORMAL_A_);
			flushIstream();
	        while (1)
	        {
	        	movePlayerPos();
	        	waitForFrame();
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
		{
			SpeechBubble[_ENEMY_SANS_].isActive = 0;
			return -1;
		}
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
			currLen = slen;
			readOver = clock() + 1500;
		}
		else
		{
			if (slen <= currLen)
	    	{
				readOver = clock() + 1500;
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
			readOver = clock() + 1500;
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
	COORD pos, begin, end, exit, explode, expl_target;
	ConsoleColor tColor;
	float t;
	
	switch (blasterAngle)
	{
		// vertical
		case _BLAST_TOP_CENTER_:
			end.X 		= EnemyPhaseBox.x + (EnemyPhaseBox.width / 2) - 5;
			end.Y 		= EnemyPhaseBox.y - 9;
			begin.X 	= end.X - 18;
			begin.Y 	= end.Y;
			exit.X 		= end.X + 18;
			exit.Y 		= end.Y;
			expl_target.X = 0;
			expl_target.Y = end.Y;
			break;
		case _BLAST_BOT_CENTER_:
			end.X = EnemyPhaseBox.x + (EnemyPhaseBox.width / 2) - 5;
			end.Y = EnemyPhaseBox.y + EnemyPhaseBox.height - 2;
			begin.X 	= end.X + 18;
			begin.Y 	= end.Y;
			break;
			
		// horizontal
		case _BLAST_MID_RIGHT_:
			end.X 		= EnemyPhaseBox.x + EnemyPhaseBox.width + 10;
			end.Y 		= EnemyPhaseBox.y + (EnemyPhaseBox.height / 2) - 3;
			begin.X 	= end.X;
			begin.Y 	= end.Y - 5;
			exit.X 		= end.X;
			exit.Y 		= end.Y + 5;
			explode.X 	= end.X + 1;
			explode.Y 	= end.Y + 2;
			expl_target.X = 0;
			expl_target.Y = explode.Y;
			break;
		case _BLAST_MID_LEFT_:
			end.X = EnemyPhaseBox.x - 23;
			end.Y = EnemyPhaseBox.y + (EnemyPhaseBox.height / 2) - 3;
			begin.X 	= end.X;
			begin.Y 	= end.Y + 5;
			exit.X 		= end.X;
			exit.Y 		= end.Y - 5;
			explode.X 	= end.X + 13;
			explode.Y 	= end.Y + 2;
			expl_target.X = ScreenWidth;
			expl_target.Y = explode.Y;
			break;
			
		// diagonal
		case _BLAST_TOP_RIGHT_:
			end.X = EnemyPhaseBox.x + EnemyPhaseBox.width + 5;
			end.Y = EnemyPhaseBox.y - 9;
			break;
		case _BLAST_BOT_RIGHT_:
			end.X = EnemyPhaseBox.x + EnemyPhaseBox.width + 5;
			end.Y = EnemyPhaseBox.y + EnemyPhaseBox.height + 2;
			break;
		case _BLAST_BOT_LEFT_:
			end.X = EnemyPhaseBox.x - 20;
			end.Y = EnemyPhaseBox.y + (EnemyPhaseBox.height / 2) - 5;
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
		&(sansPattern[pId].renderInfo[0][0]), 
		begin.X,
		begin.Y,
		AssetFile[blasterType],
		_GRAY_,
		_BLACK_
	);
	sansPattern[pId].renderInfoLen[0] = 1;
	playSFXOnThread(_SFX_GASTERBLASTER_);
	// Main Loop
	t = 0;
	oldTime = clock();
	// appearance loop
	while (t < 1)
	{
		t = (clock() - oldTime) / 400.0f;
		blastId = (int)(t / 0.15f);
		if (4 < blastId)
			blastId = 4;
		// set color
		if (t < 0.4f)
			tColor = _GRAY_;
		else if (t < 0.7f)
			tColor = _BRIGHT_GRAY_;
		else
			tColor = _WHITE_;
			
		// render GasterBlaster
		pos.X = (int)lerp(begin.X, end.X, t);
		pos.Y = (int)lerp(begin.Y, end.Y, t);
		setRenderInfo(
			&(sansPattern[pId].renderInfo[0][0]), 
			pos.X,
			pos.Y,
			AssetFile[blasterType + blastId],
			tColor,
			_BLACK_
		);
		sansPattern[pId].renderInfoLen[0] = 1;
		waitForFrame();
	}
	t = 0;
	oldTime = clock();
	// exploding loop
	while (t < 1)
	{
		t = (clock() - oldTime) / 1500.0f;
		// render GasterBlaster
		setRenderInfo(
			&(sansPattern[pId].renderInfo[0][0]), 
			end.X,
			end.Y,
			AssetFile[blasterType + 4],
			_WHITE_,
			_BLACK_
		);
		sansPattern[pId].renderInfoLen[0] = 1;
		// explode
		if (t < 0.4f)
		{
			// render exploding blaster (charging ver)
			setRenderInfoAttr(
				&(sansPattern[pId].renderInfo[1][0]), 
				explode.X,
				explode.Y,
				4,
				2,
				_BRIGHT_GRAY_,
				_WHITE_
			);
			sansPattern[pId].renderInfoLen[1] = 1;
		}
		else 
		{
			// render exploding blaster
			pos.X = lerp(explode.X, expl_target.X, (t - 0.3f) * 3.5f);
			pos.Y = lerp(explode.Y, expl_target.Y, (t - 0.3f) * 3.5f);
			sansPattern[pId].renderInfoLen[1] = explodeBlaster(blasterAngle, pId, explode, pos, _WHITE_);
		}
		waitForFrame();
	}
	// release rendering exlode
	sleep(1.0f);
	sansPattern[pId].renderInfoLen[1] = 0;
	//
	t = 0;
	oldTime = clock();
	// explode complete, exit loop
	while (t < 1)
	{
		t = (clock() - oldTime) / 400.0f;
		blastId = 4 - (int)(t / 0.15f);
		if (blastId < 0)
			blastId = 0;
		// set color
		if (t < 0.4f)
			tColor = _WHITE_;
		else if (t < 0.7f)
			tColor = _BRIGHT_GRAY_;
		else
			tColor = _GRAY_;
			
		// render GasterBlaster
		pos.X = (int)lerp(end.X, exit.X, t);
		pos.Y = (int)lerp(end.Y, exit.Y, t);
		setRenderInfo(
			&(sansPattern[pId].renderInfo[0][0]), 
			pos.X,
			pos.Y,
			AssetFile[blasterType + blastId],
			tColor,
			_BLACK_
		);
		sansPattern[pId].renderInfoLen[0] = 1;
		waitForFrame();
	}
	sansPattern[pId].renderInfoLen[0] = 0;
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
	const int blasterWidth = 2;
	int width, height;
	int i = 0;
	
	switch (angle)
	{
		// vertical
		case _BLAST_TOP_CENTER_:
		case _BLAST_BOT_CENTER_:
			width = blasterWidth;
			height = (end.Y - begin.Y) + 1;
			for (i = 0; i < height; i++)
			{
				setRenderInfoAttr(
					&(sansPattern[pId].renderInfo[1][i]), 
					begin.X,
					begin.Y + i,
					width,
					height,
					_WHITE_,
					bColor
				);
			}
			break;
		// horizontal
		case _BLAST_MID_RIGHT_:
		case _BLAST_MID_LEFT_:
			width = (end.X - begin.X) + 1;
			height = blasterWidth;
			for (i = 0; i < height; i++)
			{
				setRenderInfoAttr(
					&(sansPattern[pId].renderInfo[1][i]), 
					begin.X,
					begin.Y + i,
					width,
					height,
					_WHITE_,
					bColor
				);
			}
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
				setRenderInfoAttr(
					&(sansPattern[pId].renderInfo[1][i]), 
					lerp(begin.X, end.X, i / (float)height),
					begin.Y + i,
					width,
					height,
					_WHITE_,
					bColor
				);
			}
			break; 
	}
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
	if (pid < 0)
		return;
	sansPattern[pid].hThread = startPattern(
			sansPattern[pid].pattern,
			sansPattern[pid].data,
			&(sansPattern[pid].threadID));
			
	GetExitCodeThread(sansPattern[pid].hThread, &(sansPattern[pid].isActive));
}

void runSansPatternInRange(int begin, int end)
{
	if (begin < 0 || end < begin)
		return;
	int pid;
	for (pid = begin; pid <= end; pid++)
	{
		sansPattern[pid].hThread = startPattern(
			sansPattern[pid].pattern,
			sansPattern[pid].data,
			&(sansPattern[pid].threadID));
			
		GetExitCodeThread(sansPattern[pid].hThread, &(sansPattern[pid].isActive));
	}
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
void releasePatternInRange(int begin, int end)
{
	int pid;
	for (pid = begin; pid <= end; pid++)
		CloseHandle(sansPattern[pid].hThread);
}

void releasePatterns()
{
	int i;
	for (i = 0; i < _SANS_PATTERN_LEN_; i++)
		CloseHandle(sansPattern[i].hThread);
}