#include "sans_battle.h"

void runSansBattle()
{
	int select = 0;
	char input;
	
	while (1)
	{
		if (kbhit())
		{
			return;
//			input = getch();
//			switch (input)
//			{
//				case _UP_:
//				case _LEFT_:
//					if (0 < select)
//						select--;
//					break;
//				
//				case _DOWN_:
//				case _RIGHT_:
//					if (select < 1)
//						select++;
//					break;
//				
//				case _CARRIGE_RETURN_:
//					return select;
//			}
		}
		renderScreen();
	}
}