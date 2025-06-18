#include "GameMode.h"


GameMode::GameMode()
{
	damageSystem = ConstructComponent<DamageSystem>();
	music = ConstructComponent<AudioPlayer>();
	sound = ConstructComponent<AudioPlayer>();
	music_ = ConstructComponent<AudioPlayer>();

	for (int i = 0; i < 3; i++)
	{
		volumes[i] = GetAudioPlayer(0)->GetVolume();
	}

	timer = 0.5f;
	bEarRinging = false;
}

void GameMode::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	if (!bEarRinging && timer < 0.5f)
	{
		timer += deltaTime;
		music->SetVolume(volumes[0] * (timer + 0.5f));
		if(sound->GetVolume() < volumes[1])sound->SetVolume(volumes[1] * (timer + 0.5f));
		music_->SetVolume(volumes[2] * (timer + 0.5f));
		if (timer >= 0.5f)
		{
			music->SetVolume(volumes[0]);
			sound->SetVolume(volumes[1]);
			music_->SetVolume(volumes[2]);
		}
	}
}
