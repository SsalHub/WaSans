#ifndef __UTILS__
#define __UTILS__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <process.h>
#include <windows.h>
#include "settings.h"

#define _ASSET_LEN_ 54

typedef enum AssetType
{
	/* Global Asset */
    _LOGO_UNDERTALE_ 			= 0,
    _SELECT_BOX_,
    
    /* On Sans Battle */
    _SANS_LEG_NORMAL_,
    _SANS_BODY_NORMAL_,
    _SANS_FACE_NORMAL_A_,
    _SANS_FACE_NORMAL_B_,
    
    _SANS_BLASTER_VERT_0A_,
    _SANS_BLASTER_VERT_0B_,
    _SANS_BLASTER_VERT_0C_,
    _SANS_BLASTER_VERT_0D_,
    _SANS_BLASTER_VERT_0E_,
    _SANS_BLASTER_VERT_0F_,
    _SANS_BLASTER_DIAG_0A_,
    _SANS_BLASTER_DIAG_0B_,
    _SANS_BLASTER_DIAG_0C_,
    _SANS_BLASTER_DIAG_0D_,
    _SANS_BLASTER_DIAG_0E_,
    _SANS_BLASTER_DIAG_0F_,
    
    _SANS_BLASTER_VERT_90A_,
    _SANS_BLASTER_VERT_90B_,
    _SANS_BLASTER_VERT_90C_,
    _SANS_BLASTER_VERT_90D_,
    _SANS_BLASTER_VERT_90E_,
    _SANS_BLASTER_VERT_90F_,
    _SANS_BLASTER_DIAG_90A_,
    _SANS_BLASTER_DIAG_90B_,
    _SANS_BLASTER_DIAG_90C_,
    _SANS_BLASTER_DIAG_90D_,
    _SANS_BLASTER_DIAG_90E_,
    _SANS_BLASTER_DIAG_90F_,
    
    _SANS_BLASTER_VERT_180A_,
    _SANS_BLASTER_VERT_180B_,
    _SANS_BLASTER_VERT_180C_,
    _SANS_BLASTER_VERT_180D_,
    _SANS_BLASTER_VERT_180E_,
    _SANS_BLASTER_VERT_180F_,
    _SANS_BLASTER_DIAG_180A_,
    _SANS_BLASTER_DIAG_180B_,
    _SANS_BLASTER_DIAG_180C_,
    _SANS_BLASTER_DIAG_180D_,
    _SANS_BLASTER_DIAG_180E_,
    _SANS_BLASTER_DIAG_180F_,
    
    _SANS_BLASTER_VERT_270A_,
    _SANS_BLASTER_VERT_270B_,
    _SANS_BLASTER_VERT_270C_,
    _SANS_BLASTER_VERT_270D_,
    _SANS_BLASTER_VERT_270E_,
    _SANS_BLASTER_VERT_270F_,
    _SANS_BLASTER_DIAG_270A_,
    _SANS_BLASTER_DIAG_270B_,
    _SANS_BLASTER_DIAG_270C_,
    _SANS_BLASTER_DIAG_270D_,
    _SANS_BLASTER_DIAG_270E_,
    _SANS_BLASTER_DIAG_270F_,
} AssetType;

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



extern char* AssetFile[_ASSET_LEN_];



