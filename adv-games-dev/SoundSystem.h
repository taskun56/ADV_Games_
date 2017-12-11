#pragma once

#include <iostream>
#include <Windows.h>
#include <mmsystem.h>

using namespace std;

class SoundSystem
{
public:
	//R// --> https://social.msdn.microsoft.com/Forums/en-US/37e37989-357d-4258-bd9a-aa0e1e2fd4a6/how-to-adjust-the-volume-of-sound?forum=vssmartdevicesnative
	//OFF = 0		LOW = 858993459			NORMAL = 1717986918			MEDIUM = -1717986919		HIGH = -858993460			VERY_HIGH = -1
	int MUTE = 0,	LOW = 858993459,		NORMAL = 1717986918;		// , MEDIUM = -1717986919, HIGH = -858993460;			

	//Couldn't find a way to dynamically alter the sound properly so I've just cut down to mute low and normal 
	//So if you press M you get mute, + for normal and - for low

	//... aw crap I didn't consider controllers...

	
	void PlayMenuMusic(int volume)
	{
		waveOutSetVolume(0, volume);
		PlaySound(TEXT("test.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}

	void PlayGameMusic(int volume)
	{
		waveOutSetVolume(0, volume);
		PlaySound(TEXT("filename.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}

	//Keep SFX volume to normal but let the music be alterable
	void PlayShootingSound()
	{
		//R//Leave the 0, the value on the right affects the volume
		waveOutSetVolume(0, NORMAL);
		PlaySound(TEXT("filename.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}

	
	void TurnUpMusic()
	{
		//If user pressed + 
		waveOutSetVolume(0, NORMAL);
	}

	void TurnDownMusic()
	{
		//If user pressed - 
		waveOutSetVolume(0, LOW);
	}

	void MuteMusic()
	{
		//If user pressed M
		waveOutSetVolume(0, MUTE);
	}
};