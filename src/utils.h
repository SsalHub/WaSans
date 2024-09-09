#ifndef __UTILS__
#define __UTILS__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <process.h>
#include <windows.h>
#include "settings.h"

#define _ASSETFILE_NUM_ 18

typedef enum AssetFileType
{
    _LOGO_UNDERTALE_ 			= 0,
    _SELECT_BOX_,
    _SANS_BODY_NORMAL_,
    _SANS_LEG_NORMAL_,
    _SANS_FACE_NORMAL_A_,
    _SANS_FACE_NORMAL_B_,
    _SANS_BLAST_VERTICAL_A_,
    _SANS_BLAST_VERTICAL_B_,
    _SANS_BLAST_VERTICAL_C_,
    _SANS_BLAST_VERTICAL_D_,
    _SANS_BLAST_VERTICAL_E_,
    _SANS_BLAST_VERTICAL_F_,
    _SANS_BLAST_DIAGONAL_A_,
    _SANS_BLAST_DIAGONAL_B_,
    _SANS_BLAST_DIAGONAL_C_,
    _SANS_BLAST_DIAGONAL_D_,
    _SANS_BLAST_DIAGONAL_E_,
    _SANS_BLAST_DIAGONAL_F_,
} AssetFileType;

typedef enum ConsoleColor
{
    _BLACK_ 					= 0,
    _BLUE_ 						= 1,
    _DARK_GREEN_ 				= 2,
    _SKYBLUE_ 					= 3,
    _RED_ 						= 4,
    _DARK_PURPLE_ 				= 5,
    _YELLOW_ 					= 6,
    _BRIGHT_GRAY_ 				= 7,
    _GRAY_ 						= 8,
    _OCEAN_BLUE_ 				= 9,
    _GREEN_ 					= 10,
    _SKY_ 						= 11,
    _HOTPINK_ 					= 12,
    _PURPLE_ 					= 13,
    _LIGHT_YELLOW_ 				= 14,
    _WHITE_ 					= 15,
} ConsoleColor;

typedef enum InputType
{
    _SPACE_ 					= 32,
    _ESCAPE_ 					= 27,
    _LEFT_ 						= 75,
    _RIGHT_ 					= 77,
    _UP_ 						= 72,
    _DOWN_ 						= 80,
    _UPPER_R_ 					= 82,
    _LOWER_R_ 					= 114,
    _CARRIAGE_RETURN_ 			= 13,
} InputType;

typedef struct RenderInfo
{
	int x;
	int y;
	char* s;
	ConsoleColor tColor;
	ConsoleColor bColor;
} RenderInfo;

typedef struct Pattern
{
	unsigned __stdcall (*pattern)(int);
	int data;
	HANDLE hThread;
	DWORD isActive;
	unsigned int threadID;
	int renderInfoLen;
	RenderInfo renderInfo[10];
} Pattern;

static const char assetFilePath[_ASSETFILE_NUM_][64] = {
					"../../data/UNDERTALE_LOGO.asset",
					"../../data/SELECT_BOX.asset",
					"../../data/character/sans/body_normal.asset",
					"../../data/character/sans/leg_normal.asset",
					"../../data/character/sans/face_normal_a.asset",
					"../../data/character/sans/face_normal_b.asset",
					"../../data/character/sans/blast/blast_vertical_a.asset",
					"../../data/character/sans/blast/blast_vertical_b.asset",
					"../../data/character/sans/blast/blast_vertical_c.asset",
					"../../data/character/sans/blast/blast_vertical_d.asset",
					"../../data/character/sans/blast/blast_vertical_e.asset",
					"../../data/character/sans/blast/blast_vertical_f.asset",
					"../../data/character/sans/blast/blast_diagonal_a.asset",
					"../../data/character/sans/blast/blast_diagonal_b.asset",
					"../../data/character/sans/blast/blast_diagonal_c.asset",
					"../../data/character/sans/blast/blast_diagonal_d.asset",
					"../../data/character/sans/blast/blast_diagonal_e.asset",
					"../../data/character/sans/blast/blast_diagonal_f.asset",
				};

extern char *AssetFile[_ASSETFILE_NUM_];

void sleep(float sec);
void initDataAsset();
char *readFile(const char *fname, char *dest);
char *readAssetFile(const char *fname);
void releaseAssetFile();
int getRandomRange(int min, int max);
char* rotateString(char* dst, char* src, int angle);
HANDLE startPattern(unsigned __stdcall (*pattern)(int), void* args, unsigned int* threadID);
float lerp(float from, float to, float t);
void setRenderInfo(RenderInfo* target, int x, int y, char* s, ConsoleColor tColor, ConsoleColor bColor);
#endif