#include "mainmenu.h"

int menuSelect;

int showMainmenu()
{
	menuSelect = 0;
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
				case _CARRIGE_RETURN_:
					if (menuSelect == 0)
					{
						// begin game
						fadeIn(renderMainmenu);
						runSansBattle();
					}
					else if(menuSelect == 1)
					{
						// exit game
						return 1;
					}
					return 0;
			}
		}
		renderCustomScreen(renderMainmenu);
	}
	return -1;
}

void renderMainmenu()
{
	// print logo
	printLines(_ALIGN_CENTER_, 6, DataFile[_LOGO_UNDERTALE_], _WHITE_);	
	
	// print selections
	ConsoleColor tSelect[2] = { _WHITE_, _WHITE_ };
	tSelect[menuSelect] = _LIGHT_YELLOW_;
	
	printLine(_ALIGN_CENTER_, 23, "Begin Game", tSelect[0]);
	printLine(_ALIGN_CENTER_, 25, "Exit Game", tSelect[1]);
}