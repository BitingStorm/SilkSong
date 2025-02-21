#include "DieParticle.h"
#include "Components/ParticleSystem.h"
#include "Components/AudioPlayer.h"
#include "GameModeHelper.h"


DieParticle::DieParticle()
{
	particle = ConstructComponent<ParticleSystem>();
	SetRootComponent(particle);
	particle->SetCapacity(50);
	particle->SetInterval(0);
	particle->SetIsLoop(false);
	particle->Load("particle_heal");
	particle->SetMaxSpeed(250);
	particle->SetMinSpeed(150);
	particle->SetPattern(EParticlePattern::Center);
	particle->SetCenter({ 25,75 });
	particle->SetFadingOutTime(0.5f);
	particle->SetFadingType(EParticleFadingType::ExpandAndShrink);
	particle->SetGravity(-9.8f);
	particle->SetLifeCycle(1.5f);
	particle->SetLayer(6);

	silk = ConstructComponent<ParticleSystem>();
	silk->AttachTo(root);
	silk->SetCapacity(75);
	silk->SetInterval(0.01f);
	silk->Load("particle_silk");
	silk->SetMaxSpeed(600);
	silk->SetMinSpeed(200);
	silk->SetPattern(EParticlePattern::Center);
	silk->SetCenter({ 40,80 });
	silk->SetGravity(-9.8f);
	silk->SetLifeCycle(0.75f);
	silk->SetSizeRange(0.4f, 0.8f);
	silk->SetLayer(5);
	silk->Deactivate();

	ChangeTimerHandle.Bind(0.5f, [this]() {silk->Activate(); GameModeHelper::PlayFXSound("sound_die"); }, false);
	DestroyTimerHandle.Bind(3.f, [this]() {Destroy();}, false);
}