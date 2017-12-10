#pragma once

#include <iostream>
#include <Windows.h>
#include <mmsystem.h>

using namespace std;

class SoundSystem
{
public:
	void PlayMenuMusic()
	{
		PlaySound(TEXT("test.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

		//system("pause");
	}

	void PlayGameMusic()
	{
		PlaySound(TEXT("filename.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}

	void PlayShootingSound()
	{
		PlaySound(TEXT("filename.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}

	void PauseSoundSystem()
	{
		system("pause");
	}
};