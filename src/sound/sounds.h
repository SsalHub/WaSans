#ifndef __SOUNDS__
#define __SOUNDS__
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <Digitalv.h>
#include "../utils.h"

#define _SOUNDASSET_LEN_ 12

typedef enum SoundFileType
{
    _BGM_BIRDNOISE_			= 0,
    _BGM_CYMBAL_,
    _BGM_GAMEOVER_,
    _BGM_MEGALOVANIA_,
    _BGM_STARTMENU_,
    _VOICE_SANS_,
    _SFX_ATTACK_,
    _SFX_DOORCLOSE_,
    _SFX_GASTERBLASTER_,
    _SFX_INTRONOISE_,
    _SFX_MOMENT_,
    _SFX_REFLECT_,
} SoundFileType;

typedef enum SoundPlayType
{
    _SOUND_ONCE_ 			= 0,
    _SOUND_LOOP_,
    _SOUND_BEGIN_,
    _SOUND_PAUSE_,
} SoundPlayType;

static MCI_OPEN_PARMS mciOpen;
static MCI_PLAY_PARMS mciPlay[_SOUNDASSET_LEN_];
static MCI_STATUS_PARMS mciStatus[_SOUNDASSET_LEN_];
static DWORD sounds[_SOUNDASSET_LEN_];
static const char soundFilePath[_SOUNDASSET_LEN_][64]= {
    "../../data/bgm/birdnoise.wav",
    "../../data/bgm/cymbal.wav",
    "../../data/bgm/gameover.wav",
    "../../data/bgm/megalovania.wav",
    "../../data/bgm/startmenu.wav",
    "../../data/voice/sans.wav",
    "../../data/sfx/attack.wav",
    "../../data/sfx/doorclose.wav",
    "../../data/sfx/gasterblaster.wav",
    "../../data/sfx/intronoise.wav",
    "../../data/sfx/moment.wav",
    "../../data/sfx/reflect.wav",
};

void initSoundAsset();
void loadWav(int id, const char* wavPath);
void playBGM(SoundFileType fType, SoundPlayType flag);
void playVoice(SoundFileType fType);
void playSFX(SoundFileType fType);
void releaseSoundAsset();
#endif