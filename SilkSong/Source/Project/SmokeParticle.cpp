#include "SmokeParticle.h"
#include "Components/ParticleSystem.h"

SmokeParticle::SmokeParticle()
{
	particle = ConstructComponent<ParticleSystem>();
	SetRootComponent(particle);
	particle->SetCapacity(40);
	particle->SetInterval(0.01);
	particle->Load("particle_grimm_smoke");
	particle->SetMaxSpeed(2000);
	particle->SetMinSpeed(1500);
	particle->SetPattern(EParticlePattern::Center);
	particle->SetCenter({ 50,200 }, { 0,360 });
	particle->SetFadingOutTime(0.4f);
	particle->SetLifeCycle(0.4f);
	particle->SetLayer(3);
	particle->SetGravity(-98.f);
	particle->SetSizeRange(0.3f, 0.75f);
	particle->SetTransparency(200);

	DestroyTimerHandle.Bind(5.f, [this]() { particle->SetIsLoop(false); }, false);
}
