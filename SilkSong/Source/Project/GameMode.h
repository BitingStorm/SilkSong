#pragma once
#include "Objects/Actor.h"
#include "DamageSystem.h"
#include "Components/AudioPlayer.h"


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
		volumes[index] = vol;
		GetAudioPlayer(index)->SetVolume(vol);
	}

	void MakeEarRinging()
	{
		bEarRinging = true;
		music->SetVolume(volumes[0] / 2);
		sound->SetVolume(volumes[1] / 2);
		music_->SetVolume(volumes[2] / 2);
	}

	void MakeEarRinging_()
	{
		bEarRinging = true;
		music->SetVolume(volumes[0] / 2);
		music_->SetVolume(volumes[2] / 2);
	}

	void RefreshVolume()
	{
		bEarRinging = false;
		timer = 0.f;
	}

protected:
	DamageSystem* damageSystem;

	AudioPlayer* music;

	AudioPlayer* sound;

	AudioPlayer* music_;

	int32 volumes[3];

	bool bEarRinging;
	float timer;
};