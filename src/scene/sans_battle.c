#include "sans_battle.h"

static int battleSelect, playerHP;

void runSansBattle()
{
	char input;
	battleSelect = 0;
	playerHP = MaxHP;
	
	sleep(1.0f);
	fadeOut(renderSansBattle);
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
		renderCustomScreen(renderSansBattle);
	}
}

void renderSansBattle()
{
	int x, y, i, damaged;
	char hpText[11];
	
	// print Sans
	printSans();
	
	// print battle box
	x = 6;
	y = 16;
	printLines(x, y, AssetFile[_BATTLE_BOX_], _WHITE_);
	
	// print player info
	x = 9;
	y = 24;
	printLine(x, y, "HSU", _WHITE_);
	x += 12;
	printLine(x, y, "LV 1", _WHITE_);
	
	// print HP info
	// set max HP text
	for (int i = 0; i < 10; i++)
		hpText[i] = '@';
	hpText[10] = '\0';
	x = 37;
	printLine(x, y, "HP", _WHITE_);
	x += 3;
	printLine(x, y, hpText, _YELLOW_);
	damaged = (MaxHP - playerHP) / 10;
	// set player HP info
	for (i = 0; i < damaged; i++)
		hpText[i] = '#';
	hpText[damaged] = '\0';
	printLine(x + 10 - damaged, y, hpText, _RED_);
	// print numeric HP info
	x += 15;
	itoa(playerHP, hpText, 10);
	printLine(x, y, hpText, _WHITE_);
	x += strlen(hpText);
	printLine(x, y, " / ", _WHITE_);
	x += 3;
	itoa(MaxHP, hpText, 10);
	printLine(x, y, hpText, _WHITE_);
	
	// print select boxes
	ConsoleColor tSelect[4] = { _YELLOW_, _YELLOW_, _YELLOW_, _YELLOW_ };
	tSelect[battleSelect] = _LIGHT_YELLOW_;
	x = 7;
	y = 25;
	printLines(x, y, AssetFile[_SELECT_BOX_], tSelect[0]);
	printLine(x + 8, y + 2, "FIGHT", tSelect[0]); 
	x += 27;
	printLines(35, y, AssetFile[_SELECT_BOX_], tSelect[1]);
	printLine(x + 11, y + 2, "ACT", tSelect[1]); 
	x += 27;
	printLines(63, y, AssetFile[_SELECT_BOX_], tSelect[2]);
	printLine(x + 11, y + 2, "ITEM", tSelect[2]); 
	x += 27;
	printLines(91, y, AssetFile[_SELECT_BOX_], tSelect[3]);
	printLine(x + 11, y + 2, "MERCY", tSelect[3]); 
	
}

void printSans()
{
	const int MAX_TICK = 12;
	static int tick, oldTime;
	int face_move[12][2] = { { 0, 0 }, { 1, 0 }, { 1, 0 }, { 0, 0 },
							{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
							{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	    body_move[12][2] = { { 1, 0 }, { 1, 0 }, { 1, 0 }, { 1, 0 }, 
							{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, 
							{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	    leg_move[12][2] = { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
							{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
							{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } };
	int x = 50, y = 1, currTime;
	
	// leg
	printLines(x + 1 + leg_move[tick][0], y + 11 + leg_move[tick][1], AssetFile[_SANS_LEG_NORMAL_], _WHITE_);
	// body
	printLines(x + body_move[tick][0], y + 7 + body_move[tick][1], AssetFile[_SANS_BODY_NORMAL_], _WHITE_);	
	// face
	printLines(x + face_move[tick][0], y + face_move[tick][1], AssetFile[_SANS_FACE_NORMAL_A_], _WHITE_);
	
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