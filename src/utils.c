#include "utils.h"

void sleep(float sec)
{
	Sleep(sec * 1000);
}

char* readFile(char* fname, char* dest)
{
	size_t ret;
	FILE* fp;
	
	fp = fopen(fname, "r");
	if (fp != NULL)
	{
		ret = fread(dest, sizeof(char), ScreenWidth * ScreenHeight + 1, fp);
//		if (0 < ret) // fread success 
		fclose(fp);
	}
	return dest;
}