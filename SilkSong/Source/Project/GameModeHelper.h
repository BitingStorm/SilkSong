#pragma once
#include "GameMode.h"


class GameModeHelper
{
public:
	static GameMode* gameMode;

	static GameMode* GetInstance();

	static void ApplyDamage(IDamagable* damageCauser, IDamagable* damagedActor, float baseValue, EDamageType damageType);

	static void PlayFXSound(std::string path);

	static void PlayBGMusic(std::string path);
};