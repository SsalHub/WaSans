#include "mainmenu.h"

void showMainmenu()
{
	int select = 0;
	char input;
	
	while (1)
	{
		if (kbhit())
		{
			input = getch();
			switch (input)
			{
				case _UP_:
				case _LEFT_:
					if (0 < select)
						select--;
					break;
				
				case _DOWN_:
				case _RIGHT_:
					if (select < 1)
						select++;
					break;
				
				case _CARRIGE_RETURN_:
					// next action 
					break;
			}
		}
		renderCustomScreen(renderMainmenu, select);
	}
}

void renderMainmenu(int select)
{
	// print logo
	printLogo();
	
	// print selections
	ConsoleColor tSelect[2] = { _WHITE_, _WHITE_ };
	tSelect[select] = _YELLOW_;
	
	printLine(_ALIGN_CENTER_, 23, "Begin Game", tSelect[0]);
	printLine(_ALIGN_CENTER_, 25, "Exit Game", tSelect[1]);
}

void printLogo()
{
	const char fname[128] = "../..//data/UNDERTALE.logo";
	char logo[ScreenWidth * ScreenHeight], *result;
	result = readFile(fname, logo);
	printLines(_ALIGN_CENTER_, 6, logo, _WHITE_);	
}