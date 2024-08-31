#include "mainmenu.h"

int showMainmenu()
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
					return select;
			}
		}
		renderCustomScreen(renderMainmenu, select);
	}
	return -1;
}

void renderMainmenu(int select)
{
	// print logo
	printLines(_ALIGN_CENTER_, 6, DataFile[_LOGO_UNDERTALE_], _WHITE_);	
	
	// print selections
	ConsoleColor tSelect[2] = { _WHITE_, _WHITE_ };
	tSelect[select] = _YELLOW_;
	
	printLine(_ALIGN_CENTER_, 23, "Begin Game", tSelect[0]);
	printLine(_ALIGN_CENTER_, 25, "Exit Game", tSelect[1]);
}