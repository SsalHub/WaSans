#include "mainmenu.h"


void initMainMenu()
{
	setSceneRenderer(renderMainmenu);
	menuSelect = 0;
	playBGM(_BGM_STARTMENU_, _SOUND_BEGIN_);
}

void runMainmenu()
{
	char input;
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
						fadeOut();
						gotoNextScene(_SCENE_SANS_BATTLE_);
						return;
					}
					else if(menuSelect == 1)
					{
						// exit game
						gotoNextScene(_SCENE_EXIT_GAME_);
						return;
					}
					break;
			}
		}
		sleep(0.01f);
	}
	gotoNextScene(_SCENE_EXIT_GAME_);
}

void renderMainmenu()
{
	ConsoleColor tSelect[2] = { _WHITE_, _WHITE_ };
	// render logo
	printLines(6, 5, AssetFile[_LOGO_UNDERTALE_], _WHITE_, _BLACK_);	
	printLine(61, 7, "####", _HOTPINK_, _BLACK_);
	printLine(62, 8, "=/", _HOTPINK_, _BLACK_);
	// render button
	tSelect[menuSelect] = _YELLOW_;
	printLine(_ALIGN_CENTER_, 21, "Begin Game", tSelect[0], _BLACK_);
	printLine(_ALIGN_CENTER_, 23, "Exit Game", tSelect[1], _BLACK_);
}