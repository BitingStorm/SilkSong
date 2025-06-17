#include "SmokeParticle.h"
#include "Components/ParticleSystem.h"

SmokeParticle::SmokeParticle()
{
	particle = ConstructComponent<ParticleSystem>();
	SetRootComponent(particle);
	particle->SetCapacity(50);
	particle->SetInterval(0.01);
	particle->Load("particle_grimm_smoke");
	particle->SetMaxSpeed(1500);
	particle->SetMinSpeed(1000);
	particle->SetPattern(EParticlePattern::Center);
	particle->SetCenter({ 50,200 }, { 0,360 });
	particle->SetFadingOutTime(0.5f);
	particle->SetLifeCycle(0.4f);
	particle->SetLayer(3);
	particle->SetSizeRange(0.3f, 0.75f);
	particle->SetTransparency(200);

	DestroyTimerHandle.Bind(5.f, [this]() { particle->SetIsLoop(false); }, false);
}
