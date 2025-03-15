#include "HealParticle.h"
#include "Components/ParticleSystem.h"


HealParticle::HealParticle()
{
	particle = ConstructComponent<ParticleSystem>();
	SetRootComponent(particle);
	particle->SetCapacity(25);
	particle->SetInterval(0.01f);
	particle->Load("particle_heal");
	particle->SetMaxSpeed(275);
	particle->SetMinSpeed(175);
	particle->SetPattern(EParticlePattern::Center);
	particle->SetCenter({ 30,75 });
	particle->SetFadingOutTime(0.5f);
	particle->SetGravity(-9.8f);
	particle->SetLifeCycle(1.2f);
	particle->SetSizeRange(0.4f,0.8f);
	particle->SetIsLoop(false);
	particle->SetLayer(5);

	DestroyTimerHandle.Bind(2.25f, [this]() {Destroy(); }, false);
}