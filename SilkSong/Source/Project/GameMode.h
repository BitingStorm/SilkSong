#pragma once
#include "Objects/Actor.h"
#include "DamageSystem.h"
#include "Components/AudioPlayer.h"


class GameMode :public Actor
{
public:
	GameMode();

	DamageSystem* GetDamageSystem()const { return damageSystem; }

	AudioPlayer* GetAudioPlayer(int32 index)const 
	{
		switch (index)
		{
		case 0: return music;
		case 1: return sound;
		case 2: return music_;
		default: return nullptr;
		}
	}

protected:
	DamageSystem* damageSystem;

	AudioPlayer* music;

	AudioPlayer* sound;

	AudioPlayer* music_;
};