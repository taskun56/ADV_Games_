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
		PlaySound(TEXT("test.wav"), NULL, SND_FILENAME);
		
		//system("pause"); 
	}

	void PlayGameMusic()
	{
		PlaySound(TEXT("filename.wav"), NULL, SND_FILENAME);
	}

	void PlayShootingSound()
	{
		PlaySound(TEXT("filename.wav"), NULL, SND_FILENAME);
	}

	void PauseSoundSystem()
	{
		system("pause");
	}
};