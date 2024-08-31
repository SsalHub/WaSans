#include "sans_battle.h"

int battleSelect;

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
					
				case _SPACE_:
				case _CARRIGE_RETURN_:
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
	// print battle box
	x = 6;
	y = 11;
	printLines(x, 11, DataFile[_ASSET_BATTLE_BOX_], _WHITE_);
	
	// print player info
	y = 23;
		// print here
	
	// print select boxes
	ConsoleColor tSelect[4] = { _YELLOW_, _YELLOW_, _YELLOW_, _YELLOW_ };
	tSelect[battleSelect] = _LIGHT_YELLOW_;
	x = 7;
	y = 24;
	printLines(x, y, DataFile[_ASSET_SELECT_BOX_], tSelect[0]);
	printLine(x + 9, y + 2, "FIGHT", tSelect[0]); 
	x += 28;
	printLines(35, y, DataFile[_ASSET_SELECT_BOX_], tSelect[1]);
	printLine(x + 10, y + 2, "ACT", tSelect[1]); 
	x += 28;
	printLines(63, y, DataFile[_ASSET_SELECT_BOX_], tSelect[2]);
	printLine(x + 10, y + 2, "ITEM", tSelect[2]); 
	x += 28;
	printLines(91, y, DataFile[_ASSET_SELECT_BOX_], tSelect[3]);
	printLine(x + 9, y + 2, "MERCY", tSelect[3]); 
}