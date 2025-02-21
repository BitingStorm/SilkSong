#pragma once
#include "Objects/Actor.h"
#include "DamageSystem.h"
#include "Components/AudioPlayer.h"


class GameMode :public Actor
{
public:
	GameMode();

	DamageSystem* GetDamageSystem()const { return damageSystem; }

	AudioPlayer* GetAudioPlayer(int32 index)const { return index == 0 ? music : sound; }

protected:
	DamageSystem* damageSystem;

	AudioPlayer* music;

	AudioPlayer* sound;
};