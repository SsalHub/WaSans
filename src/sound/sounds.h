#ifndef __SOUNDS__
#define __SOUNDS__
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <Digitalv.h>
#include "../utils.h"

#define _SOUND_ASSET_LEN_ 	12
#define _BGM_ASSET_LEN_		5
#define _VOICE_ASSET_LEN_	1
#define _SFX_ASSET_LEN_		6

typedef enum BGMAssetType
{
    _BGM_BIRDNOISE_,
    _BGM_CYMBAL_,
    _BGM_GAMEOVER_,
    _BGM_MEGALOVANIA_,
    _BGM_STARTMENU_,
} BGMAssetType;
    
typedef enum VoiceAssetType
{
    _VOICE_SANS_,
} VoiceAssetType;
    
typedef enum SFXAssetType
{
    _SFX_ATTACK_,
    _SFX_DOORCLOSE_,
    _SFX_GASTERBLASTER_,
    _SFX_INTRONOISE_,
    _SFX_MOMENT_,
    _SFX_REFLECT_,
} SFXAssetType;

typedef enum SoundPlayType
{
    _SOUND_ONCE_,
    _SOUND_LOOP_,
    _SOUND_BEGIN_,
    _SOUND_PAUSE_,
    _SOUND_RESUME_,
} SoundPlayType;

typedef struct Sound
{
	DWORD dw;
	unsigned int dwID;
 } Sound;

static MCI_OPEN_PARMS mciOpen;
static MCI_PLAY_PARMS mciPlay;
static MCI_STATUS_PARMS mciStatus;
static const char bgmAssetPath[_BGM_ASSET_LEN_][64]= {
    "../../data/bgm/birdnoise.wav",
    "../../data/bgm/cymbal.wav",
    "../../data/bgm/gameover.wav",
    "../../data/bgm/megalovania.wav",
    "../../data/bgm/startmenu.wav",
};
static Sound bgmAsset[_BGM_ASSET_LEN_];
static const char voiceAssetPath[_VOICE_ASSET_LEN_][64]= {
    "../../data/voice/sans.wav",
};
static Sound voiceAsset[_VOICE_ASSET_LEN_];
static const char sfxAssetPath[_SFX_ASSET_LEN_][64]= {
    "../../data/sfx/attack.wav",
    "../../data/sfx/doorclose.wav",
    "../../data/sfx/gasterblaster.wav",
    "../../data/sfx/intronoise.wav",
    "../../data/sfx/moment.wav",
    "../../data/sfx/reflect.wav",
};

void initBGMAsset();
void initVoiceAsset();
void playBGM(BGMAssetType bgmType, SoundPlayType flag);
void playVoice(VoiceAssetType voiceType);
void playSFX(SFXAssetType sfxType);
/* Not using static vars */
void playSFXOnThread(SFXAssetType sfxType); 
void releaseBGMAsset();
void releaseVoiceAsset();
void releaseSoundAssets();
#endif