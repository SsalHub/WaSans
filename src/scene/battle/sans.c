#include "sans.h"


/* Main func in sans battle */
void Sans_Start()
{
	BATTLE_OBJECT sans[_ENEMY_INFO_LEN_];
	patternIdx = 0;
    scriptIdx = -1;
    Player.mode = _PLAYER_ALIVE_;
    Player.width = 0;	// sub HP
    Player.height = 0;	// jump(gravity) speed

	// init battle
	initSansPattern();
	initSansObject(&sans);
	initBattle(1, &sans, _SANS_PATTERN_LEN_, sansPattern);
	// init speech bubble
	setCOORD(&(SpeechBubble[_ENEMY_SANS_].pos), 74, 2);
	SpeechBubble[_ENEMY_SANS_].tColor = _BLACK_;
	SpeechBubble[_ENEMY_SANS_].bColor = _WHITE_;
	SpeechBubble[_ENEMY_SANS_].width = 24;
	SpeechBubble[_ENEMY_SANS_].height = 6;
	SpeechBubble[_ENEMY_SANS_].isActive = 0;
	// init event listener
	addEventListener(Sans_onDamaged, NULL, _EVENT_ON_DAMAGED_);
}

void Sans_Update()
{
    introPhase();
    enemyPhase();
	// main phase
	while (1)
	{
	    playerPhase();
	    if (getCurrentScene() != _SCENE_BATTLE_SANS_)
	    	break;
	    enemyPhase();
	}
	// exit battle scene
	Sans_releasePatterns();
	releaseBattleAssets();
	flushEvent(_EVENT_ON_DAMAGED_);
	gotoNextScene(_SCENE_MAINMENU_);
}



/* Init Func */
void initSansObject(BATTLE_OBJECT (*enemy)[_ENEMY_INFO_LEN_])
{
	(*enemy)[_ENEMY_BODY_].isActive = 0;
	(*enemy)[_ENEMY_FACE_].isActive = 0;
	
	// init body
	setCOORD(&((*enemy)[_ENEMY_BODY_].pos), 50, 0);
	(*enemy)[_ENEMY_BODY_].data = AssetFile[_SANS_BODY_IDLE_];
	(*enemy)[_ENEMY_BODY_].tColor = _WHITE_;
	(*enemy)[_ENEMY_BODY_].bColor = _BLACK_;
	// init face
	setCOORD(&((*enemy)[_ENEMY_FACE_].pos), 51, 0);
	(*enemy)[_ENEMY_FACE_].data = AssetFile[_SANS_FACE_IDLE_A_];
	(*enemy)[_ENEMY_FACE_].tColor = _WHITE_;
	(*enemy)[_ENEMY_FACE_].bColor = _BLACK_;
	
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
		sansPattern[i].pattern = sansPatternInfo[i].pattern;
		sansPattern[i].collider = Sans_onCollision;
		sansPattern[i].data = &(sansPatternInfo[i]);
		for (j = 0; j < _PATTERN_LAYER_LEN_; j++)
			sansPattern[i].renderInfoLen[j] = 0;
	}
}



