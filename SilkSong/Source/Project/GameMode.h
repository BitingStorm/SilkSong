#pragma once
#include "Objects/Actor.h"
#include "DamageSystem.h"
#include "Components/AudioPlayer.h"
#include "WinUser.h"


class GameMode :public Actor
{
public:
	GameMode();

	virtual void Update(float deltaTime)override;

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

	int32 GetVolume(int32 index)const
	{
		return volumes[index];
	}

	void SetVolume(int32 index, int32 vol)
	{
		volumes[index] = vol; GetAudioPlayer(index)->SetVolume(vol);
	}

	void MakeEarRinging();

	void MakeEarRinging_();

	void RefreshVolume()
	{
		bEarRinging = false; timer = 0.f;
	}

	uint8 GetKeyCode(int index) const
	{
		return keyCodes[index];
	}

	void SetKeyCode(int index, uint8 value)
	{
		keyCodes[index] = value;
	}

protected:
	DamageSystem* damageSystem;

	AudioPlayer* music;

	AudioPlayer* sound;

	AudioPlayer* music_;

	int32 volumes[3];

	uint8 keyCodes[15] = { 'W','A','S','D','K','J','F','L','I','O','U','E','X','Q',0x20 };

	bool bEarRinging;
	float timer;
};