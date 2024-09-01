#include "sans_battle.h"

static int battleSelect;

void runSansBattle()
{
	char input;
	battleSelect = 0;
	
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
				
				case 'T':
				case 't':	
//				case _SPACE_:
//				case _CARRIGE_RETURN_:
					// action 
					return;	
			}
		}
		renderCustomScreen(renderSansBattle);
	}
}

void renderSansBattle()
{
	int x, y;
	
	// print Sans
	printSans();
	
	// print battle box
	x = 6;
	y = 16;
	printLines(x, y, AssetFile[_BATTLE_BOX_], _WHITE_);
	
	// print player info
	y = 24;
		// print here
	
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
	const int MAX_TICK = 10;
	static int tick, oldTime;
	int face_move[10][2] = { { 0, 0 }, { 1, 0 }, { 1, 0 }, { 1, 0 }, { 0, 0 }, 
							{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }},
	    body_move[10][2] = { { 0, 0 }, { 1, 0 }, { 1, 0 }, { 0, 0 }, { 0, 0 }, 
							{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }},
	    leg_move[10][2] = { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, 
							{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }};
	int x = 50, y = 1, currTime;
	
	// leg
	printLines(x + 1 + leg_move[tick][0], y + 11 + leg_move[tick][1], AssetFile[_SANS_LEG_NORMAL_], _WHITE_);
	// body
	printLines(x + body_move[tick][0], y + 7 + body_move[tick][1], AssetFile[_SANS_BODY_NORMAL_], _WHITE_);	
	// face
	printLines(x + face_move[tick][0], y + face_move[tick][1], AssetFile[_SANS_FACE_NORMAL_A_], _WHITE_);
	
	if (oldTime)
	{
		currTime = clock();
		if (100 <= currTime - oldTime)
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