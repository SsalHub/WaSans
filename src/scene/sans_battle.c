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
	(*enemy)[_ENEMY_LEG_].isActive 	= 0;
	(*enemy)[_ENEMY_BODY_].isActive = 0;
	(*enemy)[_ENEMY_FACE_].isActive = 0;
	
	// init leg
	(*enemy)[_ENEMY_LEG_].x = 51;
	(*enemy)[_ENEMY_LEG_].y = 11;
	(*enemy)[_ENEMY_LEG_].data = AssetFile[_SANS_LEG_NORMAL_];
	(*enemy)[_ENEMY_LEG_].tColor = _WHITE_;
	(*enemy)[_ENEMY_LEG_].bColor = _BLACK_;
	// init body
	(*enemy)[_ENEMY_BODY_].x = 50;
	(*enemy)[_ENEMY_BODY_].y = 7;
	(*enemy)[_ENEMY_BODY_].data = AssetFile[_SANS_BODY_NORMAL_];
	(*enemy)[_ENEMY_BODY_].tColor = _WHITE_;
	(*enemy)[_ENEMY_BODY_].bColor = _BLACK_;
	// init face
	(*enemy)[_ENEMY_FACE_].x = 51;
	(*enemy)[_ENEMY_FACE_].y = 0;
	(*enemy)[_ENEMY_FACE_].data = AssetFile[_SANS_FACE_NORMAL_A_];
	(*enemy)[_ENEMY_FACE_].tColor = _WHITE_;
	(*enemy)[_ENEMY_FACE_].bColor = _BLACK_;
	
	(*enemy)[_ENEMY_LEG_].isActive 	= 1;
	(*enemy)[_ENEMY_BODY_].isActive = 1;
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
		sansPattern[i].data = &(gasterBlasterPatternInfo[i]);
		for (j = 0; j < _PATTERN_LAYER_LEN_; j++)
			sansPattern[i].renderInfoLen[j] = 0;
	}
	// init detail info
	sansPattern[0].pattern = (Pattern)fireBlastToCenter;
	sansPattern[1].pattern = (Pattern)fireBlastToCenter;
	sansPattern[2].pattern = (Pattern)fireBlastToCenter;
	sansPattern[3].pattern = (Pattern)fireBlastToCenter;
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
		scriptIdx = writeSpeechBubble(scriptIdx, _BLACK_, 1);
    	waitForFrame();
	}
	
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
    Player.x = 59;
    Player.y = 19;
    
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
	    		// Player.isActive = 1;
			while (scriptIdx < introScriptIdx_A)
			{
				scriptIdx = writeSpeechBubble(scriptIdx, _RED_, 0);
	        	movePlayerPos();
    			waitForFrame();
			}
	    	
        	// run boss pattern
			setSansFace(_SANS_FACE_NORMAL_A_);
			runSansPatternInRange(0, 3);
			// wait until all pattern completed
			flushIstream();
	        while (isAnyPatternAlive())
	        {
	        	movePlayerPos();
	        	waitForFrame();
	        }
	        releasePatternInRange(0, 3);
	        flushIstream();
			while (scriptIdx < introScriptIdx_B)
			{
				scriptIdx = writeSpeechBubble(scriptIdx, _BLACK_, 0);
	        	movePlayerPos();
    			waitForFrame();
			}
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
			end.X 		= EnemyPhaseBox.x + (EnemyPhaseBox.width * 0.5f) - 3;
			end.Y 		= EnemyPhaseBox.y - 7;
			begin.X 	= end.X - 14;
			begin.Y 	= end.Y;
			exit.X 		= end.X + 14;
			exit.Y 		= end.Y;
			explode.X 	= end.X + 4;
			explode.Y 	= end.Y + 6;
			expl_target.X = explode.X;
			expl_target.Y = ScreenHeight;
			break;
		case _BLAST_BOT_CENTER_:
			end.X 		= EnemyPhaseBox.x + (EnemyPhaseBox.width * 0.5f) - 3;
			end.Y 		= EnemyPhaseBox.y + EnemyPhaseBox.height + 1;
			begin.X 	= end.X + 14;
			begin.Y 	= end.Y;
			exit.X 		= end.X - 14;
			exit.Y 		= end.Y;
			explode.X 	= end.X + 4;
			explode.Y 	= end.Y - 1;
			expl_target.X = explode.X;
			expl_target.Y = 0;
			break;
			
		// horizontal
		case _BLAST_MID_RIGHT_:
			end.X 		= EnemyPhaseBox.x + EnemyPhaseBox.width + 10;
			end.Y 		= EnemyPhaseBox.y + (EnemyPhaseBox.height / 2) - 3;
			begin.X 	= end.X;
			begin.Y 	= end.Y - 5;
			exit.X 		= end.X;
			exit.Y 		= end.Y + 5;
			explode.X 	= end.X - 1;
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
			explode.X 	= end.X + 14;
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
		if (0.4f < t)
		{
			// render exploding blaster
			pos.X = lerp(explode.X, expl_target.X, (t - 0.5f) * 4.0f);
			pos.Y = lerp(explode.Y, expl_target.Y, (t - 0.5f) * 4.0f);
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
	sleep(1.0f);
}

