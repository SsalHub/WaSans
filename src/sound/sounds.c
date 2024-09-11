#include "sounds.h"

void initBGMAsset()
{
	int i;
	for (i = 0; i < _BGM_ASSET_LEN_; i++)
	{
		mciOpen.lpstrDeviceType = "WaveAudio";
		mciOpen.lpstrElementName = bgmAssetPath[i];
		bgmAsset[i].dw = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&mciOpen);
		bgmAsset[i].dwID = mciOpen.wDeviceID;
	}
}

void initVoiceAsset()
{
	int i;
	for (i = 0; i < _VOICE_ASSET_LEN_; i++)
	{
		voiceAsset[i].dw = 0;
		voiceAsset[i].dwID = 0;
	}
}

void playBGM(BGMAssetType bgmType, SoundPlayType flag)
{
	switch (flag)
	{
		case _SOUND_BEGIN_:
			mciSendCommand(bgmAsset[bgmType].dwID, MCI_SEEK, MCI_SEEK_TO_START, NULL);
			mciSendCommand(bgmAsset[bgmType].dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&mciPlay);
			break;
			
		case _SOUND_RESUME_:
			mciSendCommand(bgmAsset[bgmType].dwID, MCI_RESUME, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlay);
			return;
			
		case _SOUND_PAUSE_:
			mciSendCommand(bgmAsset[bgmType].dwID, MCI_PAUSE, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlay);
			return;
			
		default:
			break;
	}
}

void playVoice(VoiceAssetType voiceType)
{
	// check wav is running
	mciStatus.dwItem = MCI_STATUS_MODE;
	mciSendCommand(voiceAsset[voiceType].dwID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPSTR)&mciStatus); 
	if (mciStatus.dwReturn != MCI_MODE_PLAY)		// if not playing this sound
	{
		mciSendCommand(voiceAsset[voiceType].dwID, MCI_SEEK, MCI_SEEK_TO_START, NULL);
		mciSendCommand(voiceAsset[voiceType].dwID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlay);
	}
}

void playSFX(SFXAssetType sfxType)
{
	DWORD dw;
	unsigned int dwID;
	
	// open wav
	mciOpen.lpstrDeviceType = "WaveAudio";
	mciOpen.lpstrElementName = sfxAssetPath[sfxType];
	dw = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&mciOpen);
	dwID = mciOpen.wDeviceID;
	// play wav
	mciSendCommand(dwID, MCI_SEEK, MCI_SEEK_TO_START, NULL);
	mciSendCommand(dwID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlay);
}


void releaseBGMAsset()
{
	int i;
	for (i = 0; i < _BGM_ASSET_LEN_; i++)
	{
		mciSendCommand(bgmAsset[i].dwID, MCI_CLOSE, 0, NULL);
	}
}

void releaseVoiceAsset()
{
	int i;
	for (i = 0; i < _VOICE_ASSET_LEN_; i++)
	{
		mciSendCommand(voiceAsset[i].dwID, MCI_CLOSE, 0, NULL);
	}
}

void releaseSoundAssets()
{
	if (mciSendCommand(MCI_ALL_DEVICE_ID, MCI_CLOSE, MCI_WAIT, NULL))
	{
		perror("release sound assets failed.");
		exit(1);
	}
}

unsigned __stdcall releaseSoundAuto(void* args)
{
	unsigned int dwID = *((unsigned int*)args);
	MCI_STATUS_PARMS localStatus;
	
	// check wav is running
	localStatus.dwItem = MCI_STATUS_MODE;
	mciSendCommand(dwID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPSTR)&localStatus); 
	while (localStatus.dwReturn == MCI_MODE_PLAY)		// if not playing this sound
	{
		waitForFrame();
	}
	mciSendCommand(dwID, MCI_CLOSE, 0, NULL);
}



/* Using On Multi-Threads Only */
void playVoiceOnThread(VoiceAssetType voiceType)
{
	MCI_OPEN_PARMS localOpen;
	MCI_PLAY_PARMS localPlay;
	MCI_STATUS_PARMS localStatus;
	
	if (!voiceAsset[voiceType].dwID)
	{
		// open wav
		localOpen.lpstrDeviceType = "WaveAudio";
		localOpen.lpstrElementName = voiceAssetPath[voiceType];
		voiceAsset[voiceType].dw = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&localOpen);
		voiceAsset[voiceType].dwID = localOpen.wDeviceID;
	}
	// check wav is running
	localStatus.dwItem = MCI_STATUS_MODE;
	mciSendCommand(voiceAsset[voiceType].dwID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPSTR)&localStatus); 
	if (localStatus.dwReturn != MCI_MODE_PLAY)		// if not playing this sound
	{
		mciSendCommand(voiceAsset[voiceType].dwID, MCI_SEEK, MCI_SEEK_TO_START, NULL);
		mciSendCommand(voiceAsset[voiceType].dwID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&localPlay);
	}
}

void playSFXOnThread(SFXAssetType sfxType)
{
	MCI_OPEN_PARMS localOpen;
	MCI_PLAY_PARMS localPlay;
	DWORD dw;
	unsigned int dwID;
	
	// open wav
	localOpen.lpstrDeviceType = "WaveAudio";
	localOpen.lpstrElementName = sfxAssetPath[sfxType];
	dw = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&localOpen);
	dwID = localOpen.wDeviceID;
	// play wav
	mciSendCommand(dwID, MCI_SEEK, MCI_SEEK_TO_START, NULL);
	mciSendCommand(dwID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&localPlay);
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)releaseSoundAuto, &dwID, 0, NULL);
}