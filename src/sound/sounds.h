#ifndef __SOUNDS__
#define __SOUNDS__
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <Digitalv.h>
#include "../utils.h"

#define _SOUNDASSET_LEN_ 3

static MCI_OPEN_PARMS mciOpen;
static MCI_PLAY_PARMS mciPlay[_SOUNDASSET_LEN_];
static MCI_STATUS_PARMS mciStatus[_SOUNDASSET_LEN_];
static DWORD sounds[_SOUNDASSET_LEN_];
static const char soundFilePath[_SOUNDASSET_LEN_][64]= {
    "../../data/bgm/startmenu.wav",
    "../../data/bgm/megalovania.wav",
    "../../data/voice/sans.wav",
};

typedef enum SoundFileType
{
    _BGM_STARTMENU_ 		= 0,
    _BGM_MEGALOVANIA_,
    _VOICE_SANS_,
} SoundFileType;

typedef enum SoundPlayType
{
    _SOUND_ONCE_ 			= 0,
    _SOUND_LOOP_,
    _SOUND_BEGIN_,
    _SOUND_PAUSE_,
} SoundPlayType;

void initSoundAsset();
void loadWav(int id, const char* wavPath);
void playBGM(SoundFileType fType, SoundPlayType flag);
void playVoice(SoundFileType fType);
void releaseSoundAsset();
#endif