#include "utils.h"

char* AssetFile[_ASSETFILE_NUM_];
				
void sleep(float sec)
{
	Sleep(sec * 1000);
}

void initDataAsset()
{
	AssetFileType t;
	for (t = 0; t < _ASSETFILE_NUM_; t++)
		AssetFile[t] = readAssetFile(assetFilePath[t]);
}

char* readFile(const char* fname, char* dest)
{
	size_t fsize, result;
	FILE* fp;
	
	fp = fopen(fname, "r");
	if (fp == NULL)
	{
		perror("Error opening file");
		return NULL;
	}
	fseek(fp, 0, SEEK_END);
	fsize = ftell(fp) / sizeof(char);
	fseek(fp, 0, SEEK_SET);
	result = fread(dest, sizeof(char), fsize, fp);
	dest[result] = '\0';
	if (result <= 0) // fread failed
	{
		perror("Error reading file data");
		return NULL;
	}
	fclose(fp);
	return dest;
}

char* readAssetFile(const char* fname)
{
	FILE* fp;
	char* dest;
	size_t fsize, result;
	
	fp = fopen(fname, "r");
	if (fp == NULL)
	{
		perror("Error opening file");
		return NULL;
	}
	
	fseek(fp, 0, SEEK_END);
	fsize = ftell(fp) / sizeof(char);
	fseek(fp, 0, SEEK_SET);
	dest = (char*)malloc(sizeof(char) * fsize + 1);
	
	result = fread(dest, sizeof(char), fsize, fp);
	dest[result] = '\0';
	if (result <= 0) // fread failed
	{
		perror("Error reading asset file data");
		exit(1);
		return NULL;
	}
	fclose(fp);
	return dest;
}

void releaseAssetFile()
{
	int i;
	for (i = 0; i < _ASSETFILE_NUM_; i++)
	{
		if (AssetFile[i] == NULL)
			continue;
		free(AssetFile[i]);
	}
}

int getRandomRange(int min, int max)
{
	int limit = abs(max - min);
	srand(time(NULL));
	return rand() % limit - min;
}

char* rotateString(char* dst, char* src, int angle)
{
	int w = 0, h = 0, i, j;
	
	if (angle / 90 == 0)
	{
		strcpy(dst, src);
		return dst;
	}
	
	// get source string's width/height
	i = 0;
	while (src[i])
	{
		if (src[i] == '\n')
			h++;
		if (h == 0)
			w++;
		i++;
	}
	h++;
	w++;
	
	switch (angle / 90)
	{
		case 1:
			for (i = 0; i < h; i++)
			{
				for (j = 0; j < w; j++)
				{
					dst[h * j + i] = src[w * i + (w - j - 1)];
				}
				dst[h * i + h] = '\n';
			}
			break;
			
		case 2:
			for (i = 0; i < h; i++)
			{
				for (j = 0; j < w; j++)
				{
					dst[(w * i) + j] = src[w * (h - i - 1) + (w - j - 1)];
				}
				dst[(w * i) + j] = '\n';
			}
			break;
			
		case 3:
			for (i = 0; i < h; i++)
			{
				for (j = 0; j < w; j++)
				{
					dst[h * j + i] = src[w * (h - i - 1) + j];
				}
				dst[h * i + h] = '\n';
			}
			break;
	}
	dst[strlen(src)] = '\0';
	return dst;
}

HANDLE startPattern(unsigned __stdcall (*pattern)(int), void* args, unsigned int* threadID)
{
	return (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)pattern, args, 0, threadID);
}

float lerp(float from, float to, float t)
{
	if (t <= 0)
		return from;
	if (1 <= t)
		return to;
	return (to - from) * t + from;
}

void setRenderInfo(RenderInfo* target, int x, int y, char* s, ConsoleColor tColor, ConsoleColor bColor)
{
	if (s != NULL)
		strcpy(target->s, s);
	target->x = x;
	target->y = y;
	target->tColor = tColor;
	target->bColor = bColor;
}