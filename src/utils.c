#include "utils.h"

char* DataFile[3];

void sleep(float sec)
{
	Sleep(sec * 1000);
}

void initDataFile()
{
	DataFileType t;
	const char fname[3][64] =  {
					"../../data/UNDERTALE.logo",
					"../../data/UNDERTALE.logo",
					"../../data/UNDERTALE.logo",
	};
	
	for (t = 0; t < 3; t++)
	{
		DataFile[t] = readDataFile(fname[t]);
	}
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

char* readDataFile(const char* fname)
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
		perror("Error reading file data");
		exit(1);
		return NULL;
	}
	fclose(fp);
	return dest;
}

void releaseDataFile()
{
	int i;
	for (i = 0; i < 3; i++)
	{
		if (&(DataFile[i]) != NULL)
			free(&(DataFile[i]));
	}
}