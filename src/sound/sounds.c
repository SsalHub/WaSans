#include "sounds.h"

void initSoundAsset()
{
	int i;
	for (i = 0; i < _SOUNDASSET_LEN_; i++)
	{
		loadWav(i, soundFilePath[i]);
	}
}

void loadWav(int id, const char* wavPath)
{
	mciOpen.lpstrDeviceType = "WaveAudio";
	mciOpen.lpstrElementName = wavPath;
	sounds[id] = mciSendCommand(id + 1, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&mciOpen);
}

void playBGM(SoundFileType fType, SoundPlayType flag)
{
	int dwID = fType + 1;
	switch (flag)
	{
		case _SOUND_BEGIN_:
			mciSendCommand(dwID, MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
			mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&(mciPlay[fType]));
			break;
			
		case _SOUND_PAUSE_:
			mciSendCommand(dwID, MCI_PAUSE, MCI_NOTIFY, (DWORD)(LPVOID)&(mciPlay[fType]));
			return;
			
		default:
			break;
	}
}

void playVoice(SoundFileType fType)
{
	int dwID = fType + 1;
	
	mciStatus[fType].dwItem = MCI_STATUS_MODE;
	mciSendCommand(dwID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPSTR)&(mciStatus[fType])); 
	if (mciStatus[fType].dwReturn != MCI_MODE_PLAY)		// if not playing this sound
	{
		mciSendCommand(dwID, MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
		mciSendCommand(dwID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&(mciPlay[fType]));
	}
}

void playSFX(SoundFileType fType)
{
	int dwID = fType + 1;
	mciSendCommand(dwID, MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
	mciSendCommand(dwID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&(mciPlay[fType]));
//	mciStatus[fType].dwItem = MCI_STATUS_MODE;
//	mciSendCommand(dwID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPSTR)&(mciStatus[fType])); 
//	if (mciStatus[fType].dwReturn != MCI_MODE_PLAY)		// if not playing this sound
//	{
//		mciSendCommand(dwID, MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
//		mciSendCommand(dwID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&(mciPlay[fType]));
//	}
}

void releaseSoundAsset()
{
	int i;
	for (i = 0; i < _SOUNDASSET_LEN_; i++)
	{
		mciSendCommand(i + 1, MCI_CLOSE, 0, (DWORD)(LPVOID)NULL);
	}
}