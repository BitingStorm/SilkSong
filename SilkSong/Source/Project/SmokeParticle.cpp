#include "SmokeParticle.h"
#include "Components/ParticleSystem.h"
#include "GameModeHelper.h"

SmokeParticle::SmokeParticle()
{
	particle = ConstructComponent<ParticleSystem>();
	SetRootComponent(particle);
	particle->SetCapacity(40);
	particle->SetInterval(0.01);
	particle->Load("particle_grimm_smoke");
	particle->SetMaxSpeed(300);
	particle->SetMinSpeed(200);
	particle->SetPattern(EParticlePattern::Center);
	particle->SetCenter({ 0,100 }, { -75,255 });
	particle->SetFadingOutTime(0.4f);
	particle->SetLifeCycle(0.4f);
	particle->SetLayer(3);
	particle->SetGravity(-2000.f);
	particle->SetSizeRange(0.3f, 0.75f);
	particle->SetTransparency(250);

	GameModeHelper::PlayFXSound("sound_boss_gushing");

	DestroyTimerHandle.Bind(4.5f, [this]() { particle->SetIsLoop(false); }, false);
}

void SmokeParticle::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	particle->SetMaxSpeed(300 + DestroyTimerHandle.GetDelay() * 75);
	particle->SetMinSpeed(200 + DestroyTimerHandle.GetDelay() * 50);
	particle->SetCenter({ 0,100.f + 30.f * float(DestroyTimerHandle.GetDelay()) }, { -30,210 });
}
