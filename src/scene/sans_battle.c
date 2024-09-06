#include "sans_battle.h"

static int battleTurn, battleSelect, playerHP;
static const char scripts[_SANS_SCRIPT_LEN_][64] = {
							"it's a beautiful day outside.",
							"birds are singing. flowers are blooming...",
							"on days like these, kids like you...",
							"Should be burning in hell.",
						};
static int scriptIdx;

void runSansBattle()
{
	char input;
	playerHP = MaxHP;
	battleTurn = 0;
	battleSelect = 0;
	
	// Enter scene
	introPhase();
	
	bossPhase();
	playerPhase();
	battleTurn++;
	
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
				case _CARRIGE_RETURN_:
					if (battleSelect == 0)
					{
						playerHP -= 5;
					}
					else if (battleSelect == 3)
					{
						return;	
					}
					break;
			}
		}
		renderCustom(renderSansBattle);
	}
}

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
	if (battleTurn == 0)
	{
		// action
		
	}
}

void playerPhase()
{
	if (battleTurn == 0)
	{
		// action
		
	}
}

void renderSansBattle()
{
	renderSans();
	
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
	renderSans();
	// render player info
	renderPlayerInfo();
	// render speech bubble
	if (scriptIdx < 0)	// waiting script before speech
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
	flag = renderSpeechBubble(scripts[scriptIdx]);
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
	renderSans();
	renderBossPhaseBox();
	renderPlayerInfo();
	renderSelectBox();
}

void renderPlayerPhase()
{
	renderSans();
	renderPlayerPhaseBox();
	renderPlayerInfo();
	renderSelectBox();
}

void renderSans()
{
	const int MAX_TICK = 12;
	static int tick, oldTime;
	int face_move[12][2] 	= { { 0, 0 }, { 1, 0 }, { 1, 0 }, { 0, 0 },
								{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
								{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	    body_move[12][2] 	= { { 1, 0 }, { 1, 0 }, { 1, 0 }, { 1, 0 }, 
								{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, 
								{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	    leg_move[12][2] 	= { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
								{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
								{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } };
	int x = 50, y = 0, currTime;
	
	// leg
//	printLines(x + 1 + leg_move[tick][0], y + 11 + leg_move[tick][1], AssetFile[_SANS_LEG_NORMAL_], _WHITE_);
	printLines(x + 1, y + 11, AssetFile[_SANS_LEG_NORMAL_], _WHITE_, _BLACK_);
	// body
//	printLines(x + body_move[tick][0], y + 7 + body_move[tick][1], AssetFile[_SANS_BODY_NORMAL_], _WHITE_);	
	printLines(x, y + 7, AssetFile[_SANS_BODY_NORMAL_], _WHITE_, _BLACK_);	
	// face
//	printLines(x + face_move[tick][0], y + face_move[tick][1], AssetFile[_SANS_FACE_NORMAL_A_], _WHITE_);
	printLines(x, y, AssetFile[_SANS_FACE_NORMAL_A_], _WHITE_, _BLACK_);
	
	// set ticks based on current/old time
	if (oldTime)
	{
		currTime = clock();
		if (100 <= currTime - oldTime)	// 100ms == 0.1sec
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

int renderSpeechBubble(const char* script)
{
	int x = 74, y = 2, w = 24, h = 6, i, j, currTime;
	static int currLen, oldTime;
	static const char* pScript;
	char buffer[ScreenWidth * 2], ch[3], *copy, tmp;
	
	if (pScript != script)
	{
		currLen = 0;
		pScript = script;
	}
	
	if (!oldTime)
		oldTime = clock();
	currTime = clock();
	if (80 < currTime - oldTime)
	{
		currLen = strlen(script) < currLen + 1 ? currLen : currLen + 1;
		oldTime = currTime;
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
	x += 1; y += 1;
	copy = (char*)malloc(sizeof(char) * strlen(script) + 1);
	strcpy(copy, script);
	copy[currLen] = '\0';
	i = 0;
	while (strlen(copy) / (w - 2))
	{
		tmp = copy[w - 2];
		copy[w - 2] = '\0';
		printLine(x, y + i, copy, _BLACK_, _WHITE_);
		copy[w - 2] = tmp;
		strcpy(copy, copy + (w - 2));
		i++;
	}
	printLine(x, y + i, copy, _BLACK_, _WHITE_);
	free(copy);
	
	if (strlen(script) <= currLen)
		return -1;
	return currLen;
}

void renderBossPhaseBox()
{
	int x, y, w, h, i, j;
	char buffer[(ScreenWidth + 1) * (ScreenHeight / 2)], ch[3];
	
	w = 18;
	h = 9;
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
	
	x = 49;
	y = 15;
	printLines(x, y, buffer, _WHITE_, _BLACK_);
}

void renderPlayerPhaseBox()
{
	int x, y, w, h, i, j;
	char buffer[(ScreenWidth + 1) * (ScreenHeight / 2)], ch[3];
	
	w = 102;
	h = 9;
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
	
	x = 6;
	y = 15;
	printLines(x, y, buffer, _WHITE_, _BLACK_);
}

void renderPlayerInfo()
{
	int x, y, i, damaged;
	char hpText[11];
	
	// render player info
	x = 12;
	y = 24;
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
	int x, y;
	ConsoleColor tSelect[4] = { _YELLOW_, _YELLOW_, _YELLOW_, _YELLOW_ };
	tSelect[battleSelect] = _LIGHT_YELLOW_;
	x = 7;
	y = 25;
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