static const char assetFilePath[_ASSET_LEN_][128] = {
					/* Global Asset */
					"../../data/UNDERTALE_LOGO.asset",
					"../../data/SELECT_BOX.asset",
					
					/* On Sans Battle Asset */
					"../../data/character/sans/leg_normal.asset",
					"../../data/character/sans/body_normal.asset",
					"../../data/character/sans/face_normal_a.asset",
					"../../data/character/sans/face_normal_b.asset",
					
					"../../data/character/sans/gasterblaster/0/vertical_0_a.asset",
					"../../data/character/sans/gasterblaster/0/vertical_0_b.asset",
					"../../data/character/sans/gasterblaster/0/vertical_0_c.asset",
					"../../data/character/sans/gasterblaster/0/vertical_0_d.asset",
					"../../data/character/sans/gasterblaster/0/vertical_0_e.asset",
					"../../data/character/sans/gasterblaster/0/vertical_0_f.asset",
					"../../data/character/sans/gasterblaster/0/diagonal_0_a.asset",
					"../../data/character/sans/gasterblaster/0/diagonal_0_b.asset",
					"../../data/character/sans/gasterblaster/0/diagonal_0_c.asset",
					"../../data/character/sans/gasterblaster/0/diagonal_0_d.asset",
					"../../data/character/sans/gasterblaster/0/diagonal_0_e.asset",
					"../../data/character/sans/gasterblaster/0/diagonal_0_f.asset",
					
					"../../data/character/sans/gasterblaster/90/vertical_90_a.asset",
					"../../data/character/sans/gasterblaster/90/vertical_90_b.asset",
					"../../data/character/sans/gasterblaster/90/vertical_90_c.asset",
					"../../data/character/sans/gasterblaster/90/vertical_90_d.asset",
					"../../data/character/sans/gasterblaster/90/vertical_90_e.asset",
					"../../data/character/sans/gasterblaster/90/vertical_90_f.asset",
					"../../data/character/sans/gasterblaster/90/diagonal_90_a.asset",
					"../../data/character/sans/gasterblaster/90/diagonal_90_b.asset",
					"../../data/character/sans/gasterblaster/90/diagonal_90_c.asset",
					"../../data/character/sans/gasterblaster/90/diagonal_90_d.asset",
					"../../data/character/sans/gasterblaster/90/diagonal_90_e.asset",
					"../../data/character/sans/gasterblaster/90/diagonal_90_f.asset",
					
					"../../data/character/sans/gasterblaster/180/vertical_180_a.asset",
					"../../data/character/sans/gasterblaster/180/vertical_180_b.asset",
					"../../data/character/sans/gasterblaster/180/vertical_180_c.asset",
					"../../data/character/sans/gasterblaster/180/vertical_180_d.asset",
					"../../data/character/sans/gasterblaster/180/vertical_180_e.asset",
					"../../data/character/sans/gasterblaster/180/vertical_180_f.asset",
					"../../data/character/sans/gasterblaster/180/diagonal_180_a.asset",
					"../../data/character/sans/gasterblaster/180/diagonal_180_b.asset",
					"../../data/character/sans/gasterblaster/180/diagonal_180_c.asset",
					"../../data/character/sans/gasterblaster/180/diagonal_180_d.asset",
					"../../data/character/sans/gasterblaster/180/diagonal_180_e.asset",
					"../../data/character/sans/gasterblaster/180/diagonal_180_f.asset",
					
					"../../data/character/sans/gasterblaster/270/vertical_270_a.asset",
					"../../data/character/sans/gasterblaster/270/vertical_270_b.asset",
					"../../data/character/sans/gasterblaster/270/vertical_270_c.asset",
					"../../data/character/sans/gasterblaster/270/vertical_270_d.asset",
					"../../data/character/sans/gasterblaster/270/vertical_270_e.asset",
					"../../data/character/sans/gasterblaster/270/vertical_270_f.asset",
					"../../data/character/sans/gasterblaster/270/diagonal_270_a.asset",
					"../../data/character/sans/gasterblaster/270/diagonal_270_b.asset",
					"../../data/character/sans/gasterblaster/270/diagonal_270_c.asset",
					"../../data/character/sans/gasterblaster/270/diagonal_270_d.asset",
					"../../data/character/sans/gasterblaster/270/diagonal_270_e.asset",
					"../../data/character/sans/gasterblaster/270/diagonal_270_f.asset",
				};

void sleep(float sec);
void initDataAsset();
char* readFile(const char *fname, char *dest);
char* readAssetFile(const char *fname);
void releaseAssetFile();
int getRandomRange(int min, int max);
//char* rotateString(char* dst, char* src, int angle);
float lerp(float from, float to, float t);
void fillSpaceChar(char* str, int begin, int end);
#endif