/* Each Phase Func */
static void introPhase()
{
    int speechFlag = 0;
	scriptIdx = 0;
    
	sleep(1.0f);
	// run intro phase
    playBGM(_BGM_BIRDNOISE_, _SOUND_BEGIN_);
    fadeIn(_SCENE_BATTLE_SANS_);
    
	setSansFace(_SANS_FACE_IDLE_A_);
	sleep(1.0f);
	scriptIdx = Sans_runScript(0, 2, _BLACK_, 1);
	
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
	int i, speechFlag = 0;
	setCOORD(&(Player.pos), 59, 19);
    
    switch (getBattleTurn())
    {
	    case 0: // intro turn
			playSFX(_SFX_MOMENT_);
			blackScreenEffect(1.0f);
			setSansFace(_SANS_FACE_IDLE_B_);
	    	setBattlePhase(_ENEMY_PHASE_);
	    	playSFX(_SFX_MOMENT_);
	    	
	    	// run script
	    	sleep(1.0f);
			scriptIdx = Sans_runScript(scriptIdx, 3, _RED_, 0);
        	// run boss pattern
			Sans_runPattern(0);
			Sans_runPattern(1);
			sleep(0.2f);
			Sans_runPattern(2);
			Sans_runPatternInRange(3, 6);
			Sans_runPatternInRange(7, 10);
			sleep(0.3f);
	        
			setSansFace(_ENEMY_ASSET_DEFAULT_);
			setSansBody(_ENEMY_ASSET_DEFAULT_);
			scriptIdx = Sans_runScript(scriptIdx, 4, _BLACK_, 1);
			sleep(0.1f);
	        break;
	    
	    case 1:
	    	setGravityMode(_INPUT_NONE_);
			setSansFace(_SANS_FACE_IDLE_A_);
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
void Sans_runPattern(int pid)
{
	if (pid < 0)
		return;
	sansPattern[pid].hThread = startPattern(
			sansPattern[pid].pattern,
			sansPattern[pid].data,
			&(sansPattern[pid].threadID));
			
	GetExitCodeThread(sansPattern[pid].hThread, &(sansPattern[pid].isActive));
	flushIstream();
	while (isAnyPatternAlive())
    {
    	movePlayerPos();
    	waitForFrame();
    }
	Sans_releasePattern(pid);
}

void Sans_runPatternInRange(int begin, int end)
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
	while (isAnyPatternAlive())
    {
    	movePlayerPos();
    	waitForFrame();
    }
	Sans_releasePatternInRange(begin, end);
}

int Sans_runScript(int begin, int end, CONSOLE_COLOR tColor, unsigned int bVoice)
{
	int idx = begin;
	flushIstream();
	while (idx <= end)
	{
		idx = writeSpeechBubble(idx, tColor, bVoice);
    	movePlayerPos();
		waitForFrame();
	}
	return idx;
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

int explodeBlaster(BLASTER_POSITION angle, int pId, COORD begin, COORD end, CONSOLE_COLOR bColor)	
{
	const int blasterWidth = 2;
	COORD pos;
	int width, height;
	int i = 0;
	
	switch (angle)
	{
		// vertical
		case _BLAST_TOP_LEFT_:
		case _BLAST_TOP_CENTER_:
		case _BLAST_TOP_RIGHT_:
		case _BLAST_BOT_LEFT_:
		case _BLAST_BOT_CENTER_:
		case _BLAST_BOT_RIGHT_:
			width = blasterWidth;
			height = abs((end.Y - begin.Y) + 1);
			cpyCOORD(&pos, (begin.Y < end.Y ? &begin : &end));
			for (i = 0; i < height; i++)
			{
				setRenderInfoAttr(
					&(sansPattern[pId].renderInfo[1][i]), 
					pos,
					width * 2,
					height,
					_WHITE_,
					bColor,
					1
				);
				pos.Y++;
			}
			break;
		// horizontal
		case _BLAST_RIGHT_TOP_:
		case _BLAST_RIGHT_CENTER_:
		case _BLAST_RIGHT_BOT_:
		case _BLAST_LEFT_TOP_:
		case _BLAST_LEFT_CENTER_:
		case _BLAST_LEFT_BOT_:
			width = abs((end.X - begin.X) + 1);
			height = blasterWidth;
			cpyCOORD(&pos, (begin.X < end.X ? &begin : &end));
			for (i = 0; i < height; i++)
			{
				setRenderInfoAttr(
					&(sansPattern[pId].renderInfo[1][i]), 
					pos,
					width,
					height,
					_WHITE_,
					bColor,
					1
				);
				pos.Y++;
			}
			break;
		// diagonal
		case _BLAST_EDGE_TOP_LEFT_:
		case _BLAST_EDGE_TOP_RIGHT_:
		case _BLAST_EDGE_BOT_LEFT_:
		case _BLAST_EDGE_BOT_RIGHT_:
			width = blasterWidth;
			height = abs((end.Y - begin.Y) + 1);
			cpyCOORD(&pos, (begin.Y < end.Y ? &begin : &end));
			for (i = 0; i < height; i++)
			{
				pos.X = lerp(begin.X, end.X, i / (float)height);
				setRenderInfoAttr(
					&(sansPattern[pId].renderInfo[1][i]), 
					pos,
					width * 2,
					height,
					_WHITE_,
					bColor,
					1
				);
				pos.Y++;
			}
			break; 
	}
	return i;
}

ASSET_TYPE getBlasterType(BLASTER_POSITION blasterPos)
{
	switch (blasterPos)
	{
		case _BLAST_TOP_CENTER_:
			return _SANS_BLASTER_VERT_0A_;
		case _BLAST_RIGHT_CENTER_:
			return _SANS_BLASTER_VERT_90A_;
		case _BLAST_BOT_CENTER_:
			return _SANS_BLASTER_VERT_180A_;
		case _BLAST_LEFT_CENTER_:
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

char* fixBlasterAngle(char *dst, size_t dstSize, BLASTER_POSITION blasterPos)
{
	ASSET_TYPE blasterType = getBlasterType(blasterPos);
	dst = AssetFile[blasterType];
	return dst;
}

void Sans_onDamaged(void *args)
{
	static int oldTime = -1000;
	if (50 < clock() - oldTime)
	{
		oldTime = clock();
		Player.HP--;
		if (Player.HP <= 0)
			Player.mode = _PLAYER_DIED_;
	}
	else
	{
		Player.width++;
	}
}

void Sans_onHit(void *args)
{
	
}

void setGravityMode(INPUT_TYPE gravity)
{
	if (gravity == _INPUT_NONE_)
	{
		Player.tColor = _HOTPINK_;
		EnemyPhaseBox.mode = _ENEMYBOX_DEFAULT_;
	}
	else
	{
		Player.tColor = _BLUE_;
		if (gravity == _UP_)
			EnemyPhaseBox.mode = _ENEMYBOX_GRAVITY_UP_;
		else if (gravity == _DOWN_)
			EnemyPhaseBox.mode = _ENEMYBOX_GRAVITY_DOWN_;
		else if (gravity == _LEFT_)
			EnemyPhaseBox.mode = _ENEMYBOX_GRAVITY_LEFT_;
		else if (gravity == _RIGHT_)
			EnemyPhaseBox.mode = _ENEMYBOX_GRAVITY_RIGHT_;
	}
}



/* Pattern Func */
BATTLE_PATTERN explodeBlasterToCenter(void *args)
{
	// receive args
	SANS_PATTERN_ARGS *data = (SANS_PATTERN_ARGS*)args;
	unsigned int pId = data->patternId;
	BLASTER_POSITION blasterPos = data->blasterPos;
	// other vars
	int oldTime, renderInfoIdx = 0, blastId = 0;
	COORD pos, begin, end, exit, explode, expl_target;
	ASSET_TYPE blasterType;
	CONSOLE_COLOR tColor;
	float t;
	
	switch (blasterPos)
	{
		// vertical
		case _BLAST_TOP_CENTER_:
			blasterType = _SANS_BLASTER_VERT_0A_;
			end.X 		= EnemyPhaseBox.pos.X + (EnemyPhaseBox.width * 0.5f) - 4;
			end.Y 		= EnemyPhaseBox.pos.Y - 7;
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
			blasterType = _SANS_BLASTER_VERT_180A_;
			end.X 		= EnemyPhaseBox.pos.X + (EnemyPhaseBox.width * 0.5f) - 4;
			end.Y 		= EnemyPhaseBox.pos.Y + EnemyPhaseBox.height + 1;
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
		case _BLAST_RIGHT_CENTER_:
			blasterType = _SANS_BLASTER_VERT_90A_;
			end.X 		= EnemyPhaseBox.pos.X + EnemyPhaseBox.width + 10;
			end.Y 		= EnemyPhaseBox.pos.Y + (EnemyPhaseBox.height / 2) - 3;
			begin.X 	= end.X;
			begin.Y 	= end.Y - 5;
			exit.X 		= end.X;
			exit.Y 		= end.Y + 5;
			explode.X 	= end.X - 1;
			explode.Y 	= end.Y + 1;
			expl_target.X = 0;
			expl_target.Y = explode.Y;
			break;
		case _BLAST_LEFT_CENTER_:
			blasterType = _SANS_BLASTER_VERT_270A_;
			end.X = EnemyPhaseBox.pos.X - 23;
			end.Y = EnemyPhaseBox.pos.Y + (EnemyPhaseBox.height / 2) - 3;
			begin.X 	= end.X;
			begin.Y 	= end.Y + 5;
			exit.X 		= end.X;
			exit.Y 		= end.Y - 5;
			explode.X 	= end.X + 14;
			explode.Y 	= end.Y + 1;
			expl_target.X = ScreenWidth;
			expl_target.Y = explode.Y;
			break;
			
		// diagonal
		case _BLAST_EDGE_TOP_RIGHT_:
			blasterType = _SANS_BLASTER_DIAG_0A_;
			end.X = EnemyPhaseBox.pos.X + EnemyPhaseBox.width + 5;
			end.Y = EnemyPhaseBox.pos.Y - 9;
			break;
		case _BLAST_EDGE_BOT_RIGHT_:
			blasterType = _SANS_BLASTER_DIAG_90A_;
			end.X = EnemyPhaseBox.pos.X + EnemyPhaseBox.width + 5;
			end.Y = EnemyPhaseBox.pos.Y + EnemyPhaseBox.height + 2;
			break;
		case _BLAST_EDGE_BOT_LEFT_:
			blasterType = _SANS_BLASTER_DIAG_180A_;
			end.X = EnemyPhaseBox.pos.X - 20;
			end.Y = EnemyPhaseBox.pos.Y + (EnemyPhaseBox.height / 2) - 5;
			begin.X 	= end.X;
			begin.Y 	= end.Y + 5;
			break;
		case _BLAST_EDGE_TOP_LEFT_:
			blasterType = _SANS_BLASTER_DIAG_270A_;
			end.X = EnemyPhaseBox.pos.X - 20;
			end.Y = EnemyPhaseBox.pos.Y - 8;
			break;
	}
	oldTime = clock();
	setRenderInfo(
		&(sansPattern[pId].renderInfo[0][0]), 
		begin,
		0,
		0,
		AssetFile[blasterType],
		_GRAY_,
		_BLACK_,
		0
	);
	sansPattern[pId].renderInfoLen[0] = 1;
	playSFXOnThread(_SFX_GASTERBLASTER_);
	// Main Loop
	t = 0;
	oldTime = clock();
	// appearance loop
	while (t < 1)
	{
		t = (clock() - oldTime) / 300.0f;
		blastId = (int)(t / 0.08f);
		blastId = 4 < blastId ? 4 : blastId;
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
			pos,
			0,
			0,
			AssetFile[blasterType + blastId],
			tColor,
			_BLACK_,
			0
		);
		sansPattern[pId].renderInfoLen[0] = 1;
		waitForFrame();
	}
	t = 0;
	oldTime = clock();
	// exploding loop
	while (t < 1)
	{
		t = (clock() - oldTime) / 1000.0f;
		// render GasterBlaster
		setRenderInfo(
			&(sansPattern[pId].renderInfo[0][0]), 
			end,
			0,
			0,
			AssetFile[blasterType + 4],
			_WHITE_,
			_BLACK_,
			0
		);
		sansPattern[pId].renderInfoLen[0] = 1;
		// explode
		if (0.2f < t)
		{
			// render exploding blaster
			pos.X = lerp(explode.X, expl_target.X, (t - 0.4f) * 6.0f);
			pos.Y = lerp(explode.Y, expl_target.Y, (t - 0.4f) * 6.0f);
			sansPattern[pId].renderInfoLen[1] = explodeBlaster(blasterPos, pId, explode, pos, _WHITE_);
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
			pos,
			0,
			0,
			AssetFile[blasterType + blastId],
			tColor,
			_BLACK_,
			0
		);
		sansPattern[pId].renderInfoLen[0] = 1;
		waitForFrame();
	}
	sansPattern[pId].renderInfoLen[0] = 0;
	sleep(0.2f);
}

BATTLE_PATTERN explodeBlasterToEdge(void *args)
{
	// receive args
	SANS_PATTERN_ARGS *data = (SANS_PATTERN_ARGS*)args;
	unsigned int pId = data->patternId;
	BLASTER_POSITION blasterPos = data->blasterPos;
	// other vars
	int oldTime, renderInfoIdx = 0, blastId = 0;
	COORD pos, begin, end, exit, explode, expl_target;
	ASSET_TYPE blasterType;
	CONSOLE_COLOR tColor;
	float t;
	
	switch (blasterPos)
	{
		// vertical
		case _BLAST_TOP_LEFT_:
			blasterType = _SANS_BLASTER_VERT_0A_;
			end.X 		= EnemyPhaseBox.pos.X;
			end.Y 		= EnemyPhaseBox.pos.Y - 7;
			begin.X 	= end.X - 14;
			begin.Y 	= end.Y;
			exit.X 		= end.X + 14;
			exit.Y 		= end.Y;
			explode.X 	= end.X + 4;
			explode.Y 	= end.Y + 6;
			expl_target.X = explode.X;
			expl_target.Y = ScreenHeight;
			break;
		case _BLAST_TOP_RIGHT_:
			blasterType = _SANS_BLASTER_VERT_0A_;
			end.X 		= EnemyPhaseBox.pos.X + EnemyPhaseBox.width - 8;
			end.Y 		= EnemyPhaseBox.pos.Y - 7;
			begin.X 	= end.X - 14;
			begin.Y 	= end.Y;
			exit.X 		= end.X + 14;
			exit.Y 		= end.Y;
			explode.X 	= end.X + 4;
			explode.Y 	= end.Y + 6;
			expl_target.X = explode.X;
			expl_target.Y = ScreenHeight;
			break;
		case _BLAST_BOT_LEFT_:
			blasterType = _SANS_BLASTER_VERT_180A_;
			end.X 		= EnemyPhaseBox.pos.X;
			end.Y 		= EnemyPhaseBox.pos.Y + EnemyPhaseBox.height + 1;
			begin.X 	= end.X + 14;
			begin.Y 	= end.Y;
			exit.X 		= end.X - 14;
			exit.Y 		= end.Y;
			explode.X 	= end.X + 4;
			explode.Y 	= end.Y - 1;
			expl_target.X = explode.X;
			expl_target.Y = 0;
			break;
		case _BLAST_BOT_RIGHT_:
			blasterType = _SANS_BLASTER_VERT_180A_;
			end.X 		= EnemyPhaseBox.pos.X + EnemyPhaseBox.width - 8;
			end.Y 		= EnemyPhaseBox.pos.Y + EnemyPhaseBox.height + 1;
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
		case _BLAST_RIGHT_TOP_:
			blasterType = _SANS_BLASTER_VERT_90A_;
			end.X 		= EnemyPhaseBox.pos.X + EnemyPhaseBox.width + 10;
			end.Y 		= EnemyPhaseBox.pos.Y - 1;
			begin.X 	= end.X;
			begin.Y 	= end.Y - 5;
			exit.X 		= end.X;
			exit.Y 		= end.Y + 5;
			explode.X 	= end.X - 1;
			explode.Y 	= end.Y + 1;
			expl_target.X = 0;
			expl_target.Y = explode.Y;
			break;
		case _BLAST_RIGHT_BOT_:
			blasterType = _SANS_BLASTER_VERT_90A_;
			end.X 		= EnemyPhaseBox.pos.X + EnemyPhaseBox.width + 10;
			end.Y 		= EnemyPhaseBox.pos.Y + EnemyPhaseBox.height - 3;
			begin.X 	= end.X;
			begin.Y 	= end.Y - 5;
			exit.X 		= end.X;
			exit.Y 		= end.Y + 5;
			explode.X 	= end.X - 1;
			explode.Y 	= end.Y + 1;
			expl_target.X = 0;
			expl_target.Y = explode.Y;
			break;
		case _BLAST_LEFT_TOP_:
			blasterType = _SANS_BLASTER_VERT_270A_;
			end.X = EnemyPhaseBox.pos.X - 23;
			end.Y = EnemyPhaseBox.pos.Y - 1;
			begin.X 	= end.X;
			begin.Y 	= end.Y + 5;
			exit.X 		= end.X;
			exit.Y 		= end.Y - 5;
			explode.X 	= end.X + 14;
			explode.Y 	= end.Y + 1;
			expl_target.X = ScreenWidth;
			expl_target.Y = explode.Y;
			break;
		case _BLAST_LEFT_BOT_:
			blasterType = _SANS_BLASTER_VERT_270A_;
			end.X = EnemyPhaseBox.pos.X - 23;
			end.Y = EnemyPhaseBox.pos.Y + EnemyPhaseBox.height - 3;
			begin.X 	= end.X;
			begin.Y 	= end.Y + 5;
			exit.X 		= end.X;
			exit.Y 		= end.Y - 5;
			explode.X 	= end.X + 14;
			explode.Y 	= end.Y + 1;
			expl_target.X = ScreenWidth;
			expl_target.Y = explode.Y;
			break;
	}
	oldTime = clock();
	setRenderInfo(
		&(sansPattern[pId].renderInfo[0][0]), 
		begin,
		0,
		0,
		AssetFile[blasterType],
		_GRAY_,
		_BLACK_,
		0
	);
	sansPattern[pId].renderInfoLen[0] = 1;
	playSFXOnThread(_SFX_GASTERBLASTER_);
	// Main Loop
	t = 0;
	oldTime = clock();
	// appearance loop
	while (t < 1)
	{
		t = (clock() - oldTime) / 300.0f;
		blastId = (int)(t / 0.08f);
		blastId = 4 < blastId ? 4 : blastId;
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
			pos,
			0,
			0,
			AssetFile[blasterType + blastId],
			tColor,
			_BLACK_,
			0
		);
		sansPattern[pId].renderInfoLen[0] = 1;
		waitForFrame();
	}
	t = 0;
	oldTime = clock();
	// exploding loop
	while (t < 1)
	{
		t = (clock() - oldTime) / 1000.0f;
		// render GasterBlaster
		setRenderInfo(
			&(sansPattern[pId].renderInfo[0][0]), 
			end,
			0,
			0,
			AssetFile[blasterType + 4],
			_WHITE_,
			_BLACK_,
			0
		);
		sansPattern[pId].renderInfoLen[0] = 1;
		// explode
		if (0.2f < t)
		{
			// render exploding blaster
			pos.X = lerp(explode.X, expl_target.X, (t - 0.4f) * 6.0f);
			pos.Y = lerp(explode.Y, expl_target.Y, (t - 0.4f) * 6.0f);
			sansPattern[pId].renderInfoLen[1] = explodeBlaster(blasterPos, pId, explode, pos, _WHITE_);
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
			pos,
			0,
			0,
			AssetFile[blasterType + blastId],
			tColor,
			_BLACK_,
			0
		);
		sansPattern[pId].renderInfoLen[0] = 1;
		waitForFrame();
	}
	sansPattern[pId].renderInfoLen[0] = 0;
	sleep(0.2f);
}

BATTLE_PATTERN explodeBlasterToPlayer(void *args)
{
	// receive args
	SANS_PATTERN_ARGS *data = (SANS_PATTERN_ARGS*)args;
	unsigned int pId = data->patternId;
	BLASTER_POSITION blasterPos = data->blasterPos;
	// other vars
//	int playerX = Player.x, playerY = Player.y;
}

BATTLE_PATTERN swapGravity(void *args)
{
	// receive args
	SANS_PATTERN_ARGS *data = (SANS_PATTERN_ARGS*)args;
	unsigned int pId = data->patternId;
	INPUT_TYPE gravityDir = data->direction;
	ASSET_TYPE face, body;
	ENEMYBOX_STATUS gravityMode;
	
	// init asset, gravity info
	if (gravityDir == _INPUT_NONE_)
	{
		switch (EnemyPhaseBox.mode)
		{
			case _ENEMYBOX_GRAVITY_DOWN_:
				body = _SANS_BODY_HANDS_UP_A_;
				break;
			case _ENEMYBOX_GRAVITY_UP_:
				body = _SANS_BODY_HANDS_DOWN_A_;
				break;
		}
	}
	else if (gravityDir == _DOWN_)
	{
		body = _SANS_BODY_HANDS_DOWN_A_;
	}
	setSansBody(body);
	
	sleep(0.2f);
	setSansBody(body + 1);
	sleep(0.02f);
	setSansBody(body + 2);
	sleep(0.02f);
	setSansBody(body + 3);
	sleep(0.02f);
	setSansBody(body + 4);
	setGravityMode(gravityDir);
	if (gravityDir != _INPUT_NONE_)
		playSFX(_SFX_IMPACT_);
	sleep(0.5f);
}

BATTLE_PATTERN riseFloorBone(void *args)
{
	// receive args
	SANS_PATTERN_ARGS *data = (SANS_PATTERN_ARGS*)args;
	unsigned int pId = data->patternId;
	ENEMYBOX_STATUS gravityDir = EnemyPhaseBox.mode;
	const int height = 2, warningLayer = 1, boneLayer = 0;
	COORD pos;
	CONSOLE_COLOR warningColor;
	char boneHead[ScreenWidth + 1], boneBody[ScreenWidth + 1];
	int oldTime;
	int i, j, t;
	
	if (gravityDir == _ENEMYBOX_GRAVITY_DOWN_)
	{
		setCOORD(&pos, EnemyPhaseBox.pos.X + 2, EnemyPhaseBox.pos.Y + EnemyPhaseBox.height - 1 - height);
		
		// init bone string
		for (i = 0; i < EnemyPhaseBox.width; i++)
		{
			boneHead[i] = 'Y';
			boneBody[i] = '|';
		}
		boneHead[i] = '\n';
		boneBody[i] = '\n';
		
		// show warning area
		playSFX(_SFX_BELL_);
		t = 0;
		oldTime = clock();
		while (t < 1)
		{
			t = (clock() - oldTime) / 500.0f;
			
			if (t < 0.2f || 0.8f < t)
				warningColor = _HOTPINK_;
			else
				warningColor = _RED_;
				
			setRenderInfoAttr(
				&(sansPattern[pId].renderInfo[warningLayer][0]), 
				pos,
				EnemyPhaseBox.width,
				height,
				warningColor,
				warningColor,
				0
			);
			sansPattern[pId].renderInfoLen[warningLayer] = 1;
			waitForFrame();
		}
		sansPattern[pId].renderInfoLen[warningLayer] = 0;
		sleep(0.05f);
		
		// show bone attack
		for (i = 0; i < height; i++)
		{
			// render bone head
			pos.Y = EnemyPhaseBox.pos.Y + EnemyPhaseBox.height - 2 - i;
			setRenderInfo(
				&(sansPattern[pId].renderInfo[boneLayer][0]), 
				pos,
				EnemyPhaseBox.width,
				1,
				boneHead,
				_WHITE_,
				_BLACK_,
				1
			);
			// render bone body
			for (j = 0; j < i; j++)
			{
				pos.Y = EnemyPhaseBox.pos.Y + EnemyPhaseBox.height - 2 - j;
				setRenderInfo(
					&(sansPattern[pId].renderInfo[boneLayer][1 + j]), 
					pos,
					EnemyPhaseBox.width,
					1,
					boneBody,
					_WHITE_,
					_BLACK_,
					1
				);
			}
			sansPattern[pId].renderInfoLen[boneLayer] = 1 + j;
			sleep(0.05f);
		}
		sleep(0.05f);
		for (; 0 <= i; i--)
		{
			// render bone head
			pos.Y = EnemyPhaseBox.pos.Y + EnemyPhaseBox.height - 1 - (i + 1);
			setRenderInfo(
				&(sansPattern[pId].renderInfo[boneLayer][0]), 
				pos,
				EnemyPhaseBox.width,
				1,
				boneHead,
				_WHITE_,
				_BLACK_,
				1
			);
			// render bone body
			for (j = 0; j < i; j++)
			{
				pos.Y = EnemyPhaseBox.pos.Y + EnemyPhaseBox.height - 1 - j;
				setRenderInfo(
					&(sansPattern[pId].renderInfo[boneLayer][0]), 
					pos,
					EnemyPhaseBox.width,
					1,
					boneBody,
					_WHITE_,
					_BLACK_,
					1
				);
			}
			sansPattern[pId].renderInfoLen[boneLayer] = 1 + j;
			sleep(0.05f);
		}
		sansPattern[pId].renderInfoLen[boneLayer] = 0;
		sleep(0.5f);
	}
}




/* Main onCollision Func */
void Sans_onCollision(void *args)
{
	// collision cooltime
	static int oldTime = 0;
	int currTime = clock();
	if (currTime - oldTime < 20)
		return;
	oldTime = currTime;
	// Main Func
	COLLISION_INFO colInfo = *(COLLISION_INFO*)args;
	if (colInfo.pattern != NULL) 	// Should Fix Later
	{
		Sans_DefaultCollision();
	}
	else
	{
		Sans_MoveDetectionCollision();
	}
}



/* Sub onCollision Func */
void Sans_DefaultCollision()
{
	onEvent(_EVENT_ON_DAMAGED_);
}

void Sans_MoveDetectionCollision()
{
	if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(0x41) 
			|| GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(0x44))
		return;
	onEvent(_EVENT_ON_DAMAGED_);
}



/* etc */
int writeSpeechBubble(unsigned int idx, CONSOLE_COLOR tColor, int bVoice)
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

void setSansFace(ASSET_TYPE type)
{
	if (type == _ENEMY_ASSET_DEFAULT_)
		EnemyInfo[0][_ENEMY_FACE_].data = AssetFile[_SANS_FACE_IDLE_A_];
	else
		EnemyInfo[0][_ENEMY_FACE_].data = AssetFile[type];
}

void setSansBody(ASSET_TYPE type)
{
	if (type == _ENEMY_ASSET_DEFAULT_)
		EnemyInfo[0][_ENEMY_BODY_].data = AssetFile[_SANS_BODY_IDLE_];
	else
		EnemyInfo[0][_ENEMY_BODY_].data = AssetFile[type];
}

void movePlayerPos()
{
	static int oldTime = 0;
	if (EnemyPhaseBox.mode == _ENEMYBOX_DEFAULT_)
	{
		if (40 < clock() - oldTime)
		{
			// move player pos
			if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(0x41))
				Player.pos.X -= 1;
			else if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(0x44))
				Player.pos.X += 1;
			if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(0x57))
				Player.pos.Y -= 1;
			else if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(0x53))
				Player.pos.Y += 1;
		    // fix player pos
			fixPlayerPos(_ENEMYBOX_DEFAULT_);
			// set timer
			oldTime = clock();
		}
	}
	else
	{
		// if gravity on
		if (EnemyPhaseBox.mode == _ENEMYBOX_GRAVITY_UP_)
		{
			
		}
		else if (EnemyPhaseBox.mode == _ENEMYBOX_GRAVITY_DOWN_)
		{
			if (40 < clock() - oldTime)
			{
				if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(0x41))
					Player.pos.X -= 1;
				else if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(0x44))
					Player.pos.X += 1;
				if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(0x57))
				{
					// if player is on platform
					if (Player.pos.Y == EnemyPhaseBox.pos.Y + EnemyPhaseBox.height - 2)
						Player.height = 3;
				}
			    // fix player pos
				fixPlayerPos(_ENEMYBOX_GRAVITY_DOWN_);
				oldTime = clock();
			}
		}
	}
}

