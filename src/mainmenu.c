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
	ConsoleColor tSelect[2] = { _WHITE_, _WHITE_ };
	tSelect[select] = _YELLOW_;
	
	printString(_ALIGN_CENTER_, 22, "Begin Game", tSelect[0]);
	printString(_ALIGN_CENTER_, 24, "Exit Game", tSelect[1]);
}