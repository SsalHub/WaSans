#ifndef __SOUNDS__
#define __SOUNDS__
#include <stdio.h>
#include <windows.h>
#include <process.h>
#include <Digitalv.h>
#include "../utils.h"
#include "../settings.h"

#define _SOUND_ASSET_LEN_ 	12
#define _BGM_ASSET_LEN_		5
#define _VOICE_ASSET_LEN_	1
#define _SFX_ASSET_LEN_		13

typedef enum BGM_TYPE
{
    _BGM_BIRDNOISE_,
    _BGM_CYMBAL_,
    _BGM_GAMEOVER_,
    _BGM_MEGALOVANIA_,
    _BGM_STARTMENU_,
} BGM_TYPE;
    
typedef enum VOICE_TYPE
{
    _VOICE_SANS_,
} VOICE_TYPE;
    
typedef enum SFX_TYPE
{
    _SFX_APPEAR_,
    _SFX_ATTACK_,
    _SFX_BELL_,
    _SFX_DOORCLOSE_,
    _SFX_GASTERBLASTER_,
    _SFX_HURT_,
    _SFX_IMPACT_,
    _SFX_INTRONOISE_,
    _SFX_MOMENT_,
    _SFX_REFLECT_,
    _SFX_SAVE_,
    _SFX_SEGAPOWER_,
    _SFX_SQUEAK_,
} SFX_TYPE;

typedef enum SOUND_PLAY_TYPE
{
    _SOUND_ONCE_,
    _SOUND_LOOP_,
    _SOUND_BEGIN_,
    _SOUND_PAUSE_,
    _SOUND_RESUME_,
    _SOUND_STOP_,
} SOUND_PLAY_TYPE;

typedef struct SOUND_DATA
{
	DWORD dw;
	unsigned int dwID;
 } SOUND_DATA;


static MCI_OPEN_PARMS mciOpen;
static MCI_PLAY_PARMS mciPlay;
static MCI_STATUS_PARMS mciStatus;
static SOUND_DATA bgmAsset[_BGM_ASSET_LEN_];
static SOUND_DATA voiceAsset[_VOICE_ASSET_LEN_];


void initSoundAsset();
void initBGMAsset();
void initVoiceAsset();

void playBGM(BGM_TYPE bgmType, SOUND_PLAY_TYPE flag);
void playVoice(VOICE_TYPE voiceType);
void playSFX(SFX_TYPE sfxType);

/* Using On Multi-Threads Only */
void playSFXOnThread(SFX_TYPE sfxType); 

/* Terminate Func */
void releaseBGMAsset();
void releaseVoiceAsset();
void releaseSoundAsset();
unsigned __stdcall releaseSoundAuto(void* args);



/*
		SOUND_DATA Asset Paths
								*/
static const char bgmAssetPath[_BGM_ASSET_LEN_][64]= {
    "data/bgm/birdnoise.wav",
    "data/bgm/cymbal.wav",
    "data/bgm/gameover.wav",
    "data/bgm/megalovania.wav",
    "data/bgm/startmenu.wav",
};
static const char voiceAssetPath[_VOICE_ASSET_LEN_][64]= {
    "data/voice/sans.wav",
};
static const char sfxAssetPath[_SFX_ASSET_LEN_][64]= {
    "data/sfx/appear.wav",
    "data/sfx/attack.wav",
    "data/sfx/bell.wav",
    "data/sfx/doorclose.wav",
    "data/sfx/gasterblaster.wav",
    "data/sfx/hurt.wav",
    "data/sfx/impact.wav",
    "data/sfx/intronoise.wav",
    "data/sfx/moment.wav",
    "data/sfx/reflect.wav",
    "data/sfx/save.wav",
    "data/sfx/segapower.wav",
    "data/sfx/squeak.wav",
};
#endif