void fixPlayerPos(ENEMYBOX_STATUS gravityDir)
{
	switch (gravityDir)
	{
		case _ENEMYBOX_DEFAULT_:
			if (Player.pos.X <= EnemyPhaseBox.pos.X + 2)
		        Player.pos.X = EnemyPhaseBox.pos.X + 2;
		    else if (EnemyPhaseBox.pos.X + 1 + EnemyPhaseBox.width <= Player.pos.X)
		        Player.pos.X = EnemyPhaseBox.pos.X + 1 + EnemyPhaseBox.width;
		    if (Player.pos.Y <= EnemyPhaseBox.pos.Y)
		        Player.pos.Y = EnemyPhaseBox.pos.Y + 1;
		    else if (EnemyPhaseBox.pos.Y + EnemyPhaseBox.height - 2 <= Player.pos.Y)
		        Player.pos.Y = EnemyPhaseBox.pos.Y + EnemyPhaseBox.height - 2;
		    break;
		    
		case _ENEMYBOX_GRAVITY_DOWN_:
			if (Player.pos.X <= EnemyPhaseBox.pos.X + 2)
		        Player.pos.X = EnemyPhaseBox.pos.X + 2;
		    else if (EnemyPhaseBox.pos.X + 1 + EnemyPhaseBox.width <= Player.pos.X)
		        Player.pos.X = EnemyPhaseBox.pos.X + 1 + EnemyPhaseBox.width;
			break;
	}
}


/* Terminate Func */
void Sans_releasePattern(int pid)
{
	CloseHandle(sansPattern[pid].hThread);
}

void Sans_releasePatternInRange(int begin, int end)
{
	int pid;
	for (pid = begin; pid <= end; pid++)
		CloseHandle(sansPattern[pid].hThread);
}

void Sans_releasePatterns()
{
	int i;
	for (i = 0; i < _SANS_PATTERN_LEN_; i++)
		CloseHandle(sansPattern[i].hThread);
}