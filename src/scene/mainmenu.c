#include "mainmenu.h"

static int menuSelect;

int showMainmenu()
{
	char input;
	menuSelect = 0;
	setCustomRenderer(renderMainmenu);
	
	playBGM(_BGM_STARTMENU_, _SOUND_BEGIN_);
	while (1)
	{
		if (kbhit())
		{
			input = getch();
			switch (input)
			{
				case _UP_:
				case 'W':
				case 'w':	
					if (0 < menuSelect)
						menuSelect--;
					break;		
				
				case _DOWN_:
				case 'S':
				case 's':
					if (menuSelect < 1)
						menuSelect++;
					break;
				
				case _SPACE_:
				case _CARRIAGE_RETURN_:
					playBGM(_BGM_STARTMENU_, _SOUND_PAUSE_);
					if (menuSelect == 0)
					{
						// begin game
						fadeOut(renderMainmenu);
						runSansBattle();
						return 0;
					}
					else if(menuSelect == 1)
					{
						// exit game
						return 1;
					}
					return -1;
			}
		}
		waitForFrame();
	}
	return -1;
}

void renderMainmenu()
{
	int x = 6, y = 5;
	
	// render logo
	printLines(x, y, AssetFile[_LOGO_UNDERTALE_], _WHITE_, _BLACK_);	
	printLine(x + 55, y + 2, "####", _HOTPINK_, _BLACK_);
	printLine(x + 56, y + 3, "=/", _HOTPINK_, _BLACK_);
	// render selections
	ConsoleColor tSelect[2] = { _WHITE_, _WHITE_ };
	tSelect[menuSelect] = _YELLOW_;
	// render buttom
	y = 21;
	printLine(_ALIGN_CENTER_, y, "Begin Game", tSelect[0], _BLACK_);
	printLine(_ALIGN_CENTER_, y + 2, "Exit Game", tSelect[1], _BLACK_);
}