unsigned __stdcall fireBlastToPlayer(void* args)
{
	// receive args
	PatternArgs_Blaster *data = (PatternArgs_Blaster*)args;
	int pId = data->patternId;
	BlasterAngle blasterAngle = data->blasterAngle;
	// other vars
	int playerX = Player.x, playerY = Player.y;
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
					width * 2,
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
					width * 2,
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

void movePlayerPos()
{
	static int playerSpeed = 1, oldTime = 0;
	if (playerSpeed)
	{
		// key input
		if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(0x41))
		{
			Player.x -= playerSpeed;
		}
		else if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(0x44))
		{
			Player.x += playerSpeed;
		}
		if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(0x57))
		{
			Player.y -= playerSpeed;
		}
		else if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(0x53))
		{
			Player.y += playerSpeed;
		}
		playerSpeed = 0;
	    // fix player pos
		if (Player.x <= EnemyPhaseBox.x + 2)
	        Player.x = EnemyPhaseBox.x + 2;
	    else if (EnemyPhaseBox.x + 1 + EnemyPhaseBox.width <= Player.x)
	        Player.x = EnemyPhaseBox.x + 1 + EnemyPhaseBox.width;
	    if (Player.y <= EnemyPhaseBox.y)
	        Player.y = EnemyPhaseBox.y + 1;
	    else if (EnemyPhaseBox.y + EnemyPhaseBox.height - 2 <= Player.y)
	        Player.y = EnemyPhaseBox.y + EnemyPhaseBox.height - 2;
	    // set clock (oldTime)
		oldTime = clock();
	}
	else
	{
		int currTime = clock();
		if (50 < currTime - oldTime)
		{
			playerSpeed = 1;
			oldTime = currTime;
		}
	}
}



/* etc */
int writeSpeechBubble(unsigned int idx, ConsoleColor tColor, int bVoice)
{
    static int currLen = 0, oldTime = 0, readOver = 0;
    static const char *pScript = NULL;
    char buffer[128], input;
	int i, j, currTime, slen;

	// init data
    if (pScript != scripts[idx])
    {
        pScript = scripts[idx];
		readOver = 0;
        currLen = 1;
        oldTime = clock() - 1000;
		SpeechBubble[_ENEMY_SANS_].isActive = 1;
		SpeechBubble[_ENEMY_SANS_].tColor = tColor;
    }
    slen = strlen(pScript);
    
    // if already script read fully
	if (readOver)
	{
		if (readOver < clock())
		{
			SpeechBubble[_ENEMY_SANS_].isActive = 0;
			return idx + 1;
		}
		return idx;
	}
	// if not enough time passed
	currTime = clock();
    if (currTime - oldTime < 120)
    	return idx;
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
				return idx;
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
			return idx;
		}
		currLen++;
        oldTime = currTime;
	}
    // write script on 'SpeechBubble' until currLen
    memcpy(buffer, pScript, currLen);
    buffer[currLen] = '\0';
    memcpy(SpeechBubble[_ENEMY_SANS_].data, buffer, currLen + 1);
    if (bVoice)
		playVoice(_VOICE_SANS_);
    return idx;
}

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