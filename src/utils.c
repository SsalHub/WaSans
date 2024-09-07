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